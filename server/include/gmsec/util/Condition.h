
/* Copyright 2007-2010 United States Government as represented by the     */
/* Administrator of The National Aeronautics and Space Administration. All */
/* Rights Reserved.                                                        */


/** @file Condition.h
 *
 *  @brief This file contains a class for cross platform condition variable support
**/

#ifndef gmsec_util_Condition_h
#define gmsec_util_Condition_h

#include <gmsec/util/Mutex.h>

#if defined (WIN32)
#include <windows.h>
#include <process.h>
#else
#include <pthread.h>
#endif
/*

EXAMPLES

internal mutex example

{
	Condition condition;
	Mutex &mutex = condition.getMutex();

	mutex.enterMutex();

	int reason = condition.wait();
	printf("condition got %d\n", reason);

	mutex.leaveMutex();
}


external mutex example
{
	Mutex external;
	Condition condition(external);

	external.enterMutex();

	int reason = condition.wait();
	printf("condition got %d\n", reason);

	external.leaveMutex();
}


If the Mutex class is updated to keep track of its ownership, this can
be made more robust and use less code as:


internal mutex example with AutoMutex

{
	Condition condition;
	AutoMutex hold(condition.getMutex());

	int reason = condition.wait();
	printf("condition got %d\n", reason);
}


external mutex example with AutoMutex
{
	Mutex external;
	Condition condition(external);

	AutoMutex hold(external);

	int reason = condition.wait();
	printf("condition got %d\n", reason);
}

*/


namespace gmsec
{
namespace util
{


class GMSEC_API Condition
{
public:

	enum
	{
		INVALID,
		TIMEOUT,
		USER
	};


	/**	@fn Condition()
		 *
		 *	@brief Standard constructor. Initializes the internal platform dependent
		 *	modules.
		 *
		**/
	Condition();

	/**	@fn Condition()
		 *
		 *	@brief Standard constructor. Initializes the internal platform dependent
		 *	modules with an externally managed mutex.
		 *
		**/
	Condition(gmsec::util::Mutex &mutex);

	/**	@fn ~Condition()
		 *
		 *	@brief Standard deconstructor. Runs platform dependent tear-down code.
		 *
		**/
	~Condition();

	/**	@fn getMutex()
		 *
		 *	@brief Returns a reference to the mutex.
		 *
		**/
	gmsec::util::Mutex &getMutex();


	/**	@fn Wait()
		 *
		 *	@brief The function will hold at this point until the thread is signaled.
		 *
		**/
	int Wait();

	/**	@fn Wait(int millis)
		 *
		 *	@brief The function will hold at this point until the thread is signaled or
		 *	the time specified has expired.
		 *
		 *	@param millis - The length of time in milliseconds to wait for the thread to
		 *	be signalled before continuing
		 *
		**/
	int Wait(int millis);

	/**	@fn Signal()
		 *
		 *	@brief This function signals any waiting threads to continue with their
		 *	activities.
		 *
		**/
	void Signal(int reason);

	/**	@fn SignalAll()
		 *
		 *	@brief This function signals all threads that are waiting on the condition
		 *	variable to continue with their activities.
		 *
		**/
	void Broadcast(int reason);

private:

	int status;
	bool ownsMutex;
	gmsec::util::Mutex *mutex;
	bool insideMutex;

#if defined (WIN32)
	/*
	HANDLE events_[2];
	int waiters;
	*/
	int waiters_count_;
	// Number of waiting threads.

	CRITICAL_SECTION waiters_count_lock_;

	HANDLE sema_;
	// Semaphore used to queue up threads waiting for the condition to
	// become signaled.

	HANDLE waiters_done_;
	// An auto-reset event used by the broadcast/signal thread to wait
	// for all the waiting thread(s) to wake up and be released from the
	// semaphore.

	int was_broadcast_;
	// Keeps track of whether we were broadcasting or signaling.  This
	// allows us to optimize the code if we're just signaling.


#else
	pthread_cond_t cv;
#endif
};


} // namespace internal
} // namespace gmsec


#endif /* gmsec_util_Condition_h */

