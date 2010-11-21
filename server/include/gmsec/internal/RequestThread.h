
/* Copyright 2007-2010 United States Government as represented by the     */
/* Administrator of The National Aeronautics and Space Administration. All */
/* Rights Reserved.                                                        */


/** @file RequestThread.h
 *
 *  @brief Interface between BaseConnection and middleware implementation
 *  of request / reply.
 */

#ifndef gmsec_internal_RequestThread_h
#define gmsec_internal_RequestThread_h

#include <string>
#include <list>

#include <gmsec/Status.h>
#include <gmsec/util/Thread.h>
#include <gmsec/util/Condition.h>
#include <gmsec/util/shared_ptr.h>
#include <gmsec/util/CountDownLatch.h>

namespace gmsec
{

class Callback;
class Connection;
class Message;
class ReplyCallback;

namespace internal
{

class BaseConnection;


/**	@class PendingRequest
*	@brief Holds state of a pending request
*/

class PendingRequest
{
public:
	Message *request;
	Message *reply;
	bool needToSend;
	bool neverExpire;
	double publishTime_s;
	double expireTime_s;
	GMSEC_I32 republish_ms;
	ReplyCallback *replyCallback;
	Callback *oldCallback;
	std::string id;
	bool sentOnce;

	gmsec::util::CountDownLatch sentLatch;

	PendingRequest(Message *request, GMSEC_I32 timeout, GMSEC_I32 republish_ms);

	~PendingRequest();
};



/**	@class RequestThread
 *
 *	@brief Threaded class to automatically handle delivery of reply
 *	from a request via a callback
 *
 *	@todo This could use BaseConnection / BaseMessage
 *
 */
class RequestThread : public gmsec::util::Thread
{
public:

	enum Reasons
	{
		QUIT = gmsec::util::Condition::USER,
		REQUEST,
		RECEIVE_REPLY,
		DELIVER_REPLY
	};

	RequestThread(BaseConnection *connection);
	~RequestThread();


	bool add(gmsec::util::shared_ptr<PendingRequest> &pending);

	void onReply(Message *reply);


	/**	@fn run()
	 *	@brief Overload the thread run method to send requests
	 *	and process replies.
	 */
	virtual void CALL_TYPE run();

	void shutdown();

	bool isRunning()
	{
		return fRunning;
	}
	bool isComplete()
	{
		return fComplete;
	}

	gmsec::util::Condition &getCondition()
	{
		return fCondition;
	}


private:

	void sendRequests();
	int waitLimit_ms();
	void deliverReplies();
	PendingRequest *findPending(const std::string &id);
	void deliverReply(Message *reply);


	BaseConnection *fConnection;
	std::list<gmsec::util::shared_ptr<PendingRequest> > fPending;
	std::list<Message*> fReplies;
	gmsec::util::Condition fCondition;

	volatile bool fRunning;
	volatile bool fComplete;

};

} // namespace internal
} // namespace gmsec

#endif /* gmsec_internal_RequestThread_h */

