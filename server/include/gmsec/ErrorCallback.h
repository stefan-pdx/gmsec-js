
/* Copyright 2007-2010 United States Government as represented by the     */
/* Administrator of The National Aeronautics and Space Administration. All */
/* Rights Reserved.                                                        */


/** @file ErrorCallback.h
 *
 *  @author Brian D Gregory
 *  @author Interface & Control Systems, Inc.
 *  @date April 24, 2003
 *
 *  @brief This file contains the abstract base class for error callbacks
 *
**/

#ifndef gmsec_ErrorCallback_h
#define gmsec_ErrorCallback_h

#include <gmsec/util/wdllexp.h>

namespace gmsec
{

class Connection;
class Message;
class Status;

/** @class ErrorCallback
 * @brief This class is the abstract base class for error callbacks.
 * A user created class, derrived from this class, can be passed into
 * %RegisterErrorCallback() to have user code executed asynchronously
 * when an error occurs in the connection object.
 *
 * Example callback class:
 * @code
 * class DumpErrorCallback : public ErrorCallback
 * {
 *	public:
 *	virtual void CALL_TYPE OnError(Connection *conn, Message *msg, Status *status, const char* event)
 *	{
 *		cout << status->Get() << endl;
 *	}
 * }
 * @endcode
 *
 * Example callback registration:
 * @code
 * DumpErrorCallback cb;
 * result = conn->RegisterErrorCallback("CONNECTION_REQUEST_TIMEOUT", &cb);
 * if( result.isError() )
 *	//handle error
 * @endcode
 *
 * @sa Connection::RegisterErrorCallback(const char *event, ErrorCallback *cb)
*/
class GMSEC_API ErrorCallback
{
public:

	// base class needs virtual destructor
	virtual ~ErrorCallback() { }

	/** @fn OnError(Connection *conn, Message *msg, Status *status, const char* event)
	 * This function is called in response to a error after a call to %RegisterErrorCallback().
	 *
	 *   The prototype for this funtion is:
	 *   @code virtual void CALL_TYPE OnError(Connection *conn, Message *msg, Status *status, const char* event) @endcode
	 *   The CALL_TYPE macro is required and is there to handle a compatibility
	 *   problem with the Windows linker.
	 *
	 * @note <b>DO NOT DESTROY</b> the Connection, Message, or Status that is passed into this function by the API.
	 * They are owned by the API and do not need to be managed by the client program. Also, they can
	 * not be stored by the client program beyond the scope of this callback function. In order to store
	 * the gmsec::Message, it must be cloned with Connection::CloneMessage().
	 *
	 * @note Message is not returned in all uses of the callback, if it is not used, msg will be set to NULL.
	 *
	 * @sa Connection::RegisterErrorCallback(const char *event, ErrorCallback *cb)
	 */
	virtual void CALL_TYPE OnError(Connection *conn, Message *msg, Status *status, const char* event) = 0;
};

} // namespace gmsec

#endif /* gmsec_ErrorCallback_h */
