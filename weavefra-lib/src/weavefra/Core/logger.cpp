#include "logger.h"
#include "weavefra/platform/platform.h"


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
        const u8     level_color[6][3] = { {255, 75, 75}, { 225, 125, 100 }, { 200, 160 , 130 }, { 170, 170, 170 }, { 100, 75, 157 }
        , { 75, 152, 125 } };
        uint8_t is_error = level < 2;

        char out_message[32000];
        std::memset(out_message, 0, sizeof(out_message));

        va_list arg_ptr;
        va_start(arg_ptr, message);
        std::vsnprintf(out_message, 32000, message.c_str(), arg_ptr);
        va_end(arg_ptr);

        char F_out_message[32000];
        std::sprintf(F_out_message, "%s%s\n", level_string[level], out_message);

        Platfrom_Console::consoleWrite(F_out_message, level_color[level][0], level_color[level][1], level_color[level][2], false);

    }



