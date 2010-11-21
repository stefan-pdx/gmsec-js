
/* Copyright 2007-2010 United States Government as represented by the     */
/* Administrator of The National Aeronautics and Space Administration. All */
/* Rights Reserved.                                                        */


/** @file Thread.h
 *
 *  @author Brian D Gregory
 *  @author Interface & Control Systems, Inc.
 *  @date April 24, 2003
 *
 *  @brief This file contains a multi-platform abstraction implementation of a Thread class
 *
 *
**/
#ifndef gmsec_util_Thread_h
#define gmsec_util_Thread_h

#include <gmsec/util/wdllexp.h>
#include <gmsec/util/shared_ptr.h>

#include <string>


// This prevents having to include windows.h
//------------------------------------------
#ifdef WIN32
typedef void * HANDLE;
typedef unsigned long DWORD;
#else
#include <pthread.h>
#endif

namespace gmsec
{

namespace util
{


/**	@class Thread
 *
 *	@brief This provides a multi-platform thread implementation
 *
**/
class GMSEC_API Thread
{
private:
#ifdef WIN32
	HANDLE				fThreadHandle;
	DWORD				fThreadID;
#else
	pthread_t			fThreadHandle;
#endif

	bool				fJoined;
	bool				fDetached;

	shared_ptr<Thread>		fLifeline;
	std::string			fName;


	static void * process0(void *arg);
	void process();


protected:
	void setName(std::string name)
	{
		fName = name;
	}

public:
	Thread(bool selfReferential = false);
	virtual ~Thread();

	/** @fn start()
	 *
	 *	@brief This function causes the thread to being execution
	 *
	**/
	int CALL_TYPE start();

	/**	@fn isCurrentContext()
	 *
	 *	@brief this function return 1 if it is called from the
	 *	context of the run function or 0 if it is called from
	 *	a different thread
	 *
	**/
	int CALL_TYPE isCurrentContext();

	/**	@fn join()
	 *
	 *	@brief Blocks the calling thread until the other thread terminates
	 *
	**/
	int CALL_TYPE join();

	int CALL_TYPE detach();

	/** @fn run()
	 *
	 *	@brief The action of the thread occurs here
	 *
	**/
	virtual void CALL_TYPE run(void) = 0;

	void attach(shared_ptr<Thread> &mine);
};

} // namespace util
} // namespace gmsec

#endif /* gmsec_util_Thread_h */
