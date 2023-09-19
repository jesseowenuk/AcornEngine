#include "LOG_log.h"

#include "PLT_platform.h"

// TODO: Temporary
#include <stdio.h>
#include <string.h>
#include <stdarg.h>

// Call this to initialise everything we need for logging.
// Must be called before any other logging functions.
bool8 LOG_Init()
{
    // TODO: Create a log file
    return TRUE;
}

// Call this to at the end of your program to close
// down the logger.
void LOG_Terminate()
{
    // TODO: Cleanup after ourselves and write out any entries in the queue
}

// This is the function which will print out the log message
void LOG_PrintMessage(LOG_LogLevel level, const char* message, ...)
{
    const char* LOG_LogLevelTags[6] = { "[FATAL]: ",
                                        "[ERROR]: ",
                                        "[WARN]: ",
                                        "[INFO]: ",
                                        "[DEBUG]: ",
                                        "[TRACE]: "                              
    };
    bool8 LOG_IsError = level < LOG_LEVEL_WARN;

    // This does create a limitation of log entries being no longer than
    // 32k characters - but really do you need more?
    // Avoids needing to worry about dynamically allocating memory which is slow
    char messageToPrint[32000];
    memset(messageToPrint, 0, sizeof(messageToPrint));

    // Format the original message sent in
    __builtin_va_list argumentPointer;
    va_start(argumentPointer, message);
    vsnprintf(messageToPrint, sizeof(messageToPrint), message, argumentPointer);
    va_end(argumentPointer);

    const int32 messageLength = 32000;
    char formattedMessageToPrint[messageLength];
    sprintf(formattedMessageToPrint, "%s%s\n", LOG_LogLevelTags[level], messageToPrint);

    // Platform specific output
    if(LOG_IsError)
    {
        PLT_ConsoleOutError(formattedMessageToPrint, level);
    }
    else
    {
        PLT_ConsoleOut(formattedMessageToPrint, level);
    }
}