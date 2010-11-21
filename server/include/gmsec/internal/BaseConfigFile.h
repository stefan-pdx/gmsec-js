
/* Copyright 2007-2010 United States Government as represented by the     */
/* Administrator of The National Aeronautics and Space Administration. All */
/* Rights Reserved.                                                        */


/** @file BaseConfigFile.h
 *  @brief This file contains a class for managing configuration files.
 *
 *
**/

#ifndef gmsec_internal_BaseConfigFile_h
#define gmsec_internal_BaseConfigFile_h

#include <string>
#include <map>
#include <vector>

#include <gmsec/util/wdllexp.h>
#include <gmsec/Status.h>
#include <gmsec/internal/ci_less.h>

// forward declare so we don't have to include libxml
typedef struct _xmlDoc xmlDoc;
typedef struct _xmlNode xmlNode;

namespace gmsec
{

// forward declare so we don't end up with recursive includes
class Config;
class Message;

namespace internal
{

/** @class BaseConfigFile
 *	@brief This class is for the managment of standard configuration files.
 * In the future, there may be alternative implementations, but for now
 * it just uses the pre-3.0 implementation.
 * @sa gmsec::ConfigFile
 *
**/
class GMSEC_API BaseConfigFile
{
private:
	std::string fXmlOutput;

	std::string fFilePath;
	xmlDoc *fDoc;
	bool fDocLoaded;

	typedef std::map<std::string, xmlNode *, ci_less> nodeMap;
	typedef nodeMap::iterator nodeItr;

	nodeMap fConfigNodes;
	nodeMap fMsgNodes;
	nodeMap fSubscriptionNodes;

	// helper function to parse XML
	Status ParseDoc(xmlDoc *doc);

protected:
	// helper to get name="" attribute from node
	const char *getNameAttr(xmlNode *node);
	// allow subclasses access to xml document
	xmlDoc *GetXmlDocument()
	{
		return fDoc;
	}

public:

	/** @fn BaseConfigFile( const char *filepath )
	 * @brief This constructor creates the association of this object
	 * with a configuration file.
	 *
	 * @param filepath - full or relative path to configuration file
	 */
	BaseConfigFile(const char *filepath);

	/** @fn ~BaseConfigFile()
	 * @brief Destructor.
	 */
	virtual ~BaseConfigFile();

	/** @fn Load()
	 * @brief This function opens the configuration file and parses the XML
	 * It will return errors explaining what, if anything, went wrong.
	 *
	 * @return status of whether the load and parse was successful
	 */
	Status Load();

	/** @fn FromXML( const char *xml)
	 * @brief This function parses the XML in the passed in string.
	 * It will return errors explaining what, if anything, went wrong.
	 *
	 * @return status of whether the parse and parse was successful
	 */
	Status FromXML(const char *xml);

	/** @fn ToXML( const char *&xml )
	 * @brief This function produces an XML config file containing all
	 *	known config file tags.
	 *
	 * @return status of whether the parse and parse was successful
	 */
	Status ToXML(const char *&xml);

	/** @fn IsLoaded()
	 * @brief This function will return true is the config file was successfully loaded
	 *
	 * @return true if file loaded
	 */
	bool IsLoaded()
	{
		return fDocLoaded;
	}

	/** @fn LookupConfig( const char *name, gmsec::Config *cfg )
	 * @brief This function will attempt to find a named connection in the
	 * config file and set the passed in config object with
	 * the loaded values.
	 *
	 *  Example config file load and get config:
	 *  @code
	  * // create configfile object and attach to file
	  * BaseConfigFile cf("path/to/file.xml");
	  *
	 * // load & parse
	  * result = cf.Load();
	  * if( result.isError() )
	 *	//handle error
	  *
	  * // get 2 configs from file
	  * Config c1,c2;
	  * cf.LookupConfig("config1",&c1);
	  * cf.LookupConfig("config2",&c2);
	  *  @endcode
	 *
	 * @param name - value in the name="" attribute of the \<CONFIG/\> block to seed the gmsec::Config object
	 * @param cfg - Config object to set
	 * @return status of whether named CONFIG block was found
	 */
	Status LookupConfig(const char *name, gmsec::Config *cfg);

	/** @fn LookupMessage( const char *name, gmsec::Message *msg )
	 * @brief This function will attempt to find a named message in the
	 * config file and create a new message from that xml
	 * the loaded fields and values.
	 *
	 *  Example config file load and get message:
	 *  @code
	  * // create configfile object and attach to file
	  * BaseConfigFile cf("path/to/file.xml");
	  *
	 * // load & parse
	  * result = cf.Load();
	  * if( result.isError() )
	 *	//handle error
	  *
	  * // create message from config file entry
	  * Message *msg = NULL;
	  * result = conn->CreateMessage(msg);
	  * if( result.isError() )
	 *	//handle error
	  *
	  * // load message subject, type, and fields from config file
	  * result = cf.LookupMessage("msg1",msg);
	  * if( result.isError() )
	 *	//handle error
	  *  @endcode
	 *
	 * @param name - value in the name="" attribute of the \<MESSAGE/\> block to seed the gmsec::Message object
	 * @param msg - Message object to set
	 * @return status of whether named MESSAGE block was found
	 */
	Status LookupMessage(const char *name, gmsec::Message *msg);

	/** @fn LookupSubscription( const char *name, const char *&pattern )
	 * @brief This function will look up a subscription pattern defined in
	 * the config file. This is usefull to allow easy modification of subject
	 * names without code changes.
	 *
	 * Example
	 * @code
	 * char *pattern = NULL;
	 *
	 * // lookup subject pattern by name
	 * result = cf.LookupSubscription( "events", pattern );
	  * if( result.isError() )
	 *	//handle error
	 *
	 * cout << "Subscribe to: " << pattern << endl;
	 *
	 * // subscribe on the connection
	 * result = conn->Subscribe(pattern);
	  * if( result.isError() )
	 *	//handle error
	 * @endcode
	 *
	 * @param name - value in the name="" attribute of the \<SUBSCRIPTION/\> block containing the subscription pattern
	 * @param pattern - out parameter, the pattern defined in the config file
	 * @return status of whether named SUBSCRIPTION block was found
	 */
	Status LookupSubscription(const char *name, const char *&pattern);

};

} // namespace internal
} // namespace gmsec

#endif /* gmsec_internal_BaseConfigFile_h */

