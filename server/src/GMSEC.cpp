#include <iostream>
#include <map>

#include "v8.h"
#include "node.h"
#include "gmsec_cpp.h"

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
	gmsec::Connection *gmsecConnection;
	map<char, Callback> subscribeCallbacks;

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

	class GenericPublishCallback : public Callback
	{
	public:
		Persistent<Function> cb;
		virtual void CALL_TYPE OnMessage(Connection *conn, Message *msg){

			}
	};

	struct message_baton_t {
		Connection *connection;
		String subject;
		GenericPublishCallback gmsecCb;
		Persistent<Function> cb;
	};

	struct connection_baton_t {
		Connection *connection;
		Persistent<Function> cb;

	  };

	static Handle<Value> Subscribe(const Arguments& args){
		HandleScope scope;

		REQ_STR_ARG(0, subscribeStr);
		REQ_FUN_ARG(1, cb);

		Connection *connection = ObjectWrap::Unwrap<Connection>(args.This());

		// Create a new instance of the generic message callback and add it
		// to our collection so that we can track the various callback instances.
		// Note that we're going to refer to these when we Unsubscribe so that
		// we can properly delete the instances of GenericPublichBallback.
		GenericPublishCallback gmsecCb = new GenericPublishCallback();
		gmsecCb.cb = cb;
		connection->subscribeCallbacks[*(String::AsciiValue(subscribeStr))] = gmsecCb;

		message_baton_t *baton = new message_baton_t();
		baton->connection = connection;
		baton->subject = *(String::AsciiValue(subscribeStr));

		eio_custom(EIO_Subscribe, EIO_PRI_DEFAULT, EIO_AfterSubscribe, baton);

		ev_ref(EV_DEFAULT_UC);

		return Undefined();
	}

	static int EIO_Subscribe(eio_req *req){
		gmsec::Status result;

		message_baton_t *baton = static_cast<message_baton_t*>(req->data);

		baton->connection->gmsecConnection->Subscribe(baton->subject, baton->gmsecCb);
	}

	static int EIO_AfterSubscribe(eio_req *req){
		HandleScope scope;

		connection_baton_t *baton = static_cast<connection_baton_t *>(req->data);

		ev_unref(EV_DEFAULT_UC);

		baton->connection->Unref();

		Local<Value> argv[1];

		argv[0] = String::New("Hello World");

		TryCatch try_catch;

		baton->cb->Call(Context::GetCurrent()->Global(), 1, argv);

		if (try_catch.HasCaught()) {
		      FatalException(try_catch);
		}

		baton->cb.Dispose();

		delete baton;

		return 0;
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

		return 0;
	}

	static int EIO_AfterConnect(eio_req *req){
		HandleScope scope;

		connection_baton_t *baton = static_cast<connection_baton_t *>(req->data);

		ev_unref(EV_DEFAULT_UC);

		baton->connection->Unref();

		Local<Value> argv[1];

		argv[0] = String::New("Hello World");

		TryCatch try_catch;

		baton->cb->Call(Context::GetCurrent()->Global(), 1, argv);

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
