
/* Copyright 2007-2010 United States Government as represented by the     */
/* Administrator of The National Aeronautics and Space Administration.    */
/* All Rights Reserved.                                                   */

#ifndef gmsec_internal_strutil_h
#define gmsec_internal_strutil_h

#include <gmsec_defs.h>
#include <gmsec/util/wdllexp.h>
#include <string>
#include <vector>
#include <cstdarg>

namespace gmsec
{
namespace util
{

/**
* \fn stringLength (const char *s)
* \brief Returns the length of the string
*/
GMSEC_API int CALL_TYPE stringLength(const char *s);

/**
* \fn stringEquals (const char *s1, const char *s2)
* \brief Returns true if the two strings are the same
*/
GMSEC_API bool CALL_TYPE stringEquals(const char *s1, const char *s2);

/**
* \fn stringEqualsIgnoreCase (const char *s1, const char *s2)
* \brief Returns true if the two strings are the same ignoring case
*/
GMSEC_API bool CALL_TYPE stringEqualsIgnoreCase(const char *s1, const char *s2);

/**
* \fn stringNew (const char *s)
* \brief Returns a C string copy of s allocated with new char[].
*/
GMSEC_API char * CALL_TYPE stringNew(const char *s);

/**
* \fn stringCopy (char *dest, const char *src, int count)
* \brief Copies count bytes from src to dest.
*/
GMSEC_API void CALL_TYPE stringCopy(char *dest, const char *src, int count = -1);

/**
* \fn copyBytes (char *dest, const char *src, int count)
* \brief Copies count bytes from src to dest.
*/
GMSEC_API void CALL_TYPE copyBytes(void *dest, const void *src, int count);

/**
* \fn split(string str, char delimiter)
* \brief Splits a string and returns a vector of resulting strings.
*/
GMSEC_API std::vector<std::string> CALL_TYPE split(std::string str, char delimiter);

/**
* \fn trim(string str, char trim)
* \brief Trims leading and trailing instances of a character from a string and returns the result.
*/
GMSEC_API std::string CALL_TYPE trim(std::string str, char chop = ' ');

/**
* \fn stringFormat (char *buffer, int size, const char *format, ...);
* \brief Applies format to variable argument list putting the result
* to buffer of specified size.
*/
GMSEC_API void CALL_TYPE stringFormat(char *dest, int size, const char *format, ...);

/**
* \fn stringFormat (char *buffer, int size, const char *format, va_list args);
* \brief Similar to stringFormat but takes a va_list as the input argument
*/
GMSEC_API void CALL_TYPE stringFormatV(char * dest, int space, const char *format, va_list args);

} // namespace util
} // namespace gmsec


#endif /* gmsec_internal_strutil_h */
