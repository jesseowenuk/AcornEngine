#ifndef DEFINES_H
#define DEFINES_H

// Unsigned int types
typedef unsigned char uint8;
typedef unsigned short uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

// Signed in types
typedef signed char int8;
typedef signed short int16;
typedef signed int int32;
typedef signed long long int64;

// Floating point types
typedef float float32;
typedef double float64;

// Boolean types
typedef char bool8;
typedef int bool32;

// Properly define static assertions
#if defined(__clang__) || defined(__gcc__)
    #define STATIC_ASSERT _Static_assert
#else
    #define STATIC_ASSERT static_assert
#endif

// Ensure all the types are the correct size
STATIC_ASSERT(sizeof(uint8) == 1, "Expected uint8 to be 1 byte.");
STATIC_ASSERT(sizeof(uint16) == 2, "Expected uint8 to be 2 bytes.");
STATIC_ASSERT(sizeof(uint32) == 4, "Expected uint8 to be 8 byte.");
STATIC_ASSERT(sizeof(uint64) == 8, "Expected uint8 to be 8 bytes.");

STATIC_ASSERT(sizeof(int8) == 1, "Expected int8 to be 1 byte.");
STATIC_ASSERT(sizeof(int16) == 2, "Expected int8 to be 2 bytes.");
STATIC_ASSERT(sizeof(int32) == 4, "Expected int8 to be 8 byte.");
STATIC_ASSERT(sizeof(int64) == 8, "Expected int8 to be 8 bytes.");

STATIC_ASSERT(sizeof(float32) == 4, "Expected uint8 to be 4 bytes.");
STATIC_ASSERT(sizeof(float64) == 8, "Expected uint8 to be 8 bytes.");

#define FALSE 0
#define TRUE 1

// Lets detect the platform
#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__)
    // We're running on Windows!
    #define ACN_PLATFORM_WINDOWS 1

    #ifndef _WIN64
        #error "64-bit is required on Windows!"
    #endif
#elif defined(__linux__) || defined(__gnu_linux__)
    // We're running on Linux!
    #define ACN_PLATFORM_LINUX 1
#elif defined(__unix__)
    // somehow we're running on Unix!
    #define ACN_PLATFORM_UNIX 1
#elif defined(_POSIX_VERSION)
    // Okay we're on POSIX
    #define ACN_PLATFORM_POSIX 1
#elif __APPLE__
    // We don't know which one yet but we're in the Apple ecosystem!
    #define ACN_PLATFORM_APPLE 1
    #include <TargetConditionals.h>
#endif

#endif /* DEFINE_H */