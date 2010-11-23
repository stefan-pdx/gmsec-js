#include <iostream>
#include <map>
#include <string.h>

#include "v8.h"
#include "node.h"
#include "gmsec_cpp.h"
#include "Log.h"

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

class Message: ObjectWrap{


};

class Connection: ObjectWrap{

private:

public:

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

	class GenericPublishCallback : public gmsec::Callback{
	public:
		Persistent<Function> cb;
		virtual void CALL_TYPE OnMessage(gmsec::Connection *conn, gmsec::Message *msg){
				const char *tmp;
				msg->ToXML(tmp);
				cout << tmp << endl;
			};
	};

	gmsec::Connection *gmsecConnection;
	map<const char*, GenericPublishCallback*> subscribeCallbacks;

	static Persistent<FunctionTemplate> s_ct;

	static void Init(Handle<Object> target){
		HandleScope scope;

		Local<FunctionTemplate> t = FunctionTemplate::New(New);

		s_ct = Persistent<FunctionTemplate>::New(t);
		s_ct->InstanceTemplate()->SetInternalFieldCount(1);
		s_ct->SetClassName(String::NewSymbol("Connection"));

		NODE_SET_PROTOTYPE_METHOD(s_ct, "Connect", Connect);
		NODE_SET_PROTOTYPE_METHOD(s_ct, "Subscribe", Subscribe);

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

	struct message_baton_t {
		Connection *connection;
		char *subject;
		GenericPublishCallback *gmsecCb;
		Persistent<Function> cb;
	};

	struct connection_baton_t {
		Connection *connection;
		Persistent<Function> cb;
	  };

	static Handle<Value> Subscribe(const Arguments& args){
		HandleScope scope;

		REQ_STR_ARG(0, subscribeV8Str);
		REQ_FUN_ARG(1, subscribeCb);
		REQ_FUN_ARG(2, immediateCb);

		Connection *connection = ObjectWrap::Unwrap<Connection>(args.This());

		// Create a new instance of the generic message callback and add it
		// to our collection so that we can track the various callback instances.
		// Note that we're going to refer to these when we Unsubscribe so that
		// we can properly delete the instances of GenericPublichBallback.
		GenericPublishCallback *gmsecCb = new GenericPublishCallback();
		gmsecCb->cb = Persistent<Function>::New(subscribeCb);

		// Extract out the subject string from the V8::String object.
		char *subscribeStr = new char[512];
		strcpy(subscribeStr, *String::AsciiValue(subscribeV8Str));

		// Log this subscription with the callback collection.
		connection->subscribeCallbacks.insert( make_pair(subscribeStr, gmsecCb) );

		// Populate a baton to pass it to the eio library.
		message_baton_t *baton = new message_baton_t();
		baton->connection = connection;
		baton->subject = subscribeStr;
		baton->gmsecCb = gmsecCb;
		baton->cb = Persistent<Function>::New(immediateCb);

		// Submit the subscription command to the thread pool and return.
		eio_custom(EIO_Subscribe, EIO_PRI_DEFAULT, EIO_AfterSubscribe, baton);
		ev_ref(EV_DEFAULT_UC);
		return Undefined();
	}

	static int EIO_Subscribe(eio_req *req){
		gmsec::Status result;

		// Extract out the baton and execute the subscribe command.
		message_baton_t *baton = static_cast<message_baton_t*>(req->data);
		baton->connection->gmsecConnection->Subscribe(baton->subject, baton->gmsecCb);
		return 0;
	}

	static int EIO_AfterSubscribe(eio_req *req){
		HandleScope scope;

		// Extract out the baton
		message_baton_t *baton = static_cast<message_baton_t *>(req->data);
		ev_unref(EV_DEFAULT_UC);
		baton->connection->Unref();

		// Execute the post-subscribe immediate callback.
		Local<Value> argv[0];
		TryCatch try_catch;
		baton->cb->Call(Context::GetCurrent()->Global(), 0, argv);

		if (try_catch.HasCaught()) {
		      FatalException(try_catch);
		}

		// Note that we are only disposing the immediate callback and not the
		// instance of the GenericPublishCallback which will remain active as
		// new messages are received. That corresponding callback should be
		// disposed properly with the Unscubscribe callback.
		baton->cb.Dispose();
		delete baton;
		return 0;
	}

	static int EIO_GMSEC_Subscribe(eio_req *req){

	}

	static int EIO_GMSEC_AfterSubscribe(eio_req *req){

	}

	static Handle<Value> Connect(const Arguments& args){
		/*
		 * Connect to a GMSEC server given a file location.
		 *
		 * Argument 1: File location
		 * Argument 2: Post-connect callback
		 */

		HandleScope scope;

		REQ_FUN_ARG(0, cb);

		Connection *connection = ObjectWrap::Unwrap<Connection>(args.This());

		connection_baton_t *baton = new connection_baton_t();
		baton->connection = connection;
		baton->cb = Persistent<Function>::New(cb);

		/*
		 * TODO: Extract out properties from Javascript object and attach to connection_baton_t
		 * and our connection instance.
		 */

		eio_custom(EIO_Connect, EIO_PRI_DEFAULT, EIO_AfterConnect, baton);

		ev_ref(EV_DEFAULT_UC);

		return Undefined();
	}

	static int EIO_Connect(eio_req *req){
		gmsec::Status result;

		connection_baton_t *baton = static_cast<connection_baton_t*>(req->data);

		gmsec::Config gmsecConfig = gmsec::Config();
		gmsecConfig.AddValue("connectiontype", "gmsec_mb");
		gmsecConfig.AddValue("server", "127.0.0.1");
		gmsecConfig.AddValue("loglevel", "VERBOSE");

		InfoHandler *infoHandler = new InfoHandler();
		gmsec::util::Log::RegisterHandler(logDEBUG, infoHandler);
		gmsec::util::Log::SetReportingLevel(logDEBUG);

		result = gmsec::ConnectionFactory::Create(&gmsecConfig, baton->connection->gmsecConnection);

		if (result.GetClass() != GMSEC_STATUS_NO_ERROR){
			cout << result.Get() << endl;
			return -1;
		}

		result = baton->connection->gmsecConnection->Connect();
		if (GMSEC_STATUS_NO_ERROR != result.GetClass())
		{
			cout << result.Get() << endl;
			return -1;
		}

		result = baton->connection->gmsecConnection->StartAutoDispatch();


		return 0;
	}

	static int EIO_AfterConnect(eio_req *req){
		HandleScope scope;

		connection_baton_t *baton = static_cast<connection_baton_t *>(req->data);

		ev_unref(EV_DEFAULT_UC);

		baton->connection->Unref();

		Local<Value> argv[0];

		TryCatch try_catch;

		baton->cb->Call(Context::GetCurrent()->Global(), 0, argv);

		if (try_catch.HasCaught()) {
		      FatalException(try_catch);
		}

		baton->cb.Dispose();

		delete baton;

		return 0;
	}
};

Persistent<FunctionTemplate> Connection::s_ct;

extern "C" {
  static void init (Handle<Object> target)
  {
	  Connection::Init(target);
  }

  NODE_MODULE(gmsec, init);
}
