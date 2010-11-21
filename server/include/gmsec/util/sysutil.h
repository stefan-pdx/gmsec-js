
/* Copyright 2007-2010 United States Government as represented by the     */
/* Administrator of The National Aeronautics and Space Administration.    */
/* All Rights Reserved.                                                   */

#ifndef gmsec_internal_sysutil_h
#define gmsec_internal_sysutil_h

#include <string>
#include <gmsec_defs.h>
#include <gmsec/util/wdllexp.h>

namespace gmsec
{
namespace util
{

/**
* \fn millisleep (int milliseconds)
* \brief Sleeps for the specified number of milliseconds (at least 1).
*/
GMSEC_API int CALL_TYPE millisleep(int milliseconds);

/**
* \fn getUserName (string &name)
* \brief Determines the user name.
* \return If there is an error, the return value will be non-zero
*/
GMSEC_API int CALL_TYPE getUserName(std::string &s);

/**
* \fn getHostName (string &name)
* \brief Determines the host name.
* \return If there is an error, the return value will be non-zero
*/
GMSEC_API int CALL_TYPE getHostName(std::string &s);

/**
* \fn getErrorString (string &name)
* \brief Returns an integer with the error code
*/
GMSEC_API bool CALL_TYPE getErrorString(int code, std::string &s);

} // namespace util
} // namespace gmsec


#endif /* gmsec_internal_sysutil_h */
