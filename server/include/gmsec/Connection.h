
/* Copyright 2007-2010 United States Government as represented by the     */
/* Administrator of The National Aeronautics and Space Administration. All */
/* Rights Reserved.                                                        */


/** @file Connection.h
 *
 *  @brief This file contains the opaque wrapper for middleware connections to
 * be created by the gmsec::ConnectionFactory.
 *
 *
**/

#ifndef gmsec_Connection_h
#define gmsec_Connection_h

#include <gmsec/util/wdllexp.h>
#include <gmsec/Status.h>
#include <gmsec/Message.h>
#include <gmsec/ConnectionFactory.h>


namespace gmsec
{

class Callback;
class ErrorCallback;
class ReplyCallback;

namespace internal
{
class BaseConnection;
class ConnectionBuddy;
}


/** @class Connection
 * @brief This is the public interface for middleware connections. Each middleware connection
 * implements a class derived from Connection to abstract middleware specific connection details.
 * The application will never access the middleware connection directly, but only through the Connection "interface".
 *
 * The connection class provides services available on an implemented GMSEC connection. An application
 * can create multiple connection objects with different parameters and different middleware.
 * Creating multiple connections to the same middleware is not prevented but may not be supported
 * by all middleware implementations.
 *
 * Example creation and use:
 * @code
 * Connection *conn = NULL;
 *
 * //Create config from command line arguments
 * Config cfg(argc,argv);
 *
 * //Create the Connection
 * result = ConnectionFactory::Create(&cfg,conn);
 * if( result.isError() )
 *	//handle error
 *
 * //Establish the connection
 * result = conn->Connect();
 * if( result.isError() )
 *	//handle error
 * @endcode
 *
 * @sa ConnectionFactory @n
 *		Config
*/
class GMSEC_API Connection
{
public:


	/** @fn Connect()
	 * @brief This function establishes this connection to the middleware
	 *
	 * @return status - result of the connection operation
	 */
	Status CALL_TYPE Connect();

	/** @fn Disconnect()
	 * @brief This function terminates this connection to the middleware.
	 * It is automatically called by the destructor if necessary
	 *
	 * @return status - result of the connection operation
	 */
	Status CALL_TYPE Disconnect();

	/** @fn IsConnected()
	 * @brief This function returns t/f whether the connection has
	 * been established
	 *
	 * @return true if connected
	 */
	bool CALL_TYPE IsConnected();

	/** @fn GetLibraryRootName()
	 * @brief This function identifies the root library name and therefore the
	 * connection type that this connection is associated with. For example,
	 * the root library name for the icsswb middleware library is "gmsec_icsswb"
	 * and matches the name of the windows library (gmsec_icsswb.dll) and the
	 * UNIX library (libgmsec_icsswb.so).
	 *
	 * @return root library name
	 *
	 * @sa Message::GetLibraryRootName()
	 */
	const char * CALL_TYPE GetLibraryRootName();

	/** @fn GetLibraryVersion()
	 * @brief This function returns a string containing the version information for
	 * this connection's associated middleware.
	 *
	 * @return library version
	 */
	const char * CALL_TYPE GetLibraryVersion();

	/** @fn RegisterErrorCallback(const char *event, ErrorCallback *cb)
	 * @brief This function allows the registration of a callback for a particular
	 * error event. Event names are middleware dependent.
	 *
	 * All connection types implement at least following error events:
	 *
	 * "CONNECTION_DISPATCHER_ERROR" - Auto-dispatcher error callback that gets called
	 *		whenever an error occurs inside the auto-dispatcher.
	 *
	 * "CONNECTION_REQUEST_TIMEOUT" - Request w/callback error callback that gets called
	 *		whenever an error occurs while trying to process an request (ex. timeout)
	 *
	 * @param event - name of event to register
	 * @param cb - object derrived from ErrorCallback to register for this error event
	 * @return status - result of the connection operation
	 *
	 * @sa ErrorCallback
	 */
	Status CALL_TYPE RegisterErrorCallback(const char *event, ErrorCallback *cb);

	/** @fn Subscribe(const char *subject)
	 * @brief This function subscribes to a particular subject or pattern. This
	 * causes middleware routing of messages that match that subject or pattern
	 * be queued for this process. Messages that are subscribed to without callback
	 * need to be pulled from the queue using GetNextMsg() and are thrown away if
	 * the auto-dispatcher is used.
	 *
	 * Example subscription patterns:
	 * @code
	 * // this will match only messages with this exact subject
	 * conn->Subscribe("gmsec.mission.const.sat.evt.msg");
	 *
	 * // this will match messages with any mission
	 * conn->Subscribe("gmsec.*.const.sat.evt.msg");
	 *
	 * // this will match messages that have AT LEAST ONE MORE TAG
	 * //	(will not match "gmsec.mission.const.sat")
	 * conn->Subscribe("gmsec.mission.const.sat.>");
	 *
	 * // this will match any event message
	 * conn->Subscribe("gmsec.*.*.*.evt.>");
	 * @endcode
	 *
	 * @note Although subscription behavior is outlines as above, the actual behavior for a particular
	 * middleware implementation MAY deviate from this behavior slightly.
	 *
	 * @param subject - subject pattern to match received messages
	 * @return status - result of the connection operation
	 *
	 * @sa Connection::GetNextMsg(Message *&msg, GMSEC_I32 timeout) @n
	 *     Connection::StartAutoDispatch() @n
	 *     Connection::StopAutoDispatch()
	 */
	Status CALL_TYPE Subscribe(const char *subject);

	/** @fn Subscribe(const char *subject, Callback *cb)
	 * @brief This function subscribes to a particular subject or pattern and
	 * associates a callback to be called when messages matching the subject
	 * or pattern are received. If all subscriptions are performed using this
	 * function then the auto-dispatcher can be used to asynchronously receive
	 * messages. If GetNextMsg() is used to pull messages then DispatchMsg()
	 * will need to be called to ensure registered Callbacks are called.
	 *
	 * <b>see Connection::Subscribe(const char *subject) for an explaination of subscription patterns</b>
	 *
	 * @param subject - subject pattern to match received messages
	 * @param cb - callback to be called when message is received
	 * @return status - result of the connection operation
	 *
	 * @sa Connection::Subscribe(const char *subject) @n
	 *	   Connection::GetNextMsg(Message *&msg, GMSEC_I32 timeout) @n
	 *     Connection::DispatchMsg(Message *msg) @n
	 *     Connection::StartAutoDispatch() @n
	 *     Connection::StopAutoDispatch()
	 */
	Status CALL_TYPE Subscribe(const char *subject, Callback *cb);

	/** @fn UnSubscribe(const char *subject)
	 * @brief This function unsubscribes to a particular subject pattern, and will stop the reception
	 * of messages that match this pattern. It will also remove the registration of any callbacks with
	 * this subject pattern.
	 *
	 * @param subject - subject pattern that was used to match received messages
	 * @return status - result of the connection operation
	 *
	 * @sa Connection::Subscribe(const char *subject) @n
	 *	   Connection::GetNextMsg(Message *&msg, GMSEC_I32 timeout) @n
	 *     Connection::DispatchMsg(Message *msg) @n
	 *     Connection::StartAutoDispatch() @n
	 *     Connection::StopAutoDispatch()
	 */
	Status CALL_TYPE UnSubscribe(const char *subject);
	/** @fn UnSubscribe(const char *subject, Callback *cb)
	 * @brief This function unsubscribes a single callback to a particular subject pattern, and
	 * will not unsubscribe the reception of the message. It will prevent a particular callback
	 * from being called by the auto-dispatch or DispatchMsg(), but the message will still be
	 * received for GetNextMsg().
	 *
	 * @param subject - subject pattern to match received messages
	 * @param cb - callback to be called when message is received
	 * @return status - result of the connection operation
	 *
	 * @sa Connection::Subscribe(const char *subject) @n
	 *	   Connection::GetNextMsg(Message *&msg, GMSEC_I32 timeout) @n
	 *     Connection::DispatchMsg(Message *msg) @n
	 *     Connection::StartAutoDispatch() @n
	 *     Connection::StopAutoDispatch()
	 */
	Status CALL_TYPE UnSubscribe(const char *subject, Callback *cb);


	/** @fn CreateMessage( Message *&msg )
	 * @brief This function creates a message for this particular middleware connection.
	 * The kind & subject are set to defaults dependent upon the particular middleware implementation.
	 *
	 * @param msg - Message pointer to be filled by created message
	 * @return status - result of the connection operation
	 *
	 */
	Status CALL_TYPE CreateMessage(Message *&msg);

	/** @fn CreateMessage(const char *subject, GMSEC_MSG_KIND msgKind, Message *&msg)
	 * @brief This function creates a message for this particular middleware connection.
	 * The subject name for this call MUST be a valid subject name and NOT a pattern.
	 *
	 * @param subject - subject under which this message will eventually be published/requested
	 * @param msgKind - indentifier for the intended message kind
	 * @param msg - Message pointer to be filled by created message
	 * @return status - result of the connection operation
	 *
	 */
	Status CALL_TYPE CreateMessage(const char *subject, GMSEC_MSG_KIND msgKind, Message *&msg);

	/** @fn CreateMessage(const char *subject, GMSEC_MSG_KIND msgKind, Message *&msg, Config *config)
	  * @brief The Config object can be used to set Subject, Kind or middleware specific settings.
	 * The options are specific to middleware implementation and are listed in the <i>GMSEC User's Guide</i>
	 *
	 * @param subject - subject under which this message will eventually be published/requested
	 * @param msgKind - indentifier for the intended message kind
	 * @param msg - Message pointer to be filled by created message
	 * @param config - Config object used to pass parameters to the middleware specific message implementation
	 * @return status - result of the connection operation
	 *
	 * @sa Connection::CreateMessage(const char *subject, GMSEC_MSG_KIND msgKind, Message *&msg)
	 */
	Status CALL_TYPE CreateMessage(const char *subject, GMSEC_MSG_KIND msgKind, Message *&msg, Config *config);

	/** @fn ConvertMessage( Message *in, Message *&out )
	 * @brief This function will call "CloneMessage" to copy a message to this connection
	 * from another connection, only if necessary. This is used by the API, or a client program
	 * to publish a message that was created or recieved on on middleware to another.
	 *
	 * @note ConvertCleanup() needs to be called with the same parameters, after the message is no
	 * longer needed, to ensure any memory required is cleaned up.
	 *
	 * @param in - message to convert FROM
	 * @param out - out parameter filled with the coverted message (could be the same if conversion wasn't necessary)
	 * @return status - result of the connection operation
	 *
	 * @sa Connection::ConvertCleanup( Message *in, Message *out )
	 */
	Status CALL_TYPE ConvertMessage(Message *in, Message *&out);

	/** @fn ConvertCleanup( Message *in, Message *out )
	 * @brief This function cleans up any memory allocated by ConvertMessage() when
	 * finished with the associated message. The parameters need to be
	 * exactly the same as those originally passed to ConvertMessage().
	 *
	 * @param in - message that was originally coverted FROM
	 * @param out - message that was output by the ConvertMessage() call
	 * @return status - result of the connection operation
	 *
	 * @sa Connection::ConvertMessage( Message *in, Message *&out )
	 */
	Status CALL_TYPE ConvertCleanup(Message *in, Message *out);

	/** @fn CloneMessage( Message *in, Message *&out )
	 * @brief This function copies a message without knowing what type it is. This
	 * function can be used to copy a message's contents.
	 *
	 * @note The 'out' message MUST BE CLEANED UP with DestroyMessage() by the client application.
	 *
	 * @param in - message to be cloned
	 * @param out - cloned message
	 * @return status - result of the connection operation
	 *
	 * @sa Connection::DestroyMessage(Message *msg)
	 */
	Status CALL_TYPE CloneMessage(Message *in, Message *&out);

	/** @fn DestroyMessage(Message *msg)
	 * @brief This function destroys a message and cleans up any associated memory.
	 *
	 * @param msg - message to be destroyed
	 * @return status - result of the connection operation
	 */
	Status CALL_TYPE DestroyMessage(Message *msg);

	/** @fn Publish(Message *msg)
	 * @brief This function will publish a message to the middleware.
	 *
	 * @param msg - message to be published
	 * @return status - result of the connection operation
	 */
	Status CALL_TYPE Publish(Message *msg);

	/** @fn Request(Message *request, GMSEC_I32 timeout, Callback *cb, GMSEC_I32 republish_ms = 0)
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
	Status CALL_TYPE Request(Message *request, GMSEC_I32 timeout, Callback *cb, GMSEC_I32 republish_ms = 0);

	/** @fn Request(Message *request, GMSEC_I32 timeout, ReplyCallback *cb, GMSEC_I32 republish_ms = 0)
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
	Status CALL_TYPE Request(Message *request, GMSEC_I32 timeout, ReplyCallback *cb, GMSEC_I32 republish_ms = 0);

	/** @fn Request(Message *request, GMSEC_I32 timeout, Message *&reply, GMSEC_I32 republish_ms = 0)
	 * @brief This function will send a request, wait for the specified timeout, and return the received reply
	 * This function will block until the reply is received or the timeout is reached.
	 * The timeout value is expressed in milliseconds.
	 *
	 * @param request - message to be sent
	 * @param timeout - maximum time to wait for reply (in milliseconds)
	 * @param reply - out parameter reply message if received
	 * @return status - result of the request operation
	 */
	Status CALL_TYPE Request(Message *request, GMSEC_I32 timeout, Message *&reply, GMSEC_I32 republish_ms = 0);

	/** @fn Reply(Message *request,Message *reply)
	 * @brief This function will send a reply to a given request.
	 *
	 * @param request - the recieved request that we are responding to
	 * @param reply - the reply to be sent
	 * @return status - result of the reply operation
	 */
	Status CALL_TYPE Reply(Message *request,Message *reply);

	/** @fn StartAutoDispatch()
	 * @brief This function will start a thread that will dispatch messages asynchronously when they are received.
	 * If this is used, all subscriptions must be made with callbacks or the messages with be dropped.
	 * If GetNextMessage() is called while the auto-dispatcher is used, the behavior will be undesireable
	 * and undefined.
	 *
	 * @return status - result of the start operation
	 *
	 * @sa Connection::Subscribe(const char *subject, Callback *cb)
	 */
	Status CALL_TYPE StartAutoDispatch();

	/** @fn StopAutoDispatch()
	 * @brief This function will stop the auto dispatch thread.
	 *
	 * @return status - result of the stop operation
	 */
	Status CALL_TYPE StopAutoDispatch();
	Status CALL_TYPE StopAutoDispatch(bool waitForComplete);

	/** @fn GetNextMsg(Message *&msg, GMSEC_I32 timeout)
	 * @brief This function returns the next message received within the specified timeout.
	 * The received messages are determined by the %Subscribe() function(s), but
	 * %DispatchMsg() needs to be called messages received from this function to
	 * ensure all registered callbacks are executed. @n
	 * @n
	 * This function <b>MUST NOT BE USED</b> if the auto-dispatcher is being used.
	 *
	 * @param msg - out parameter, the next received message, if any
	 * @param timeout - the maximum time to block waiting for a message, in milliseconds
	 * @return status - result of the operation
	 *
	 * @sa Connection::Subscribe(const char *subject) @n
	 *     Connection::Subscribe(const char *subject, Callback *cb) @n
	 *     Connection::DispatchMsg(Message *msg) @n
	 *     Connection::StartAutoDispatch() @n
	 *     Connection::StopAutoDispatch()
	 */
	Status CALL_TYPE GetNextMsg(Message *&msg, GMSEC_I32 timeout);

	/** @fn DispatchMsg(Message *msg)
	 * @brief This function will cause the any callbacks that are registered with matching
	 * patterns to be called.
	 *
	 * @param msg - message to be dispatched
	 * @return status - result of the operation
	 *
	 * @sa Connection::GetNextMsg(Message *&msg, GMSEC_I32 timeout)
	 */
	Status CALL_TYPE DispatchMsg(Message *msg);

	/** @fn GetLastDispatcherStatus()
	 * @brief When running with the auto-dispatcher, it may be necessary to monitor the
	 * status as it runs within a seperate thread.  This method allows access to the
	 * last status error reported by the dispatcher.  Once the status is read, the
	 * status is cleared.
	 *
	 * @note Another way to be notified in the case of a dispatcher error is to register an
	 * error callback with the RegisterErrorCallback() function.
	 *
	 * @sa Connection::StartAutoDispatch() @n
	 *     Connection::StopAutoDispatch() @n
	 *	   Connection::RegisterErrorCallack()
	 */
	Status CALL_TYPE GetLastDispatcherStatus();


	/** @fn GetName()
	 * @brief Get the logical name of this connection, if one has been assigned.
	 *	This is usefull for identifying connections within a client program.
	 *
	 * @return name of this connection
	 */
	const char * CALL_TYPE GetName();

	/** @fn SetName( const char *name )
	 * @brief Set the logical name of this connection. This can be usedull for
	 * Identifying connections withing a client program.
	 *
	 * @param name - name of this connection
	 */
	void CALL_TYPE SetName(const char *name);

	/** @fn GetConnectionGUI()
	 * @brief Get the string GUID for this connection.
	 *
	 */
	const char *GetConnectionGUI();

	/* helper to dispatch errors */
	Status DispatchError(const char *name, Message *msg, Status *status);

	/*
	** these are for C API support ONLY
	*/
	Status CALL_TYPE RegisterErrorCallback(const char *event, GMSEC_ERROR_CALLBACK *cb);
	Status CALL_TYPE Subscribe(const char *subject, GMSEC_CALLBACK *cb);
	Status CALL_TYPE UnSubscribe(const char *subject, GMSEC_CALLBACK *cb);
	Status CALL_TYPE Request(Message *request, GMSEC_I32 timeout, GMSEC_CALLBACK *cb);
	Status CALL_TYPE Request(Message *request, GMSEC_I32 timeout, GMSEC_REPLY_CALLBACK *cb, GMSEC_ERROR_CALLBACK *er);


#ifdef GMSEC_USE_ENUM_FOR_CONSTANTS
	enum
	{
		MWINFO_BUFFER = 256
	};
#else
	static const int MWINFO_BUFFER = 256;
#endif

	/** @fn GetMWINFO(char *)
	 *  @brief Thread Safe alternative method for const char * GetMWINFO().
	 *
	 *  @param infoBuffer - user allocated buffer to store the mw information
	 * infoBuffer must be at least MWINFO_BUFFER characters
	 *  @return char pointer to infoBuffer
	 */
	char * CALL_TYPE GetMWINFO(char * infoBuffer);


private:

	/** @fn Connection ()
	 * @brief Default constructor.
	 * Constructs an interface object with no backing connection.
	 */
	Connection();

	/** @fn Connection(Config *config)
	 * @brief Construct a connection based upon paramaters set in the config object.
	 *
	 * @note This is never called by the client application, but is used by the API
	 * to handle configuration parameters that are common to all Connection middleware
	 * implementations.
	 *
	 * @sa ConnectionFactory::Create(Config *cfg, Connection *&conn)
	 */
	Connection(Config *config);

	/** @fn ~Connection()
	 * @brief base class deconstructor
	 */
	~Connection();


	friend class internal::ConnectionBuddy;
	internal::BaseConnection *ptr;

};


} //namespace gmsec

#endif /* gmsec_Connection_h */

