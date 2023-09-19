#ifndef PLT_PLATFORM_H
#define PLT_PLATFORM_H

#include "ALI_defines.h"

typedef struct PLT_PlatformState
{
    void* internalState;
} PLT_PlatformState;

// Call this at the start of the execution in order to
// setup the platform layer.
bool8 PLT_Init(PLT_PlatformState* platformState);

// Call this clean up after our platform
void PLT_Terminate(PLT_PlatformState* platformState);

// Fetch the messages from the platform
bool8 PLT_FetchMessages(PLT_PlatformState* platformState);

// Allocate memory
void* PLT_AllocateMemory(uint64 size, bool8 aligned);

// Free memory
void PLT_FreeMemory(void* block, bool8 aligned);

// Zero memory
void* PLT_ZeroMemory(void* block, uint64 size);

// Copy memory
void* PLT_CopyMemory(void* destination, const void* source, uint64 size);

// Set memory
void* PLT_SetMemory(void* destination, int32 value, uint64 size);

// Function to write to the console
void PLT_ConsoleOut(const char* message, uint8 colour);

// Seperate function to write out as an error (just in case we want seperate stream for errors)
void PLT_ConsoleOutError(const char* message, uint8 colour);

// Get the time according to the platform we're on
float64 PLT_GetAbsoluteTime();

// Call this function to tell the current thread to sleep for the provided number of ms
void PLT_Sleep(uint64 ms);

#endif /* PLT_PLATFORM_H*/