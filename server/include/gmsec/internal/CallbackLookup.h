
/* Copyright 2007-2010 United States Government as represented by the     */
/* Administrator of The National Aeronautics and Space Administration. All */
/* Rights Reserved.                                                        */


/** @file CallbackLookup.h
 *
 *  @author Brian D Gregory
 *  @author Interface & Control Systems, Inc.
 *  @date April 24, 2003
 *
 *  @brief This file contains a class responsible for matching incoming messages
 *  with subscriptions (including patterns).
 *
 *
**/

#ifndef gmsec_internal_CallbackLookup_h
#define gmsec_internal_CallbackLookup_h

#include <gmsec/util/wdllexp.h>
#include <gmsec/Status.h>
#include <gmsec/internal/ci_less.h>

#include <string>
#include <list>
#include <map>

namespace gmsec
{

class Callback;
class Connection;

namespace internal
{

class BaseMessage;


/** @class CallbackLookup
 *
 *	@brief This class keeps track of local subscriptions so that incoming messages
 *	can be matched with the callbacks that need to be called. This class is
 *	used internal to the Connection object and should not be used outside the
 *	GMSECAPI library.
*/
class GMSEC_API CallbackLookup
{
private:
	typedef std::list<Callback *> callbackList;
	typedef callbackList::const_iterator callbackItr;

	callbackList fCallbacks;

	typedef std::map<std::string, CallbackLookup *, ci_less> callbackLkp;
	typedef callbackLkp::const_iterator callbackLkpItr;

	callbackLkp fCallbackLkps;

	virtual Status CollectCallbacks(const char *subject, callbackList &callbacks);

public:
	CallbackLookup();
	virtual ~CallbackLookup();

	/** @fn AddCallback( const char *subject, Callback *cb )
	 *
	 *	@brief This function registers a subject or pattern with a Callback to
	 *	be called when messages that match are received.
	 */
	virtual Status CALL_TYPE AddCallback(const char *subject, Callback *cb);
	/** @fn RemoveCallback( const char *subject )
	 *
	 *	@brief This function removes all Callbacks that are registed for a
	 *	certain subject or pattern.
	 */
	virtual Status CALL_TYPE RemoveCallback(const char *subject);
	/** @fn RemoveCallback( const char *subject, Callback *cb )
	 *
	 *	@brief This function removes a particular Callback that is registed for a
	 *	certain subject or pattern.
	 */
	virtual Status CALL_TYPE RemoveCallback(const char *subject, Callback *cb);

	/** @fn DispatchMsg( const char *subject, Connection *conn, BaseMessage *msg )
	 *
	 *	@brief This function is called by the Connection framework to dispatch a
	 *	received message to all the callbacks that are registerd to
	 *	subject or pattern that matches the message's subject.
	 */
	virtual Status CALL_TYPE DispatchMsg(const char *subject, Connection *conn, BaseMessage *msg);

	/** @fn GetCallbackCount( const char *subject);
	*
	*	@brief This function returns the number of callbacks subscribed
	*	the given subject.
	*/
	virtual int CALL_TYPE GetCallbackCount(const char *subject);


};

} // namespace internal
} // namespace gmsec

#endif /* gmsec_internal_CallbackLookup_h */
