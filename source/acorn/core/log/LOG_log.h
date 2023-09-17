#ifndef LOG_LOG_H
#define LOG_LOG_H

#include "defines.h"

// which log levels are on/off
#define LOG_LEVEL_WARN_ON 1
#define LOG_LEVEL_INFO_ON 1
#define LOG_LEVEL_DEBUG_ON 1
#define LOG_LEVEL_TRACE_ON 1

// Now we've turned all the logging levels on....
// turn off debug and trace if we're building for a release
#if ACN_RELEASE == 1
    #define LOG_LEVEL_DEBUG_ON 0
    #define LOG_LEVEL_TRACE_ON 0
#endif


// define the various log levels
typedef enum LOG_logLevel
{
    LOG_LEVEL_FATAL = 0,
    LOG_LEVEL_ERROR = 1,
    LOG_LEVEL_WARN = 2,
    LOG_LEVEL_INFO = 3,
    LOG_LEVEL_DEBUG = 4,
    LOG_LEVEL_TRACE = 5
} LOG_logLevel;

// Call this to initialise everything we need for logging.
// Must be called before any other logging functions.
bool8 LOG_Init();

// Call this to at the end of your program to close
// down the logger.
void LOG_Terminate();

// This is the function which will print out the log message
void LOG_printMessage(LOG_logLevel level, const char* message, ...);

// Use this macro to log a fatal level macro
#ifndef LOG_FATAL
    #define LOG_FATAL(message, ...) LOG_printMessage(LOG_LEVEL_FATAL, message, ##__VA_ARGS__);
#endif

// Use this macro to log an error level message
#ifndef LOG_ERROR
    #define LOG_ERROR(message, ...) LOG_printMessage(LOG_LEVEL_ERROR, message, ##__VA_ARGS__);
#endif

// Use this to log a warn message but only if these are enabled
#if LOG_LEVEL_WARN_ON == 1
    #define LOG_WARN(message, ...) LOG_printMessage(LOG_LEVEL_WARN, message, ##__VA_ARGS__);
#else
    // Does nothing when warnings are turned off
    #define LOG_WARN(message, ...)
#endif

// Use this to log a info message but only if these are enabled
#if LOG_LEVEL_INFO_ON == 1
    #define LOG_INFO(message, ...) LOG_printMessage(LOG_LEVEL_INFO, message, ##__VA_ARGS__);
#else
    // Does nothing when warnings are turned off
    #define LOG_INFO(message, ...)
#endif

// Use this to log a debug message but only if these are enabled
#if LOG_LEVEL_DEBUG_ON == 1
    #define LOG_DEBUG(message, ...) LOG_printMessage(LOG_LEVEL_DEBUG, message, ##__VA_ARGS__);
#else
    // Does nothing when warnings are turned off
    #define LOG_DEBUG(message, ...)
#endif

// Use this to log a trace message but only if these are enabled
#if LOG_LEVEL_TRACE_ON == 1
    #define LOG_TRACE(message, ...) LOG_printMessage(LOG_LEVEL_TRACE, message, ##__VA_ARGS__);
#else
    // Does nothing when warnings are turned off
    #define LOG_TRACE(message, ...)
#endif

#endif /* LOG_LOG_H */