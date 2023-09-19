#include "ALI_assert.h"
#include "LOG_log.h"

void ALI_ReportAssertionFailure(const char* expression, const char* message, const char* file, int32 line)
{
    LOG_PrintMessage(LOG_LEVEL_FATAL, "Assertion failure: %s, message: '%s', in file: '%s', line: %d\n", expression, message, file, line);
}