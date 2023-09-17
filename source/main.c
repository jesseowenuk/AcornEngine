#include "LOG_log.h"

int main(void)
{
    LOG_FATAL("Testing Acorn's Logging: %f", 3.14159f);
    LOG_ERROR("Testing Acorn's Logging: %f", 3.14159f);
    LOG_WARN("Testing Acorn's Logging: %f", 3.14159f);
    LOG_INFO("Testing Acorn's Logging: %f", 3.14159f);
    LOG_DEBUG("Testing Acorn's Logging: %f", 3.14159f);
    LOG_TRACE("Testing Acorn's Logging: %f", 3.14159f);

    return 0;
}