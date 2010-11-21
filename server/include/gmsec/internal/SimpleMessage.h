
/* Copyright 2007-2010 United States Government as represented by the     */
/* Administrator of The National Aeronautics and Space Administration. All */
/* Rights Reserved.                                                        */


/** @file SimpleMessage.h
 *  @brief This file contains the base class for GMSEC middleware messages.
 *
 *
**/


#ifndef gmsec_internal_SimpleMessage_h
#define gmsec_internal_SimpleMessage_h

#include <map>

#include <gmsec_defs.h>
#include <gmsec/Status.h>
#include <gmsec/internal/BaseMessage.h>
#include <gmsec/internal/BaseField.h>
#include <gmsec/internal/BaseConfig.h>
#include <gmsec/internal/BaseConfigFile.h>
#include <gmsec/internal/TrackingDetails.h>


namespace gmsec
{

namespace internal
{

/** @class SimpleMessage
 * @brief The SimpleMessage class provides a simple concrete implementation
 * of messages.
 *
 * @sa gmsec::internal::BaseMessage
 * @sa gmsec::Message
*/

class GMSEC_API SimpleMessage : public BaseMessage
{
private:

	std::string fSubject;
	GMSEC_MSG_KIND fKind;

	typedef std::map<std::string, BaseField *> FieldMap;
	FieldMap fFields;
	FieldMap::iterator fFieldItr;

	TrackingDetails fTracking;
	const char *fXmlOutput;


protected:

	/** @fn ProcessConfigValue(const char *name, const char *value)
	 * @brief This function is used to process values include in the
	 * message config.
	 *
	 * @param name - parameter name
	 * @param value - parameter value
	 * @return bool - true if value is used, otherwise false
	 *
	 * @sa SetConfig()
	 */
	virtual bool CALL_TYPE ProcessConfigValue(const char *name, const char *value);


public:

	/** @fn SimpleMessage()
	 * @brief Default constructor - initializes the message instance
	 *
	 */
	SimpleMessage();

	/** @fn ~SimpleMessage()
	 * @brief Destructor - cleans up the message instance
	 */
	virtual ~SimpleMessage();


	virtual TrackingDetails * CALL_TYPE getTracking();
	virtual void CALL_TYPE setTracking(TrackingDetails *details);


	/** @fn isValid()
	 * @brief This function will check if the message is Valid.
	 *
	 * @return true if the message is valid
	 *
	 */
	virtual bool CALL_TYPE isValid();

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
	virtual const char * CALL_TYPE GetLibraryRootName();

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
	virtual Status CALL_TYPE SetKind(GMSEC_MSG_KIND kind);

	/** @fn GetKind(GMSEC_MSG_KIND &kind)
	 * @brief This function get the 'kind' of this message.
	 *
	 * @param kind - out parameter, current message kind
	 * @return status - result of the operation
	 */
	virtual Status CALL_TYPE GetKind(GMSEC_MSG_KIND &kind);

	/** @fn SetSubject(const char *subject)
	 * @brief This function set the subject to publish/request this message upon.
	 *
	 * @param subject - valid message subject name
	 * @return status - result of the operation
	 */
	virtual Status CALL_TYPE SetSubject(const char *subject);

	/** @fn GetSubject(const char *&subject)
	 * @brief This function get the subject this message will be/was published upon.
	 *
	 * @param subject - out parameter, subject of the message
	 * @return status - result of the operation
	 */
	virtual Status CALL_TYPE GetSubject(const char *&subject);

	/** @fn ClearFields()
	 * @brief This function will remove all fields form this message.
	 *
	 * @note <b>This may not be available in all middleware implementations.</b>
	 *
	 * @return status - result of the operation
	 */
	virtual Status CALL_TYPE ClearFields();

	/** @fn AddField(BaseField &field)
	 * @brief This function will add a new field to this message. A new field will be added to the message
	 * that matches the field passed in. Therefore the field is owned by the client program and can be reused.
	 * The client program must also clean up that field when no longer needed.
	 *
	  * Example:
	  * @code
	 * Status result;
	  * BaseField fd;
	  *
	 * @param field - field object to copy info from
	 * @return status - result of the operation
	 */
	virtual Status CALL_TYPE AddField(Field &field);
	virtual Status CALL_TYPE addField(BaseField &field);

	/** @fn ClearField(const char *name)
	 * @brief This function will remove a field from this message by name.
	 *
	 * @note <b>This may not be available in all middleware implementations.</b>
	 *
	 * @param name - name of field to remove
	 * @return status - result of the operation
	 */
	virtual Status CALL_TYPE ClearField(const char *name);

	/** @fn GetField(const char *name, Field &field)
	 * @brief This function will copy a field within a message into a field created by the client program, by name.
	  *
	 * @param name - name of field to get
	 * @param field - out parameter, field object to copy info into
	 * @return status - result of the operation
	 * @sa gmsec::Message::GetField()
	 */
	virtual Status CALL_TYPE GetField(const char *name, Field &field);
	virtual Status CALL_TYPE getField(const char *name, BaseField &field);

	/** @fn GetFieldCount(GMSEC_I32 &count)
	 * @brief This function gets the number of fields contained in this message.
	 *
	 * @param count - out parameter, number of fields in this message
	 * @return status - result of the operation
	 */
	virtual Status CALL_TYPE GetFieldCount(GMSEC_I32 &count);

	/** @fn GetFirstField(Field &field)
	 * @brief This function and GetNextField() are used to iterate through the fields of a message.
	  *
	 * @param field - field object to copy field into
	 * @return status - result of the operation
	 *
	 * @sa gmsec::Message::GetFirstField()
	 * @sa GetNextField()
	 */
	virtual Status CALL_TYPE GetFirstField(Field &field);
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
	virtual Status CALL_TYPE GetNextField(Field &field);
	virtual Status CALL_TYPE getNextField(BaseField &field);

	/** @fn CopyFields(BaseMessage *toMsg)
	 * @brief This function will copy all fields from this message to a new message in the fastest way possible
	 *
	 * @param toMsg - the message to copy fields into
	 * @return status - result of the operation
	 */
	virtual Status CALL_TYPE CopyFields(Message *toMsg);
	virtual Status CALL_TYPE copyFields(BaseMessage *toMsg);


	/** @fn GetMSGSize( GMSEC_U32 &size )
	 * @brief This function will return the physical storage size of the message in bytes.
	 *
	 * @note <b>This may not be available in all middleware implementations.</b>
	 *
	 * @param size - out parameter, size of message in bytes
	 * @return status - result of the operation
	 */
	virtual Status CALL_TYPE GetMSGSize(GMSEC_U32 &size);

	virtual Status CALL_TYPE ToXML(const char *&xml);


	// static BaseMessage *get (Message *msg);

};

} //namespace internal
} //namespace gmsec

#endif	// gmsec_internal_SimpleMessage_h
