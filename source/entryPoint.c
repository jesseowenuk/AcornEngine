#include "ALI_assert.h"

#include "LOG_log.h"

#include "MEM_memory.h"

// TODO: Test
#include "PLT_platform.h"

/**
 * Acorn's main entry point
*/
int main(void)
{
    // Lets get initialised
    MEM_Init();
    LOG_Init();
    
    // TODO: Remove this testing code
    LOG_FATAL("Testing Acorn's Logging: %f", 3.14159f);
    LOG_ERROR("Testing Acorn's Logging: %f", 3.14159f);
    LOG_WARN("Testing Acorn's Logging: %f", 3.14159f);
    LOG_INFO("Testing Acorn's Logging: %f", 3.14159f);
    LOG_DEBUG("Testing Acorn's Logging: %f", 3.14159f);
    LOG_TRACE("Testing Acorn's Logging: %f", 3.14159f);

    PLT_PlatformState state;

    if(PLT_Init(&state))
    {
        bool8 acornRunning = TRUE;

        LOG_INFO(MEM_GetUsageInfo());

        while(acornRunning)
        {
            if(!PLT_FetchMessages(&state))
            {
                acornRunning = FALSE;
            }
        }

        acornRunning = FALSE;
    }

    // And we're done - terminate everything!
    PLT_Terminate(&state);
    LOG_Terminate();
    MEM_Terminate();

    return 0;
}