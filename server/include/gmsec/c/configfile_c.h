
/* Copyright 2007-2010 United States Government as represented by the     */
/* Administrator of The National Aeronautics and Space Administration. All */
/* Rights Reserved.                                                        */


/** @file configfile_c.h
 *
 *  @author Brian D Gregory
 *  @author Interface & Control Systems, Inc.
 *  @date Nov 01, 2004
 *
 *  @brief This file contains function for handling generic configuration files.
 *
 *  Example configuration file format:
 *  @code
 * <?xml version="1.0" encoding="UTF-8"?>
 * <DEFINITIONS>
 *	<SUBSCRIPTION NAME="events" PATTERN="GMSEC.MISSION.CONST.SAT.EVT.MSG.>"/>
 *	<SUBSCRIPTION NAME="custom1" PATTERN="CUSTOM.MESSAGE.SUBJECTS.*"/>
 * 	<CONFIG NAME="config1">
 * 		<PARAMETER NAME="connectiontype">gmsec_icsswb</PARAMETER>
 * 		<PARAMETER NAME="hostname">localhost</PARAMETER>
 * 		<PARAMETER NAME="port">10005</PARAMETER>
 * 	</CONFIG>
 * 	<CONFIG NAME="config2">
 * 		<PARAMETER NAME="connectiontype">gmsec_ss</PARAMETER>
 * 		<PARAMETER NAME="server">tcp:10.1.2.159</PARAMETER>
 * 		<PARAMETER NAME="tracking">true</PARAMETER>
 * 		<PARAMETER NAME="isthreaded">true</PARAMETER>
 * 		<PARAMETER NAME="compress">true</PARAMETER>
 *	</CONFIG>
 *	<MESSAGE NAME="msg1" SUBJECT="GMSEC.MISSION.CONST.SAT.EVT.MSG" KIND="PUBLISH">
 * 		<FIELD TYPE="CHAR" NAME="char_field">c</FIELD>
 * 		<FIELD TYPE="BOOL" NAME="bool_field">TRUE</FIELD>
 * 		<FIELD TYPE="SHORT" NAME="short_field">123</FIELD>
 * 		<FIELD TYPE="USHORT" NAME="ushort_field">123</FIELD>
 * 		<FIELD TYPE="LONG" NAME="long_field">123</FIELD>
 * 		<FIELD TYPE="ULONG" NAME="ulong_field">123</FIELD>
 * 		<FIELD TYPE="STRING" NAME="string_field">This is a test</FIELD>
 * 		<FIELD TYPE="FLOAT" NAME="float_field">123</FIELD>
 * 		<FIELD TYPE="DOUBLE" NAME="double_field">123</FIELD>
 * 		<FIELD TYPE="BIN" NAME="bin_field">4a4c4d4e4f5051</FIELD>
 * 	</MESSAGE>
 * </DEFINITIONS>
 *  @endcode
 *
 *
**/

#ifndef gmsec_c_configfile_c_h
#define gmsec_c_configfile_c_h

#include <gmsec/util/wdllexp.h>
#include <gmsec_defs.h>

#ifdef __cplusplus
extern "C"
{
#endif

	/** @fn CreateConfigFile(GMSEC_CONFIGFILE_HANDLE *cfgfile, const char *filepath, GMSEC_STATUS_HANDLE status)
	 * @brief Create a ConfigFile and associate a file.  The created status MUST be cleaned up with DestroyConfigFile()
	 * when no longer needed.
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[out] cfgfile - ConfigFile handle to be created
	 * @param[in]  filepath - full path to file to load & parse
	 * @param[out] status - out parameter operation result status
	 *
	 * @sa DestroyConfigFile()
	 */
	GMSEC_API void CreateConfigFile(GMSEC_CONFIGFILE_HANDLE *cfgfile, const char *filepath, GMSEC_STATUS_HANDLE status);

	/** @fn DestroyConfigFile(GMSEC_CONFIGFILE_HANDLE *cfgfile, GMSEC_STATUS_HANDLE status)
	 *
	 * @brief Reclaim / cleanup a ConfigFile
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  cfgfile - ConfigFile handle
	 * @param[out] status - out parameter operation result status
	 *
	 * @sa CreateConfigFile()
	 */
	GMSEC_API void DestroyConfigFile(GMSEC_CONFIGFILE_HANDLE *cfgfile, GMSEC_STATUS_HANDLE status);

	/** @fn LoadConfigFile(GMSEC_CONFIGFILE_HANDLE cfgfile, GMSEC_STATUS_HANDLE status)
	 *
	 * @brief This will cause the file to be loaded & parsed
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  cfgfile - ConfigFile handle
	 * @param[out] status - out parameter operation result status
	 */
	GMSEC_API void LoadConfigFile(GMSEC_CONFIGFILE_HANDLE cfgfile, GMSEC_STATUS_HANDLE status);

	/** @fn FromXMLConfigFile(GMSEC_CONFIGFILE_HANDLE cfgfile, const char *xml, GMSEC_STATUS_HANDLE status)
	 *
	 * @brief This will parse the xml string as a config file
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[out] cfgfile - ConfigFile handle
	 * @param[in]  xml - string xml input
	 * @param[out] status - out parameter operation result status
	 */
	GMSEC_API void FromXMLConfigFile(GMSEC_CONFIGFILE_HANDLE cfgfile, const char *xml, GMSEC_STATUS_HANDLE status);


	/** @fn ToXMLConfigFile(GMSEC_CONFIGFILE_HANDLE cfgfile, const char **xml, GMSEC_STATUS_HANDLE status)
	 *
	 * @brief This will output the config file in xml format
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  cfgfile - ConfigFile Handle
	 * @param[out] xml - string xml output
	 * @param[out] status - out parameter operation result status
	 */
	GMSEC_API void ToXMLConfigFile(GMSEC_CONFIGFILE_HANDLE cfgfile, const char **xml, GMSEC_STATUS_HANDLE status);

	/** @fn LookupConfigFileConfig(GMSEC_CONFIGFILE_HANDLE cfgfile, const char *name, GMSEC_CONFIG_HANDLE cfg, GMSEC_STATUS_HANDLE status)
	 *
	 * @brief This will seed a config with values from a named \<CONFIG/\> block in the config file
	 *
	 *  Example config file load & get config:
	 *  @code
	 *
	 * // create configfile
	 * CreateConfigFile(&cfgfile, "path/to/file.xml");
	 * if( isStatusError(result) )
	 *	// handle error
	 *
	 * // load & parse
	 * LoadConfigFile(cfgfile,result);
	 * if( isStatusError(result) )
	 *	//handle error
	 *
	 * // create config
	 * CreateConfig(&cfg,result);
	 * if( isStatusError(result) )
	 *	// handle error
	 *
	 * // get config from file
	 * LookupConfigFileConfig(cfgfile,"config1",cfg,result);
	 * if( isStatusError(result) )
	 *	//handle error
	 *  @endcode
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  cfgfile - ConfigFile Handle
	 * @param[in]  name - value of name="" attribute in \<CONFIG/\> block to be loaded
	 * @param[out] cfg - config that has already been created with CreateConfig()
	 * @param[out] status - out parameter operation result status
	 */
	GMSEC_API void LookupConfigFileConfig(GMSEC_CONFIGFILE_HANDLE cfgfile, const char *name, GMSEC_CONFIG_HANDLE cfg, GMSEC_STATUS_HANDLE status);

	/** @fn LookupConfigFileMessage(GMSEC_CONFIGFILE_HANDLE cfgfile, const char *name, GMSEC_MESSAGE_HANDLE msg, GMSEC_STATUS_HANDLE status)
	 *
	 * @brief This will seed a message with values from a named \<MESSAGE/\> block in the config file
	 *
	 *  Example config file load & get message:
	 *  @code
	 *
	 * // create configfile
	 * CreateConfigFile(&cfgfile, "path/to/file.xml");
	 * if( isStatusError(result) )
	 *	// handle error
	 *
	 * // load & parse
	 * LoadConfigFile(cfgfile,result);
	 * if( isStatusError(result) )
	 *	//handle error
	 *
	 * // create message
	 * CreateMessageDflt(conn,&msg,result);
	 * if( isStatusError(result) )
	 *	//handle error
	 *
	 * // Get Message from ConfigFile
	 * LookupConfigFileMessage(cfgfile,"msg1",msg,result);
	 * if( isStatusError(result) )
	 *	//handle error
	 *  @endcode
	 *
	 * @note As with all C API functions, a status MUST BE CREATED with CreateStatus() and
	 * passed in as the status parameter in order to receive any resulting status.
	 *
	 * @param[in]  cfgfile - ConfigFile Handle
	 * @param[in]  name - value of name="" attribute in \<MESSAGE/\> block to be loaded
	 * @param[out] msg - message that has already been created with CreateMessage()
	 * @param[out] status - out parameter operation result status
	 */
	GMSEC_API void LookupConfigFileMessage(GMSEC_CONFIGFILE_HANDLE cfgfile, const char *name, GMSEC_MESSAGE_HANDLE msg, GMSEC_STATUS_HANDLE status);

	/** @fn LookupConfigFileSubscription( GMSEC_CONFIGFILE_HANDLE cfgfile, const char *name, const char **pattern, GMSEC_STATUS_HANDLE status )
	 * @brief This function will look up a subscription pattern defined in
	 * the config file. This is usefull to allow easy modification of subject
	 * names without code changes.
	 *
	 * Example
	 * @code
	 * char *pattern = NULL;
	 *
	 * // lookup subject pattern by name
	 * LookupConfigFileSubscription( cfgfile, "events", &pattern, result );
	 * if( isStatusError(result) )
	 *	//handle error
	 *
	 * printf("Subscribe to: %s\n",pattern);
	 *
	 * // subscribe on the connection
	 * Subscribe( conn, pattern, result );
	 * if( isStatusError(result) )
	 *	//handle error
	 * @endcode
	 *
	 * @param[in]  cfgfile - ConfigFile handle
	 * @param[in]  name - value in the name="" attribute of the \<SUBSCRIPTION/\> block containing the subscription pattern
	 * @param[out] pattern - out parameter, the pattern defined in the config file
	 * @param[out] status - out parameter operation result status
	 */
	GMSEC_API void LookupConfigFileSubscription(GMSEC_CONFIGFILE_HANDLE cfgfile, const char *name, const char **pattern, GMSEC_STATUS_HANDLE status);


#ifdef __cplusplus
}	// extern "C"
#endif

#endif /* gmsec_c_configfile_c_h */


