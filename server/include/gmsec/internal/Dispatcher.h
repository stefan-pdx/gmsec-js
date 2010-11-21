
/* Copyright 2007-2010 United States Government as represented by the     */
/* Administrator of The National Aeronautics and Space Administration. All */
/* Rights Reserved.                                                        */


/** @file Dispatcher.h
 *
 *  @author Everett Cary Jr.
 *  @author Emergent Space Technologies.
 *  @date April 24, 2003
 *
 * @brief Perform asynchronous, automated delivery of messages off the message
 *  queue to callback functions
 *
 *
**/

#ifndef gmsec_internal_Dispatcher_h
#define gmsec_internal_Dispatcher_h

#include <gmsec/Status.h>
#include <gmsec/util/Thread.h>
#include <gmsec/util/CountDownLatch.h>
#include <gmsec/internal/BaseConnection.h>

namespace gmsec
{

class Callback;

namespace internal
{


/**	@class Dispatcher
 *
 *  @brief Threaded class to automatically handle delivery of messages
 *	to the callbacks
 */
class Dispatcher : public gmsec::util::Thread
{
public:

	Dispatcher();
	virtual ~Dispatcher();

	/** @fn init( Connection *conn )
	 *
	 * Initialize the dispatcher.  This method must be called prior
	 * to calling the run() method.
	 *
	 * @param conn The connection which the auto-dispatcher will operate
	 *			   on.
	 * @return bool - false if the connection is null, if the dispatcher
	 *		has already been initialized, or if trying to initialize while
	 *		the dispatcher is running.  True on success
	 */
	virtual bool CALL_TYPE init(Connection *conn);

	/** @fn run()
	 * Overload the thread run method to perform the dispatch
	 * task.  The init() method must be called prior to this
	 * method, else the method will fail.  If error are encountered
	 * during the running of this method, the dispatcher global
	 * status code can be checked to determine the last error. To
	 * terminate this method, the destroy() method must be called.
	 *
	 * NOTE: The messages genereted within this method are
	 *		 cleaned up within this method and should not be
	 *		 freed by the client callbacks.
	 */
	virtual void CALL_TYPE run();

	/** @fn shutdown()
	 * Set a flag so that the run loop completes.
	 */
	virtual void CALL_TYPE shutdown();

	/** @fn GetLastDispatcherStatusCode()
	 * Retrieve the last error status code triggered in the run() method.
	 * Since the dispatcher runs in a seperate thread, this method allows
	 * the GMSEC API to determine if the thread has abended or caused
	 * an illegal action.  Once this method is called, the status code
	 * is cleared.
	 */
	virtual Status CALL_TYPE GetLastDispatcherStatusCode();

	virtual bool isRunning()
	{
		return m_running;
	}

	bool await(long milliseconds);


private:

	/* Reference to the connection the dispatcher is working on */
	Connection *m_connection;

	/* Last unsuccessful status code triggered by the run() method */
	Status m_StatusCode;

	/* CountDownLatch to signal starting thread that the dispatcher has
	started up properly */
	gmsec::util::CountDownLatch startupLatch;

	// status flag turned on, when the run method is called, and
	// turned off, when the destroy is called.
	volatile bool m_running;
	volatile bool m_hascompleted;

}; //..class Dispatcer

} // namespace internal
} // namespace gmsec

#endif /* gmsec_internal_Dispatcher_h */

