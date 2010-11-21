
/* Copyright 2007-2010 United States Government as represented by the     */
/* Administrator of The National Aeronautics and Space Administration. All */
/* Rights Reserved.                                                        */


/** @file GarbageCollector.h
 *
 *  @brief This object allocates object needed to manage the C API layer, and
 *  cleans up (or garbage collects) as needed.
 *
 *
**/

#ifndef gmsec_internal_GarbageCollector_h
#define gmsec_internal_GarbageCollector_h

#include <string>
#include <map>
#include <vector>

#include <gmsec_defs.h>
#include <gmsec/util/wdllexp.h>


namespace gmsec
{

class Callback;

namespace internal
{

class CAPICallback;
class CAPIErrorCallback;
class CAPIReplyCallback;
class CAPILogHandler;

/**	@class GarbageCollector
 *
 *	@brief This class is responsible for managing the CAPI callbacks.
 *
**/
class GMSEC_API GarbageCollector
{
private:
	typedef std::map<GMSEC_CALLBACK *, CAPICallback *> cbAPILkp;
	typedef cbAPILkp::const_iterator cbAPILkpItr;
	cbAPILkp fCbAPILkps;

	typedef std::map<GMSEC_ERROR_CALLBACK *, CAPIErrorCallback *> cbErrorLkp;
	typedef cbErrorLkp::const_iterator cbErrorLkpItr;
	cbErrorLkp fCbErrorLkps;

	typedef std::map<GMSEC_REPLY_CALLBACK *, CAPIReplyCallback *> cbReplyLkp;
	typedef cbReplyLkp::const_iterator cbReplyLkpItr;
	cbReplyLkp fCbReplyLkps;

public:
	GarbageCollector();

	/**	@fn ~GarbageCollector()
	 *
	 *	@brief Destroys the garbage collector and all associated callbacks.
	 *
	**/
	virtual ~GarbageCollector();

	/**	@fn newCAPICallback(GMSEC_CALLBACK *cb)
	 *
	 *	@brief Creates a new CAPI callback if the callback does not already exist
	 *
	**/
	CAPICallback *newCAPICallback(GMSEC_CALLBACK *cb);

	/**	@fn deleteCAPICallback(GMSEC_CALLBACK *cb)
	 *
	 *	@brief Deletes the referenced CAPI callback
	 *
	**/
	void deleteCAPICallback(GMSEC_CALLBACK *cb);

	/**	@fn getCAPICallback(GMSEC_CALLBACK *cb)
	 *
	 *	@brief Retreives the CAPI callback associated with the referenced GMSEC callback
	 *
	**/
	CAPICallback *getCAPICallback(GMSEC_CALLBACK *cb);

	/**	@fn newCAPIErrorCallback(GMSEC_ERROR_CALLBACK *cb)
	 *
	 *	@brief Creates a new CAPI error callback if the callback does not already exist
	 *
	**/
	CAPIErrorCallback *newCAPIErrorCallback(GMSEC_ERROR_CALLBACK *cb);

	/**	@fn deleteCAPIErrorCallback(GMSEC_CALLBACK *cb)
	 *
	 *	@brief Deletes the referenced CAPI error callback
	 *
	**/
	void deleteCAPIErrorCallback(GMSEC_ERROR_CALLBACK *cb);

	/**	@fn getCAPIErrorCallback(GMSEC_CALLBACK *cb)
	 *
	 *	@brief Retreives the CAPI error callback associated with the referenced
	 *  GMSEC error callback
	 *
	**/
	CAPIErrorCallback *getCAPIErrorCallback(GMSEC_ERROR_CALLBACK *cb);

	/**	@fn newCAPIReplyCallback(GMSEC_ERROR_CALLBACK *cb)
	 *
	 *	@brief Creates a new CAPI reply callback if the callback does not already exist
	 *
	**/
	CAPIReplyCallback *newCAPIReplyCallback(GMSEC_REPLY_CALLBACK *cb, GMSEC_ERROR_CALLBACK *er);

	/**	@fn deleteCAPIReplyCallback(GMSEC_CALLBACK *cb)
	 *
	 *	@brief Deletes the referenced CAPI reply callback
	 *
	**/
	void deleteCAPIReplyCallback(GMSEC_REPLY_CALLBACK *cb);

	/**	@fn getCAPIReplyCallback(GMSEC_CALLBACK *cb)
	 *
	 *	@brief Retreives the CAPI reply callback associated with the referenced
	 *  GMSEC error callback
	 *
	**/
	CAPIReplyCallback *getCAPIReplyCallback(GMSEC_REPLY_CALLBACK *cb);
};

} //namespace internal
} //namespace gmsec

#endif /* gmsec_internal_GarbageCollector_h */

