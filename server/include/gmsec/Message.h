
/* Copyright 2007-2010 United States Government as represented by the     */
/* Administrator of The National Aeronautics and Space Administration. All */
/* Rights Reserved.                                                        */


/** @file Message.h
 *
 *  @author Brian D Gregory
 *  @author Interface & Control Systems, Inc.
 *  @date April 24, 2003
 *
 *  @brief This file contains the abstract base class for messages
 *
**/


#ifndef gmsec_Message_h
#define gmsec_Message_h

#include <gmsec_defs.h>
#include <gmsec/util/wdllexp.h>
#include <gmsec/Status.h>
#include <gmsec/Field.h>
#include <gmsec/Config.h>


namespace gmsec
{

namespace internal
{
class BaseMessage;
class MessageBuddy;
}

/** @class Message
 * @brief The Message object is a container for GMSEC Messages.  The methods of this class
 * allow the construction and manipulation of the data in a message.
 *
 * Example creation & use:
 * @code
 * Status result;
 * Message *msg = NULL;
 *
 * result = conn->CreateMessage("gmsec.test.publish",GMSEC_MSG_PUBLISH,msg);
 * if( result.isError() )
 *	//handle error
 *
 * // send message
 * result = conn->Publish(msg);
 * if( result.isError() )
 *	//handle error
 *
 * result = conn->DestroyMessage(msg);
 * if( result.isError() )
 *	//handle error
 * @endcode
 *
 * @sa Connection
 *     Field
*/

class GMSEC_API Message
{
public:

	/** @fn isValid()
	 * @brief This function will check if the message is Valid.
	 *
	 * @return true if the message is valid
	 *
	 */
	bool CALL_TYPE isValid();

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
	const char * CALL_TYPE GetLibraryRootName();

	/** @fn SetConfig(Config *config)
	 * @brief This function sets the Configuration of this message.  This can be used to
	 * pass middleware specific items to the Message object.
	 *
	 * All messages implmentations understand the following config values:
	 *
	 * "subject"	-	sets the message subject
	 *
	 * "kind"		-	sets the message kind
	 *
	 * Configuration values from the passed in config will be copied or set within the message. Therefore
	 * the pased in config WILL NOT BE STORED and is the responsibility of the client program to clean up.
	 *
	 * @param config - configuration object that contains values to use
	 * @return status - result of the operation
	 */
	Status CALL_TYPE SetConfig(Config *config);

	/** @fn SetKind(GMSEC_MSG_KIND kind)
	 * @brief This function set the 'kind' of this message.
	 *
	 * @param kind - valid message kind
	 * @return status - result of the operation
	 */
	Status CALL_TYPE SetKind(GMSEC_MSG_KIND kind);

	/** @fn GetKind(GMSEC_MSG_KIND &kind)
	 * @brief This function get the 'kind' of this message.
	 *
	 * @param kind - out parameter, current message kind
	 * @return status - result of the operation
	 */
	Status CALL_TYPE GetKind(GMSEC_MSG_KIND &kind);

	/** @fn SetSubject(const char *subject)
	 * @brief This function set the subject to publish/request this message upon.
	 *
	 * @param subject - valid message subject name
	 * @return status - result of the operation
	 */
	Status CALL_TYPE SetSubject(const char *subject);

	/** @fn GetSubject(const char *&subject)
	 * @brief This function get the subject this message will be/was published upon.
	 *
	 * @param subject - out parameter, subject of the message
	 * @return status - result of the operation
	 */
	Status CALL_TYPE GetSubject(const char *&subject);

	/** @fn ClearFields()
	 * @brief This function will remove all fields form this message.
	 *
	 * @note <b>This may not be available in all middleware implementations.</b>
	 *
	 * @return status - result of the operation
	 */
	Status CALL_TYPE ClearFields();

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
	  * //set field name & value
	  * result = fd.SetName("char_field");
	  * if( result.isError() )
	  *	//handle error
	  * result fd.SetValue((GMSEC_CHAR)'c');
	  * if( result.isError() )
	  *	//handle error
	  *
	  * //add to message (will copy)
	  * result = msg->AddField(fd);
	  * if( result.isError() )
	  *	//handle error
	  * @endcode
	 *
	 * @param field - field object to copy info from
	 * @return status - result of the operation
	 */
	Status CALL_TYPE AddField(Field &field);

	/** @fn ClearField(const char *name)
	 * @brief This function will remove a field from this message by name.
	 *
	 * @note <b>This may not be available in all middleware implementations.</b>
	 *
	 * @param name - name of field to remove
	 * @return status - result of the operation
	 */
	Status CALL_TYPE ClearField(const char *name);

	/** @fn GetField(const char *name, Field &field)
	 * @brief This function will copy a field within a message into a field created by the client program, by name.
	  *
	  * Example:
	  * @code
	  * GMSEC_STRING value = NULL;
	  * Field fd;
	  *
	  * result = msg->GetField("char_field",fd);
	  * if( result.isError() )
	  *	//handle error
	  *
	  * result = fd.GetValue(fd, &value);
	  * if( result.isError() )
	  *	//handle error
	  *
	  * cout << "value = " << value << endl;
	  * @endcode
	 *
	 * @param name - name of field to get
	 * @param field - out parameter, field object to copy info into
	 * @return status - result of the operation
	 */
	Status CALL_TYPE GetField(const char *name, Field &field);

	/** @fn GetFieldCount(GMSEC_I32 &count)
	 * @brief This function gets the number of fields contained in this message.
	 *
	 * @param count - out parameter, number of fields in this message
	 * @return status - result of the operation
	 */
	Status CALL_TYPE GetFieldCount(GMSEC_I32 &count);

	/** @fn GetFirstField(Field &field)
	 * @brief This function and GetNextField() are used to iterate through the fields of a message.
	  *
	  * Example:
	  * @code
	  * Field fd;
	  *
	  * // get first field
	  * result = msg->GetFirstField(fd);
	  * while( !result.isError() )
	  * {
	  *	// do something with the field here
	  *
	  *	// get next field
	  *	result = msg->GetNextField(fd);
	  * }
	  * @endcode
	 *
	 * @param field - field object to copy field into
	 * @return status - result of the operation
	 *
	 * @sa GetNextField()
	 */
	Status CALL_TYPE GetFirstField(Field &field);

	/** @fn GetNextField(Field &field)
	 * @brief This function and GetFirstField() are used to iterate through the fields of a message.
	 *
	 * @param field - field object to copy field into
	 * @return status - result of the operation
	 *
	 * @sa GetFirstField()
	 */
	Status CALL_TYPE GetNextField(Field &field);

	/** @fn CopyFields(Message *toMsg)
	 * @brief This function will copy all fields from this message to a new message in the fastest way possible
	 *
	 * @param toMsg - the message to copy fields into
	 * @return status - result of the operation
	 */
	Status CALL_TYPE CopyFields(Message *toMsg);

	/** @fn ToXML( const char *&xml )
	 * @brief This function will dump a message to an xml string. This format is compatible with
	  * the format used by FromXML() and ConfigFile::GetMessage().
	  *
	  * XML Format example:
	  * @code
	  * <MESSAGE SUBJECT="GMSEC.MISSION.CONST.SAT.EVT.MSG" KIND="PUBLISH">
	  * 	<FIELD TYPE="CHAR" NAME="char_field">c</FIELD>
	  * 	<FIELD TYPE="BOOL" NAME="bool_field">TRUE</FIELD>
	  * 	<FIELD TYPE="SHORT" NAME="short_field">123</FIELD>
	  * 	<FIELD TYPE="USHORT" NAME="ushort_field">123</FIELD>
	  * 	<FIELD TYPE="LONG" NAME="long_field">123</FIELD>
	  * 	<FIELD TYPE="ULONG" NAME="ulong_field">123</FIELD>
	  * 	<FIELD TYPE="STRING" NAME="string_field">This is a test</FIELD>
	  * 	<FIELD TYPE="FLOAT" NAME="float_field">123</FIELD>
	  * 	<FIELD TYPE="DOUBLE" NAME="double_field">123</FIELD>
	  * 	<FIELD TYPE="BIN" NAME="bin_field">4a4c4d4e4f5051</FIELD>
	  * </MESSAGE>
	  * @endcode
	  *
	  * @param xml - out parameter, xml string
	 * @return status - result of the operation
	  *
	  * @sa ConfigFile::GetMessage()
	 */
	Status CALL_TYPE ToXML(const char *&xml);

	/** @fn FromXML( const char *xml )
	 * @brief This function will set subject, kind, and fields into a message as specified by the passed in
	 * xml string. The format is compatible with what is used by ToXML() and ConfigFile::GetMessage();
	 *
	  * @param xml - xml string to parse
	 * @return status - result of the operation
	  *
	  * @sa ConfigFile::GetMessage()
	 */
	Status CALL_TYPE FromXML(const char *xml);

	/** @fn GetMSGSize( GMSEC_U32 &size )
	 * @brief This function will return the physical storage size of the message in bytes.
	 *
	 * @note <b>This may not be available in all middleware implementations.</b>
	 *
	 * @param size - out parameter, size of message in bytes
	 * @return status - result of the operation
	 */
	Status CALL_TYPE GetMSGSize(GMSEC_U32 &size);

	/** @fn GetTime()
	 * @brief This function will return the current GMT time/date in GMSEC standard format.
	 * @warning This function is not reentrant and is deprecated.
	 * Please use gmsec::util::{getTime_s/formatTime_s}.
	 *
	 * Example format:
	 * @code
	 * 2004-314-18:56:57.350
	 * @endcode
	 *
	 * @return current date/time in GMSEC standard format
	 */
	static const char * CALL_TYPE GetTime();

	/** @fn LookupKind( const char *kind )
	 * @brief Lookup a standard message kind identifier by string name. The valid strings
	 * correspond to the last part of the identifier names. ex. "PUBLISH" => GMSEC_MSG_PUBLISH
	 *
	 * @param kind - string that matches a known GMSEC message type
	 * @return GMSEC message kind identifier, or GMSEC_MSG_UNSET
	 *
	 * @sa GMSEC_MSG_KIND
	 */
	static GMSEC_MSG_KIND LookupKind(const char *kind);

#ifdef GMSEC_EXPOSE_TRACKING
	/** @fn SetTracking( int flag )
	 * @brief Turn tracking fields on/off for this Message
	 */
	void SetTracking(int flag);
	void SetTrackingNode(int flag);
	void SetTrackingProcessId(int flag);
	void SetTrackingUserName(int flag);
	void SetTrackingConnectionId(int flag);
	void SetTrackingPublishTime(int flag);
	void SetTrackingUniqueId(int flag);
	void SetTrackingMwInfo(int flag);

	/** @fn GetTracking()
	 * @brief Get the tracking fields state for this Message
	 * @return (MESSAGE_TRACKINGFIELDS_UNSET,MESSAGE_TRACKINGFIELDS_ON,MESSAGE_TRACKINGFIELDS_OFF)
	 */
	int GetTracking();
	int GetTrackingNode();
	int GetTrackingProcessId();
	int GetTrackingUserName();
	int GetTrackingConnectionId();
	int GetTrackingPublishTime();
	int GetTrackingUniqueId();
	int GetTrackingMwInfo();
#endif /* GMSEC_EXPOSE_TRACKING */

	/* Unfortunately, some compilers we currently support (gcc 3.2.3) will
	not allow some things that need to be done if the constructor is locked
	down.
	*/
private:

	/** @fn Message()
	 * @brief Default constructor - initializes the message instance
	 *
	 */
	Message();

	/** @fn ~Message()
	 * @brief Destructor - cleans up the message instance
	 */
	~Message();


	friend class internal::MessageBuddy;

	internal::BaseMessage *ptr;
};

} //namespace gmsec

#endif /* gmsec_Message_h */
