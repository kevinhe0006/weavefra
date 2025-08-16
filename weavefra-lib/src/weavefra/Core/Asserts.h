#pragma once

#include "WFpch.h"

#include "CoreDefined.h"
#include "logger.h"

#define WF_ASSERTS_ENABLED

#ifdef WF_ASSERTS_ENABLED
#if _MSC_VER
#include <intrin.h>
#define DebugBreak() __debugbreak()
#else
#define DebugBreak() __builtin_trap()
#endif

WF_API void report_assertion_failer(std::string expression, std::string message, const char* file, int32_t line);

#define WF_ASSERT(expr, ...)                                                       \
    do {                                                                           \
        if (expr){}                                                                \
        else{                                                                      \
            report_assertion_failer(#expr, ##__VA_ARGS__, __FILE__, __LINE__ );    \
            DebugBreak();                                                          \
        }                                                                          \
    } while (false)

#ifdef _DEBUG
    #define WF_DEBUG_ASSERT(expr, ...)                                             \
    do {                                                                           \
        if (expr){}                                                                \
        else{                                                                      \
            report_assertion_failer(#expr, ##__VA_ARGS__, __FILE__, __LINE__ );    \
            DebugBreak();                                                          \
        }                                                                          \
    } while (false)
#else
    #define WF_DEBUG_ASSERT(expr, ...)
#endif
#else
    #define WF_ASSERT(expr, ...)
#endif