#include "Asserts.h"

WF_API void report_assertion_failer(std::string expression, std::string message, const char *file, int32_t line)
{
    WF_ERROR_LOG("==================== ASSERTION FAILED ====================");
    WF_ERROR_LOG("  Expression: %s" , expression.c_str() );
    WF_ERROR_LOG("  Location  : %s : %d", file, line );
    if (message.c_str() && *message.c_str())
    {
        WF_ERROR_LOG( "  Message   : %s", message.c_str());
    }
    WF_ERROR_LOG("==========================================================");
}
