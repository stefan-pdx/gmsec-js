/*
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <iostream>
#include <map>
#include <string.h>


#include "v8.h"
#include "node.h"
#include "gmsec_cpp.h"
#include "gmsec\util\Log.h"
#include "gmsec\util\Mutex.h"

using namespace std;
using namespace node;
using namespace v8;

#define REQ_FUN_ARG(I, VAR)                                             \
  if (args.Length() <= (I) || !args[I]->IsFunction())                   \
    return ThrowException(Exception::TypeError(                         \
                  String::New("Argument " #I " must be a function")));  \
  Local<Function> VAR = Local<Function>::Cast(args[I]);

#define REQ_STR_ARG(I, VAR)                                             \
  if (args.Length() <= (I) || !args[I]->IsString())                     \
	return ThrowException(Exception::TypeError(                         \
				  String::New("Argument " #I " must be a string")));  \
  Local<String> VAR = Local<String>::Cast(args[I]);

class Connection: ObjectWrap{

private:

public:
	/*
	 * Forward declarations
	 */
	class MessageReceivedCallback;

	gmsec::Connection *gmsecConnection;
	map<const char*, MessageReceivedCallback*> subscribeCallbacks;
	static Persistent<FunctionTemplate> s_ct;

	/*
	 * Message batons used for callbacks.
	 */
	struct message_baton_t {
			Connection *connection;
			char *subject;
			MessageReceivedCallback *gmsecCb;
	};

	struct connection_baton_t {
		Connection *connection;
		Persistent<Function> cb;
		const char *server;
	};

	struct message_received_cb_baton_t {
		gmsec::Connection *conn;
		Persistent<Function> cb;
		gmsec::Message *copied_message;
		const char *message_contents;
	};

	struct publish_baton_t {
		Connection *connection;
		const char *message_contents;
	};

	class InfoHandler : public gmsec::util::LogHandler{
	public:
		virtual void CALL_TYPE OnMessage(const gmsec::util::LogEntry &entry)
		{
			std::cout << "On INFO Message:" << std::endl;
			char tempBuffer[50];
			gmsec::util::formatTime_s(entry.time, tempBuffer);
			std::cout << "  " << tempBuffer << ":" << gmsec::util::Log::ToString(entry.level) << ":" << entry.message << endl;
		}
	};

	class MessageReceivedCallback : public gmsec::Callback {
	public:
		Persistent<Function> cb;
		virtual void CALL_TYPE OnMessage(gmsec::Connection *conn, gmsec::Message *msg){
			
			/* Create the baton to pass it into the main thread. */
			message_received_cb_baton_t *baton = new message_received_cb_baton_t();
			baton->cb = cb;
			baton->conn = conn;

			conn->CreateMessage(baton->copied_message);
			conn->CloneMessage(msg, baton->copied_message);

			uv_work_t *req = new uv_work_t;
			req->data = baton;
			uv_queue_work(uv_default_loop(), req, EIO_OnMessage, EIO_AfterOnMessage);
		}
	};

	static void EIO_OnMessage(uv_work_t *req){
		if(req->queued_bytes > 0){
			message_received_cb_baton_t *baton = static_cast<message_received_cb_baton_t*>(req->data);
			baton->copied_message->ToXML(baton->message_contents);
			req->data = baton;
		}
	}

	static void EIO_AfterOnMessage(uv_work_t *req){
		if(req->queued_bytes > 0){
			HandleScope scope;

			message_received_cb_baton_t *baton = (message_received_cb_baton_t*)req->data;

			Handle<Value> argv[1];
			argv[0] = String::New(baton->message_contents);

			TryCatch try_catch;
			baton->cb->Call(Context::GetCurrent()->Global(), 1, argv);

			baton->conn->DestroyMessage(baton->copied_message);

			delete baton;
			delete req;

			if (try_catch.HasCaught())
				FatalException(try_catch);
		}
	}

	static void Init(Handle<Object> target){
		HandleScope scope;

		Local<FunctionTemplate> t = FunctionTemplate::New(New);

		s_ct = Persistent<FunctionTemplate>::New(t);
		s_ct->InstanceTemplate()->SetInternalFieldCount(1);
		s_ct->SetClassName(String::NewSymbol("Connection"));

		NODE_SET_PROTOTYPE_METHOD(s_ct, "Connect", Connect);
		NODE_SET_PROTOTYPE_METHOD(s_ct, "Subscribe", Subscribe);
		NODE_SET_PROTOTYPE_METHOD(s_ct, "Publish", Publish);

		target->Set(String::NewSymbol("Connection"), s_ct->GetFunction());
	}

	Connection(){
	}

	~Connection(){
	}

	static Handle<Value> New(const Arguments& args){
	    HandleScope scope;

	    Connection *connection = new Connection();

	    connection->Wrap(args.This());
	    return args.This();
	}

	static Handle<Value> Publish(const Arguments& args){
		HandleScope scope;

		REQ_STR_ARG(0, subscribeV8Str);

		Connection *connection = ObjectWrap::Unwrap<Connection>(args.This());

		char *message_contents = new char[ strlen(*String::AsciiValue(subscribeV8Str)) ];
		strcpy(message_contents, *String::AsciiValue(subscribeV8Str));

		/* Populate a baton to pass it to the eio library. */
		publish_baton_t *baton = new publish_baton_t();
		baton->connection = connection;
		baton->message_contents = message_contents;

		 uv_work_t *req = new uv_work_t;
		 req->data = baton;

		/* Submit the subscription command to the thread pool and return. */
		uv_queue_work(uv_default_loop(), req, EIO_Publish, EIO_AfterPublish);

		return Undefined();
	}

	static void EIO_Publish(uv_work_t *req){
		gmsec::Status result;

		publish_baton_t *baton = static_cast<publish_baton_t*>(req->data);

		/* Load the user data into a new message. */
		gmsec::Message *msg;
		baton->connection->gmsecConnection->CreateMessage(msg);
		msg->FromXML(baton->message_contents);

		const char *tmp;
		msg->GetSubject(tmp);

		baton->connection->gmsecConnection->Publish(msg);
		baton->connection->gmsecConnection->DestroyMessage(msg);
	}

	static void EIO_AfterPublish(uv_work_t *req){
		publish_baton_t *baton = static_cast<publish_baton_t *>(req->data);

		delete baton;
		delete req;
	}

	static Handle<Value> Subscribe(const Arguments& args){
		HandleScope scope;

		REQ_STR_ARG(0, subscribeV8Str);
		REQ_FUN_ARG(1, subscribeCb);

		Connection *connection = ObjectWrap::Unwrap<Connection>(args.This());

		/* Create a new instance of the generic message callback and add it
		 * to our collection so that we can track the various callback instances.
		 * Note that we're going to refer to these when we Unsubscribe so that
		 *  we can properly delete the instances of GenericPublichBallback.
		 */
		MessageReceivedCallback *gmsecCb = new MessageReceivedCallback();
		gmsecCb->cb = Persistent<Function>::New(subscribeCb);

		/* Extract out the subject string from the V8::String object. */
		char *subscribeStr = new char[ strlen(*String::AsciiValue(subscribeV8Str)) ];
		strcpy(subscribeStr, *String::AsciiValue(subscribeV8Str));

		/* Log this subscription with the callback collection. */
		connection->subscribeCallbacks.insert( make_pair(subscribeStr, gmsecCb) );

		/* Populate a baton to pass it to the eio library. */
		message_baton_t *baton = new message_baton_t();
		baton->connection = connection;
		baton->subject = subscribeStr;
		baton->gmsecCb = gmsecCb;

		 uv_work_t *req = new uv_work_t;
		 req->data = baton;

		/* Submit the subscription command to the thread pool and return. */
		uv_queue_work(uv_default_loop(), req, EIO_Subscribe, EIO_AfterSubscribe);

		return Undefined();
	}

	static void EIO_Subscribe(uv_work_t *req){
		gmsec::Status result;

		/* Extract out the baton and execute the subscribe command. */
		message_baton_t *baton = static_cast<message_baton_t*>(req->data);

		baton->connection->gmsecConnection->Subscribe(baton->subject, baton->gmsecCb);
	}

	static void EIO_AfterSubscribe(uv_work_t *req){
		HandleScope scope;

		/* Extract out the baton */
		message_baton_t *baton = static_cast<message_baton_t *>(req->data);

		delete baton;
		delete req;
	}

	static Handle<Value> Connect(const Arguments& args){
		HandleScope scope;

		REQ_STR_ARG(0, server);
		REQ_FUN_ARG(1, cb);

		/* Extract out the server string from the V8::String object. */
		char *serverStr = new char[ strlen(*String::AsciiValue(server)) ];
		strcpy(serverStr, *String::AsciiValue(server));

		Connection *connection = ObjectWrap::Unwrap<Connection>(args.This());

		connection_baton_t *baton = new connection_baton_t();
		baton->connection = connection;
		baton->cb = Persistent<Function>::New(cb);
		baton->server = serverStr;

		uv_work_t *req = new uv_work_t;
		req->data = baton;

		uv_queue_work(uv_default_loop(), req, EIO_Connect, EIO_AfterConnect);

		return Undefined();
	}

	static void EIO_Connect(uv_work_t *req){
		gmsec::Status result;

		connection_baton_t *baton = static_cast<connection_baton_t*>(req->data);

		gmsec::Config gmsecConfig = gmsec::Config();
		gmsecConfig.AddValue("connectiontype", "gmsec_mb");
		gmsecConfig.AddValue("server", baton->server);
		gmsecConfig.AddValue("loglevel", "VERBOSE");

		result = gmsec::ConnectionFactory::Create(&gmsecConfig, baton->connection->gmsecConnection);

		if(result.isError() == true){
			printf(result.Get());
			printf(result.GetString());
		}
		
		result = baton->connection->gmsecConnection->Connect();
		result = baton->connection->gmsecConnection->StartAutoDispatch();
	}

	static void EIO_AfterConnect(uv_work_t *req){
		
		HandleScope scope;

		connection_baton_t *baton = static_cast<connection_baton_t *>(req->data);

		TryCatch try_catch;

		baton->cb->Call(Context::GetCurrent()->Global(), 0, NULL);

		if (try_catch.HasCaught()) {
		      FatalException(try_catch);
		}

		baton->cb.Dispose();

		delete baton;
		delete req;
	}
};

Persistent<FunctionTemplate> Connection::s_ct;

static void init (Handle<Object> target)
{
	Connection::Init(target);
}

NODE_MODULE(gmsec, init);
