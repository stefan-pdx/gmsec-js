
/* Copyright 2007-2010 United States Government as represented by the     */
/* Administrator of The National Aeronautics and Space Administration. All */
/* Rights Reserved.                                                        */


/** @file OldConnection.h
 *
 *  @brief This file contains the old request / reply definition until
 * the SmartSockets wrapper can be updated or phased out.
 *
**/

#ifndef gmsec_internal_OldConnection_h
#define gmsec_internal_OldConnection_h

#include <string>
#include <list>

#include <gmsec/Status.h>
#include <gmsec/internal/BaseConnection.h>
#include <gmsec/internal/RequestDispatcher.h>


namespace gmsec
{

class Callback;
class Connection;
class ErrorCallback;
class Message;
class ReplyCallback;

namespace internal
{

class RequestDispatcher;



/** @class OldConnection
 * @brief This BaseConnection subclass can be inherited from to get the
 * old request / reply behavior.  Namely, a RequestDispatcher thread is
 * created for every request with callback, and requests without callback
 * block the connection.
 *
*/
class GMSEC_API OldConnection : public BaseConnection
{
public:

	/** @fn OldConnection(Config *config)
	 * @brief Construct a connection based upon paramaters set in the config object.
	 *
	 * @note This is never called by the client application, but is used by the API
	 * to handle configuration parameters that are common to all OldConnection middleware
	 * implementations.
	 */
	OldConnection(Config *config);

	/** @fn ~OldConnection()
	 * @brief Destructor.
	 */
	virtual ~OldConnection();


	/** @fn Request(Message *request, GMSEC_I32 timeout, Callback *cb, GMSEC_I32 republish_ms)
	 * @brief This function will send a request asyncronously. The callback will be called for the reply
	 * if it is received within the specified timeout. This function will not block.
	 * The timeout value is expressed in milliseconds.
	 *
	 * <B>IMPORTANT:</B> The request message passed into this function will be cleaned up when the processing
	 * is complete, therefore it must be created new and not stored or cleaned up by the client program.
	 *
	 * <B>NOTE:</B> This version, and the Callback class is DEPRECATED. Please use the ReplyCallback
	 * version of this function.
	 *
	 * @param request - message to be sent
	 * @param timeout - maximum time to wait for reply (in milliseconds)
	 * @param cb - Callback to call when reply is receieved
	 * @return status - result of the request operation
	 */
	virtual Status CALL_TYPE Request(Message *request, GMSEC_I32 timeout, Callback *cb, GMSEC_I32 republish_ms);

	/** @fn Request(Message *request, GMSEC_I32 timeout, ReplyCallback *cb, GMSEC_I32 republish_ms)
	 * @brief This function will send a request asyncronously. The callback will be called for the reply
	 * if it is received within the specified timeout. This function will not block.
	 * The timeout value is expressed in milliseconds.
	 *
	 * <B>IMPORTANT:</B> The request message passed into this function will be cleaned up when the processing
	 * is complete, therefore it must be created new and not stored or cleaned up by the client program.
	 *
	 * @param request - message to be sent
	 * @param timeout - maximum time to wait for reply (in milliseconds)
	 * @param cb - Callback to call when reply is receieved
	 * @return status - result of the request operation
	 */
	virtual Status CALL_TYPE Request(Message *request, GMSEC_I32 timeout, ReplyCallback *cb, GMSEC_I32 republish_ms);

	/** @fn Request(Message *request, GMSEC_I32 timeout, Message *&reply, GMSEC_I32 republish_ms)
	 * @brief This function will send a request, wait for the specified timeout, and return the received reply
	 * This function will block until the reply is received or the timeout is reached.
	 * The timeout value is expressed in milliseconds.
	 *
	 * @param request - message to be sent
	 * @param timeout - maximum time to wait for reply (in milliseconds)
	 * @param reply - out parameter reply message if received
	 * @return status - result of the request operation
	 */
	virtual Status CALL_TYPE Request(Message *request, GMSEC_I32 timeout, Message *&reply, GMSEC_I32 republish_ms) = 0;

	/** @fn Reply(Message *request,Message *reply)
	 * @brief This function will send a reply to a given request.
	 *
	 * @param request - the recieved request that we are responding to
	 * @param reply - the reply to be sent
	 * @return status - result of the reply operation
	 */
	virtual Status CALL_TYPE Reply(Message *request, Message *reply) = 0;

	/** @fn sendRequest (Message *request, string &id)
	 * @brief Send the request with a unique ID.
	* The OldConnection class provides a dummy implementation which will
	* never be called.
	 *
	 * @param request - the request to send
	 * @param id - reference through which to return the request's unique ID
	 * @return status - result of the operation
	 */
	virtual Status CALL_TYPE sendRequest(Message *request, std::string &id);

protected:

private:

	/* Handles the list of Request Dispatchers */
	Status DestroyRequestDispatchers();
	void PruneRequestList();


	/* Request dispatcher management */
	std::list<RequestDispatcher *> fRequestDispatchers;

};


} // namespace internal
} // namespace gmsec

#endif  // gmsec_internal_OldConnection_h

