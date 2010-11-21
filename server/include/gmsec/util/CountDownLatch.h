/* Copyright 2007-2010 United States Government as represented by the     */
/* Administrator of The National Aeronautics and Space Administration. All */
/* Rights Reserved.                                                        */


/** @file CountDownLatch.h
 *
 *  @brief This file contains a cross-platform CountDownLatch that functions the same
 *	as the Java class of the same name.
**/

#include <gmsec/util/Condition.h>
#include <gmsec/util/Mutex.h>

#ifndef gmsec_util_CountDownLatch_h
#define gmsec_util_CountDownLatch_h

namespace gmsec
{
namespace util
{

class GMSEC_API CountDownLatch
{
public:
	CountDownLatch(int count);
	~CountDownLatch();
	void await();
	bool await(int milliTimeout);
	void countDown();
	int getCount();
private:
	int count;
	gmsec::util::Condition condition;
};

}
}

#endif

