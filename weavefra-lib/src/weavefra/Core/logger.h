#pragma once

#include "WFpch.h"

#include "CoreDefined.h"

    #ifndef WF_LOGGER
        #define WF_LOGGER

        #define LOG_FATAL_ENABLED   1
        #define LOG_ERROR_ENABLED   1
        #define LOG_WORNING_ENABLED 1
        #define LOG_INFO_ENABLED    1
        #define LOG_DEBUG_ENABLED   1
        #define LOG_TRACE_ENABLED   1

        typedef enum log_level {
            LOG_FATAL_LEVEL     = 0,
            LOG_ERROR_LEVEL     = 1,
            LOG_WORNING_LEVEL   = 2,
            LOG_INFO_LEVEL      = 3,
            LOG_DEBUG_LEVEL     = 4,
            LOG_TRACE_LEVEL     = 5
        } log_level;

        std::uint8_t Initialize_logging();
        void ShoutDown_logging();

        WF_API void LogReporter(log_level level, std::string message, ...);

        #ifdef WF_RELEASE
            #define LOG_DEBUG_ENABLED 0
            #define LOG_TRACE_ENABLED 0
        #endif
        #if LOG_FATAL_ENABLED == 1
            #ifndef WF_FATAL_LOG
                #define WF_FATAL_LOG(message, ...) LogReporter(log_level::LOG_FATAL_LEVEL, message, ##__VA_ARGS__)
            #endif
        #else
            #define WF_FATA_LOG(message, ...)
        #endif
        #if LOG_ERROR_ENABLED == 1
            #ifndef WF_ERROR_LOG
                #define WF_ERROR_LOG(message, ...) LogReporter(log_level::LOG_ERROR_LEVEL, message, ##__VA_ARGS__)
            #endif
        #else
            #define WF_ERROR_LOG(message, ...)
        #endif
        #if LOG_WORNING_ENABLED == 1
            #ifndef WF_WORNING_LOG
                #define WF_WORNING_LOG(message, ...) LogReporter(log_level::LOG_WORNING_LEVEL, message, ##__VA_ARGS__)
            #endif
        #else
            #define WF_WORNING_LOG(message, ...)
        #endif
        #if LOG_INFO_ENABLED == 1
            #ifndef WF_INFO_LOG
                #define WF_INFO_LOG(message, ...) LogReporter(log_level::LOG_INFO_LEVEL, message, ##__VA_ARGS__)
            #endif
        #else
            #define WF_INFO_LOG(message, ...)
        #endif
        #if LOG_DEBUG_ENABLED == 1
            #ifndef WF_DEBUG_LOG
                #define WF_DEBUG_LOG(message, ...) LogReporter(log_level::LOG_DEBUG_LEVEL, message, ##__VA_ARGS__)
            #endif
        #else
            #define WF_DEBUG_LOG(message, ...)
        #endif
        #if LOG_TRACE_ENABLED == 1
            #ifndef WF_TRACE_LOG
                #define WF_TRACE_LOG(message, ...) LogReporter(log_level::LOG_TRACE_LEVEL, message, ##__VA_ARGS__)
            #endif
        #else
            #define WF_TRACE_LOG(message, ...)
        #endif
    #endif

