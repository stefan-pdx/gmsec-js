
/* Copyright 2007-2010 United States Government as represented by the     */
/* Administrator of The National Aeronautics and Space Administration. All */
/* Rights Reserved.                                                        */


/** @file BaseMessage.h
 *  @brief This file contains the base class for GMSEC middleware messages.
 *
 *
**/


#ifndef gmsec_internal_BaseMessage_h
#define gmsec_internal_BaseMessage_h

#include <gmsec_defs.h>
#include <gmsec/Status.h>
#include <gmsec/Message.h>
#include <gmsec/internal/BaseField.h>
#include <gmsec/internal/BaseConfig.h>
#include <gmsec/internal/BaseConfigFile.h>
#include <gmsec/internal/TrackingDetails.h>


// forward declare so we don't have to include libxml
typedef struct _xmlNode xmlNode;

namespace gmsec
{

class Field;
class Callback; // for deliver
class Connection; // for deliver

namespace internal
{

/** @class BaseMessage
 * @brief The BaseMessage object is a container for GMSEC Messages.  The methods of this class
 * allow the construction and manipulation of the data in a message.
 *
 * @sa gmsec::Message
*/

class GMSEC_API BaseMessage
{
private:
	Message *external;


public:

	/** @fn BaseMessage()
	 * @brief Default constructor - initializes the message instance
	 *
	 */
	BaseMessage();

	/** @fn ~BaseMessage()
	 * @brief Destructor - cleans up the message instance
	 */
	virtual ~BaseMessage();


	/** @fn createExternal ()
	 * Create a public interface Message object that owns this object.
	 */
	Message *createExternal();


	/** @fn getExternal ()
	 * Access the external object.
	 */
	Message *getExternal()
	{
		return external;
	}



	/* @fn ProcessConfigValue (const char *name, const char *value);
	 * @param name - parameter name
	 * @param value - parameter value
	 * @return bool - true if value is used, otherwise false
	 *
	 * @sa SetConfig()
	 */
	virtual bool CALL_TYPE ProcessConfigValue(const char *name, const char *value);



	virtual TrackingDetails * CALL_TYPE getTracking() = 0;
	virtual void CALL_TYPE setTracking(TrackingDetails *details) = 0;


	/** @fn isValid()
	 * @brief This function will check if the message is Valid.
	 *
	 * @return true if the message is valid
	 *
	 */
	virtual bool CALL_TYPE isValid() = 0;

	/** @fn GetLibraryRootName()
	 * @brief This function identifies the root library name and therefore the
	 * connection type that this message is associated with. This information
	 * must match the library root name of the connection in order to be sent
	 * on that connection.
	 *
	 * @return middleware implementation library root name
	 *
	 * @sa Connection::GetLibraryRootName()
	 */
	virtual const char * CALL_TYPE GetLibraryRootName() = 0;

	/** @fn SetConfig(Config *config)
	 * @brief This function sets the Configuration of this message.  This can be used to
	 * pass middleware specific items to the BaseMessage object.
	 *
	 * All messages implmentations understand the following config values:
	 *
	 * "subject"	-	sets the message subject
	 *
	 * "kind"		-	sets the message kind
	 *
	 * TRACKING-*		-	sets tracking parameters
	 *
	 * Configuration values from the passed in config will be copied or set within the message. Therefore
	 * the pased in config WILL NOT BE STORED and is the responsibility of the client program to clean up.
	 *
	 * @param config - configuration object that contains values to use
	 * @return status - result of the operation
	 */
	virtual Status CALL_TYPE SetConfig(Config *config);
	virtual Status CALL_TYPE setConfig(BaseConfig *config);

	/** @fn SetKind(GMSEC_MSG_KIND kind)
	 * @brief This function set the 'kind' of this message.
	 *
	 * @param kind - valid message kind
	 * @return status - result of the operation
	 */
	virtual Status CALL_TYPE SetKind(GMSEC_MSG_KIND kind) = 0;

	/** @fn GetKind(GMSEC_MSG_KIND &kind)
	 * @brief This function get the 'kind' of this message.
	 *
	 * @param kind - out parameter, current message kind
	 * @return status - result of the operation
	 */
	virtual Status CALL_TYPE GetKind(GMSEC_MSG_KIND &kind) = 0;

	/** @fn SetSubject(const char *subject)
	 * @brief This function set the subject to publish/request this message upon.
	 *
	 * @param subject - valid message subject name
	 * @return status - result of the operation
	 */
	virtual Status CALL_TYPE SetSubject(const char *subject) = 0;

	/** @fn GetSubject(const char *&subject)
	 * @brief This function get the subject this message will be/was published upon.
	 *
	 * @param subject - out parameter, subject of the message
	 * @return status - result of the operation
	 */
	virtual Status CALL_TYPE GetSubject(const char *&subject) = 0;

	/** @fn ClearFields()
	 * @brief This function will remove all fields form this message.
	 *
	 * @note <b>This may not be available in all middleware implementations.</b>
	 *
	 * @return status - result of the operation
	 */
	virtual Status CALL_TYPE ClearFields() = 0;

	/** @fn AddField(Field &field)
	 * @brief This function will add a new field to this message. A new field will be added to the message
	 * that matches the field passed in. Therefore the field is owned by the client program and can be reused.
	 * The client program must also clean up that field when no longer needed.
	 *
	  * Example:
	  * @code
	 * Status result;
	  * Field fd;
	  *
	 * @param field - field object to copy info from
	 * @return status - result of the operation
	 */
	virtual Status CALL_TYPE AddField(gmsec::Field &field) = 0;
	virtual Status CALL_TYPE addField(BaseField &field);

	/** @fn ClearField(const char *name)
	 * @brief This function will remove a field from this message by name.
	 *
	 * @note <b>This may not be available in all middleware implementations.</b>
	 *
	 * @param name - name of field to remove
	 * @return status - result of the operation
	 */
	virtual Status CALL_TYPE ClearField(const char *name) = 0;

	/** @fn GetField(const char *name, Field &field)
	 * @brief This function will copy a field within a message into a field created by the client program, by name.
	  *
	 * @param name - name of field to get
	 * @param field - out parameter, field object to copy info into
	 * @return status - result of the operation
	 * @sa gmsec::Message::GetField()
	 */
	virtual Status CALL_TYPE GetField(const char *name, Field &field) = 0;
	virtual Status CALL_TYPE getField(const char *name, BaseField &field);

	/** @fn GetFieldCount(GMSEC_I32 &count)
	 * @brief This function gets the number of fields contained in this message.
	 *
	 * @param count - out parameter, number of fields in this message
	 * @return status - result of the operation
	 */
	virtual Status CALL_TYPE GetFieldCount(GMSEC_I32 &count) = 0;

	/** @fn GetFirstField(Field &field)
	 * @brief This function and GetNextField() are used to iterate through the fields of a message.
	  *
	 * @param field - field object to copy field into
	 * @return status - result of the operation
	 *
	 * @sa gmsec::Message::GetFirstField()
	 * @sa GetNextField()
	 */
	virtual Status CALL_TYPE GetFirstField(Field &field) = 0;
	virtual Status CALL_TYPE getFirstField(BaseField &field);

	/** @fn GetNextField(Field &field)
	 * @brief This function and GetFirstField() are used to iterate through the fields of a message.
	 *
	 * @param field - field object to copy field into
	 * @return status - result of the operation
	 *
	 * @sa gmsec::Message::GetNextField()
	 * @sa GetFirstField()
	 */
	virtual Status CALL_TYPE GetNextField(Field &field) = 0;
	virtual Status CALL_TYPE getNextField(BaseField &field);

	/** @fn CopyFields(BaseMessage *toMsg)
	 * @brief This function will copy all fields from this message to a new message in the fastest way possible
	 *
	 * @param toMsg - the message to copy fields into
	 * @return status - result of the operation
	 */
	virtual Status CALL_TYPE CopyFields(Message *toMsg);
	virtual Status CALL_TYPE copyFields(BaseMessage *toMsg);

	/** @fn ToXML( const char *&xml )
	 * @brief This function will dump a message to an xml string. This format is compatible with
	  * the format used by FromXML() and ConfigFile::GetMessage().
	  *
	  * @param xml - out parameter, xml string
	 * @return status - result of the operation
	  *
	  * @sa ConfigFile::GetMessage()
	 */
	virtual Status CALL_TYPE ToXML(const char *&xml) = 0;
	virtual Status CALL_TYPE toXML(const char *&xml);

	/** @fn FromXML( const char *xml )
	 * @brief This function will set subject, kind, and fields into a message as specified by the passed in
	 * xml string. The format is compatible with what is used by ToXML() and ConfigFile::GetMessage();
	 *
	  * @param xml - xml string to parse
	 * @return status - result of the operation
	  *
	  * @sa ConfigFile::GetMessage()
	 */
	virtual Status CALL_TYPE FromXML(const char *xml);

	/** @fn FromXML( xmlNode *msg_element )
	 * helper function that parses a config xmlNode
	 *
	 * Since this is private, it will only be used
	 * by this class, or friends. (to be used by ConfigFile class)
	 */
	virtual Status CALL_TYPE FromXML(xmlNode *msg_element);

	/** @fn GetMSGSize( GMSEC_U32 &size )
	 * @brief This function will return the physical storage size of the message in bytes.
	 *
	 * @note <b>This may not be available in all middleware implementations.</b>
	 *
	 * @param size - out parameter, size of message in bytes
	 * @return status - result of the operation
	 */
	virtual Status CALL_TYPE GetMSGSize(GMSEC_U32 &size) = 0;

};


/**
* @class MessageBuddy
* @brief Provides exchange between internal and internal messages.
* Unfortunately, gcc 3.2.3 (maybe others) would not allow this to be a
* nested in BaseMessage and have its friend of Message privileges.
*/
class MessageBuddy
{
public:
	MessageBuddy(BaseMessage *internal)
	{
		message.ptr = internal;
	}

	~MessageBuddy()
	{
		message.ptr = NULL;
	}

	Message *ptr()
	{
		return &message;
	}

	Message &ref()
	{
		return message;
	}


	/** @fn createExternal (BaseMessage *internal)
	 * Create an external message associated with the given message.
	 */
	static Message *createExternal(BaseMessage *internal);

	/** @fn getInternal (Message *msg)
	 * Get the internal message associated with an public interface Message.
	 */
	static BaseMessage *getInternal(Message *external)
	{
		return external->ptr;
	}

	/** @fn destroy (Message *message)
	 * Call the Message destructor.
	 */
	static void destroy(Message *external)
	{
		delete external;
	}

private:
	Message message;
};


} //namespace internal
} //namespace gmsec

#endif	// gmsec_internal_BaseMessage_h
