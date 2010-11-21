
/* Copyright 2007-2010 United States Government as represented by the     */
/* Administrator of The National Aeronautics and Space Administration. All */
/* Rights Reserved.                                                        */


/** @file ReplyCallback.h
 *
 *  @author Brian D Gregory
 *  @author Interface & Control Systems, Inc.
 *  @date July 20, 2005
 *
 *  @brief This file contains the abstract base class for asynchronous request-reply callbacks
 *
**/

#ifndef gmsec_ReplyCallback_h
#define gmsec_ReplyCallback_h

#include <gmsec/util/wdllexp.h>
#include <gmsec/ErrorCallback.h>

namespace gmsec
{

class Connection;
class Message;


/** @class ReplyCallback
 * @brief This class is the abstract base class for received replies from asynchronous request messages.
 * A user created class, derrived from this class, can be passed into
 * %Request() to have user code executed asynchronously when a relpy is received or when an error occurs.
 *
 * Example Callback class:
 * @code
 * class ReqReplyCallback : public ReplyCallback
 * {
 *	public:
 *	virtual void CALL_TYPE OnReply(Connection *conn, Message *request, Message *reply)
 *	{
 *		const char *tmp;
 *		request->ToXML(tmp);
 *		cout << tmp << endl;
 *		reply->ToXML(tmp);
 *		cout << tmp << endl;
 *	}
 *  virtual void CALL_TYPE OnError(Connection *conn, Message *request, Status *result)
 *  {
 *		cout << result->Get() << endl;
 *  }
 * };
 * @endcode
 *
 * Example ReplyCallback registration:
 * @code
 * ReqReplyCallback cb;
 * Message *request = NULL;
 *  // TBD: create request here
 *
 * result = conn->Request(request,timeout,&cb);
 * if( result.isError() )
 *	// handle error
 * @endcode
 *
 * @sa Connection::Request(Message *request, GMSEC_I32 timeout, ReplyCallback *cb)
*/
class GMSEC_API ReplyCallback : public gmsec::ErrorCallback
{
public:
	/** @fn OnReply(Connection *conn, Message *request, Message *reply)
	 * @brief This function is called by the API in response to a reply recieved from a request,
	 * from within the Request call. A class derrived from gmsec::ReplyCallback needs to be
	 * passed into the Request() call.
	 *
	 *   The prototype for this funtion is:
	 *   @code virtual void CALL_TYPE OnReply(Connection *conn, Message *request, Message *reply) @endcode
	 *   The CALL_TYPE macro is required and is there to handle a compatibility
	 *   problem with the Windows linker.
	 *
	 * @note <b>DO NOT DESTROY</b> the Connection, or Message that is passed into this function by the API.
	 * They are owned by the API and do not need to be managed by the client program. Also, they can
	 * not be stored by the client program beyond the scope of this callback function. In order to store
	 * the gmsec::Message, it must be cloned with Connection::CloneMessage().
	 *
	 * @param conn - connection on which the message was recieved
	 * @param request - the pending request
	 * @param reply - the recieved reply
	 *
	 * @sa Connection::Request(Message *request, GMSEC_I32 timeout, ReplyCallback *cb)
	 */
	virtual void CALL_TYPE OnReply(Connection *conn, Message *request, Message *reply) = 0;

};

} // namespace gmsec

#endif /* gmsec_ReplyCallback_h */
