
/* Copyright 2007-2010 United States Government as represented by the     */
/* Administrator of The National Aeronautics and Space Administration. All */
/* Rights Reserved.                                                        */


/** @file Log.h
 *
 *  @brief This file contains the implementation of the logging framework for
 *  the GMSEC API
 *
**/

#ifndef gmsec_util_Log_h
#define gmsec_util_Log_h

#include <gmsec/util/wdllexp.h>
#include <gmsec/util/timeutil.h>
#include <sstream>
#include <iostream>
#include <string>
#include <stdio.h>

namespace gmsec
{
namespace util
{

typedef GMSEC_LOG_ENTRY LogEntry;

class DefaultHandler;

/** @class LogHandler
 *
 * @brief This class is the abstract base class for the logger handler callbacks.
 *
 * @sa Log
**/
class GMSEC_API LogHandler
{
public:

	/** @fn ~LogHandler()
	 * @brief Base class needs virtual destructor
	 */
	virtual ~LogHandler() { };

	/** @fn OnMessage(const LogEntry &entry)
	 * @brief This function is called by the API in response to a log message. A class derrived from
	 * gmsec::util::LogHandler needs to be registered with a Logger using registerHandler() in order
	 * to be called for a particular logging level.
	 *
	 *   The prototype for this funtion is:
	 *   @code virtual void CALL_TYPE OnMessage(const LogEntry &entry) @endcode
	 *   The CALL_TYPE macro is required and is there to handle a compatibility
	 *   problem with the Windows linker.
	 *
	 * @note <b>DO NOT DESTROY</b> the LogEntry that is passed into this function by the API.
	 * It is owned by the API and do not need to be managed by the client program. Also, it can
	 * not be stored by the client program beyond the scope of this callback function.
	 *
	 * @param entry - the received log entry
	 */
	virtual void CALL_TYPE OnMessage(const LogEntry &entry) = 0;
};

/** @class Logger
 *
 * @sa Log
**/
class GMSEC_API Logger
{
public:
	Logger();
	~Logger();
	std::ostringstream& Get(LogLevel level = logINFO);
protected:
	std::ostringstream os;
private:
	Logger(const Logger&);
	Logger& operator =(const Logger&);
	LogEntry entry;
	LogHandler *handler;
	void Output(const std::string& msg);
};

/** @class CAPILogHandler
 *
 * @brief This class connects the logging framework to the C API.
 *
**/
class CAPILogHandler : public LogHandler
{
private:
	GMSEC_LOGGER_HANDLER *fHandler;

public:
	CAPILogHandler(GMSEC_LOGGER_HANDLER *handler)
	{
		fHandler = handler;
	}

	virtual void CALL_TYPE OnMessage(const LogEntry &entry)
	{
		if (NULL != fHandler)
		{
			(*fHandler)(&entry);
		}
	}
};

/** @class Log
 *
 * @brief This class defines the logging interface
 *
**/
class GMSEC_API Log
{
public:

	/** @fn SetReportingLevel(LogLevel level)
	 * @brief This function sets the maximum logging level
	 */
	static void SetReportingLevel(LogLevel level);

	/** @fn GetReportingLevel()
	 * @brief This function returns the maximum logging level set by the library or
	 * the maximum logging level set by the user
	 */
	static LogLevel GetReportingLevel();

	/** @fn RegisterHandler(LogHandler *handler)
	 * @brief This function registers a LogHandler that will be called for all logging levels.
	 */
	static void RegisterHandler(LogHandler *handler);

	/** @fn RegisterHandler(LogLevel level, LogHandler *handler)
	 * @brief This function registers a LogHandler that will be called for a specific logging level.
	 */
	static void RegisterHandler(LogLevel level, LogHandler *handler);

	/** @fn ToString(LogLevel level)
	 * @brief This function converts the log level number to a string value
	 */
	static const char* ToString(LogLevel level);

	/** @fn FromString(const char *level)
	 * @brief This function converts the string value to the log level number
	 */
	static LogLevel FromString(const char *level);

	/** @fn setDefaultStream(std::ostream *s)
	 * @brief This function allows the user to change the default stream from stderr.
	 */
	static void setDefaultStream(std::ostream *s);

	/*
	** these are for C API support ONLY
	*/
	static void RegisterHandler(GMSEC_LOGGER_HANDLER *handler);
	static void RegisterHandler(LogLevel level, GMSEC_LOGGER_HANDLER *handler);

private:
	Log() {};
	friend class Logger;
	static LogLevel fmaxLevel;
	static LogHandler* getHandler(LogLevel level);
	static DefaultHandler fdefaultHandler;
};

inline LogLevel Log::GetReportingLevel()
{
	return fmaxLevel;
}


#ifdef LOG_LEVEL_DEFINE
#error "Attempting to redefine the log levels"
#endif
#define LOG_LEVEL_DEFINE

#define LOG(level)							  \
	if ((level <= logNONE) && (level >= logNLEVEL)) ; \
	else if (level > Log::GetReportingLevel()) ; \
	else Logger().Get(level)

#define LOG_ERROR		LOG(logERROR)
#define LOG_WARNING		LOG(logWARNING)
#define LOG_INFO		LOG(logINFO)
#define LOG_VERBOSE		LOG(logVERBOSE)
#define LOG_DEBUG		LOG(logDEBUG)

}	// namespace util
}	// namespace gmsec


#endif /* gmsec_util_Log_h */
