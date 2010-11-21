
/* Copyright 2007-2010 United States Government as represented by the     */
/* Administrator of The National Aeronautics and Space Administration.    */
/* All Rights Reserved.                                                   */

#ifndef gmsec_internal_rawbuf_h
#define gmsec_internal_rawbuf_h

#include <cstdio>
#include <streambuf>

namespace gmsec
{
namespace util
{

/**
	rawbuf is an output streambuf backed by a fixed length character array.
	The rawbuf can own the array or not.
*/

class rawbuf : public std::streambuf
{
private:

	char *buffer;
	size_t allocated;
	bool owned;


public:

	rawbuf(char *p, size_t size, bool transfer = false)
			: buffer(p), allocated(size), owned(transfer)
	{
		setp(buffer, buffer + allocated);
	}

	rawbuf(size_t size)
			: buffer(new char[size]), allocated(size), owned(true)
	{
		setp(buffer, buffer + allocated);
	}

	~rawbuf()
	{
		if (owned)
			delete[] buffer;
		buffer = 0;
	}

	char *release()
	{
		owned = false;
		return buffer;
	}

protected:

	int overflow(int c)
	{
		return EOF;
	}

};

} // namespace util
} // namespace gmsec

#endif /* gmsec_internal_rawbuf_h */
