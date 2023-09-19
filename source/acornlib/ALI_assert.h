#ifndef ALI_ASSERT_H
#define ALI_ASSERT_H

#include "ALI_defines.h"

// To disable assertions comment out the line below
#define ACN_ASSERTIONS_ON

#ifdef ACN_ASSERTIONS_ON
    #if _MSC_VER
        #include <intrin.h>
        #define debugBreak() __debugbreak()
    #else
        #define debugBreak() __builtin_trap()
    #endif

    void ALI_ReportAssertionFailure(const char* expression, const char* message, const char* file, int32 line);

    #define ACN_ASSERT(expression)                                                  \
        {                                                                           \
            if(expression)                                                          \
            {                                                                       \
                                                                                    \
            }                                                                       \
            else                                                                    \
            {                                                                       \
                ALI_ReportAssertionFailure(#expression, "", __FILE__, __LINE__);    \
                debugBreak();                                                       \
            }                                                                       \
        }  

    #define ACN_ASSERT_WITH_MESSAGE(expression, message)                            \
    {                                                                               \
        if(expression)                                                              \
        {                                                                           \
                                                                                    \
        }                                                                           \
        else                                                                        \
        {                                                                           \
            ALI_ReportAssertionFailure(#expression, message, __FILE__, __LINE__);   \
            debugBreak();                                                           \
        }                                                                           \
    }   

    #ifdef _DEBUG
        #define ACN_ASSERT_DEBUG(expression)                                        \
        {                                                                           \
            if(expression)                                                           \
            {                                                                       \
                                                                                    \
            }                                                                       \
            else                                                                    \
            {                                                                       \
                ALI_ReportAssertionFailure(#expression, "", __FILE__, __LINE__);    \
                debugBreak();                                                       \
            }                                                                       \
        }  
    #else
        #define ACN_ASSERT_DEBUG(expression)
    #endif
#else
    #define ACN_ASSERT(expression)
    #define ACN_ASSERT_WITH_MESSAGE(expression, message)
    #define ACN_ASSERT_DEBUG(expression)
#endif

#endif /* ALI_ASSERT_H */