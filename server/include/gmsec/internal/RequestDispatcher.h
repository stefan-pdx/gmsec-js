
/* Copyright 2007-2010 United States Government as represented by the     */
/* Administrator of The National Aeronautics and Space Administration. All */
/* Rights Reserved.                                                        */


/** @file RequestDispatcher.h
 *
 *  @brief Perform asynchronous, automated delivery of request/reply with callback
 */

#ifndef gmsec_internal_RequestDispatcher_h
#define gmsec_internal_RequestDispatcher_h

#include <gmsec/Status.h>
#include <gmsec/util/wdllexp.h>
#include <gmsec/util/Thread.h>
#include <gmsec/internal/BaseConnection.h>

namespace gmsec
{

class Callback;
class Message;
class ReplyCallback;

namespace internal
{


/**	@class RequestDispatcher
 *
 *	@brief Threaded class to automatically handle delivery of reply
 *	from a request via a callback
 *
 */
class GMSEC_API RequestDispatcher : public gmsec::util::Thread
{
public:
	RequestDispatcher();
	virtual ~RequestDispatcher();

	/**	@fn init( BaseConnection *conn, Message *req, GMSEC_I32 timeout, Callback *cb )
	 *
	 *	@brief Initialize the dispatcher.  This method must be called prior
	 *	to calling the run() method. NOTE: the request & callback need to
	 *	be dynamic memory because they will be cleaned up with this
	 *	dispatcher is cleaned up.
	 *
	 *	@param conn The connection on which the request will be sent
	 *	@param req The request to send
	 *	@param cb The callback to call when the reply is received (will not be called if timed out)
	 *
	 *	@return bool - false if the connection, request, or callback is null,
	 *		or if trying to initialize while the dispatcher is running.  True on success
	 *
	 */
	virtual bool CALL_TYPE init(BaseConnection *conn, Message *req, GMSEC_I32 timeout, Callback *cb);

	/**	@fn init( Connection *conn, Message *req, GMSEC_I32 timeout, ReplyCallback *cb )
	 *
	 *	@brief Initialize the dispatcher.  This method must be called prior
	 *	to calling the run() method. NOTE: the request & callback need to
	 *	be dynamic memory because they will be cleaned up with this
	 *	dispatcher is cleaned up.
	 *
	 *	THIS VERSION IS PROVIDED FOR THE MORE SPECIFIC ReplyCallback() interface.
	 *
	 *	@param conn The connection on which the request will be sent
	 *	@param req The request to send
	 *	@param cb The callback to call when the reply is received (will not be called if timed out)
	 *
	 *	@return bool - false if the connection, request, or callback is null,
	 *		or if trying to initialize while the dispatcher is running.  True on success
	 *
	 */
	virtual bool CALL_TYPE init(BaseConnection *conn, Message *req, GMSEC_I32 timeout, ReplyCallback *cb);

	/**	@fn run()
	 *
	 *	@brief Overload the thread run method to perform the dispatch
	 *	task.  The init() method must be called prior to this
	 *	method, else the method will fail.
	 *
	 */
	virtual void CALL_TYPE run();

	/** @fn destroy()
	 *
	 *	@brief Terminate the active, running thread and perform thread cleanup.
	 *	No action is performed if the thread is not running.
	 */
	virtual void CALL_TYPE destroy();

	/**	@fn GetStatus()
	 *
	 *	@brief Returns the status of the thread
	 *
	**/
	Status CALL_TYPE GetStatus()
	{
		return fStatus;
	}

	/**	@fn isRunning()
	 *
	 *	@brief Checks to see if the thread is running
	 *
	**/
	virtual bool isRunning()
	{
		return fRunning;
	}

private:

	BaseConnection *fConn;
	Message *fRequest;
	Message *fReply;
	Callback *fCallbk;
	ReplyCallback *fRCallbk;

	volatile GMSEC_I32 fTimeout;

	Status fStatus;

	volatile bool fRunning;
	volatile bool mAlive;
};

} //namespace internal
} //namespace gmsec

#endif /* gmsec_internal_RequestDispatcher_h */


