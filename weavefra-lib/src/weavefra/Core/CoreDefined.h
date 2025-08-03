#pragma once
#include "global_platform.h"

#if PLATFORM_WINDOWS
    #if defined(_BUILDING_DLL)
        #define WF_API __declspec(dllexport)
    #else
        #define WF_API __declspec(dllimport)
    #endif
#else
    #define WF_API
#endif

#ifndef WF_Defined_ShouldEnd
#define WF_Defined_ShouldEnd

inline bool ShouldEnd = false;

#endif