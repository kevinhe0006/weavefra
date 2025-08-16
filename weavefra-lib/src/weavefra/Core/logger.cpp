#include "logger.h"
#include "Asserts.h"


    std::uint8_t Initialize_logging()
    {
        // log file will been created
        return true;
    }

    void ShoutDown_logging()
    {
        // clean the stuff that need be clean
    }

    WF_API void LogReporter(log_level level, std::string message, ...)
    {
        const char * level_string[6] = { "[FATAL] :", "[ERROR] :", "[WORNING] :", "[INFO] :", "[DEBUG] :", "[TRACE] :" };
        uint8_t is_error = level < 2;

        char out_message[32000];
        std::memset(out_message, 0, sizeof(out_message));

        va_list arg_ptr;
        va_start(arg_ptr, message);
        std::vsnprintf(out_message, 32000, message.c_str(), arg_ptr);
        va_end(arg_ptr);

        char F_out_message[32000];
        std::sprintf(F_out_message, "%s%s\n", level_string[level], out_message);

        std::printf("%s", F_out_message);

        return WF_API void();
    }



