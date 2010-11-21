
/* Copyright 2007-2010 United States Government as represented by the     */
/* Administrator of The National Aeronautics and Space Administration.    */
/* All Rights Reserved.                                                   */

#ifndef gmsec_util_timeutil_h
#define gmsec_util_timeutil_h

#include <gmsec_defs.h>
#include <gmsec/util/wdllexp.h>

namespace gmsec
{
namespace util
{

struct TimeBuffer
{
	double seconds;
	int millis;
};

/**
* \fn getTime_s ()
* \brief Returns the current time in seconds since Jan 1, 1970.
* \note If buffer is non-zero, it will be populated.
*/
GMSEC_API double CALL_TYPE getTime_s(TimeBuffer *buffer = 0);

/**
* \fn formatTime_s (double time_s, char *buffer)
* \brief Formats a GMSEC time string YYYY-DDD-HH-MM-SS.sss
* \note buffer must be at least TIME_BUFSIZE characters
*/
static const int TIME_BUFSIZE = 32;
GMSEC_API void CALL_TYPE formatTime_s(double time_s, char *buffer);

} // namespace util
} // namespace gmsec


#endif /* gmsec_util_timeutil_h */
