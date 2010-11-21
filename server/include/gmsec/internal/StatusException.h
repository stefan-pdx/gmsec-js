
/* Copyright 2007-2010 United States Government as represented by the     */
/* Administrator of The National Aeronautics and Space Administration. All */
/* Rights Reserved.                                                        */


/** @file StatusException.h
 *
 *  @author Brian D Gregory
 *  @author Interface & Control Systems, Inc.
 *  @date May 25, 2004
 *
 *	@brief This class is used to throw a gmsec::Status as a C++ Exception
 *
 *
**/

#ifndef gmsec_internal_StatusException_h
#define gmsec_internal_StatusException_h


#include <gmsec_defs.h>
#include <gmsec/Status.h>
#include <gmsec/util/wdllexp.h>

namespace gmsec
{
namespace internal
{

/** @class StatusException
 *
 *	@brief This class is used to throw a gmsec::Status as a C++ Exception
 *
 *	@sa gmsec_errors.h
*/
class GMSEC_API StatusException : public gmsec::Status
{

public:
	/** @fn StatusException()
	 *
	 *	@brief Create a status set to GMSEC_STATUS_NO_ERROR.
	 */
	StatusException();

	/** @fn StatusException(GMSEC_STATUS_CLASS eclass, unsigned int code, const char *text)
	 *
	 *	@brief Create a status with specific class, code, and string.
	 */
	StatusException(GMSEC_STATUS_CLASS eclass, unsigned int code, const char *text);

	/** @fn StatusException( const StatusException &st)
	 *
	 *	@brief Deep copy constructor.
	 */
	StatusException(const StatusException &st);

	/** @fn ~StatusException()
	 *
	 *	@brief Destructor.
	 */
	virtual ~StatusException() {}

};

} // namespace internal
} // namespace gmsec

#endif /* gmsec_internal_StatusException_h */
