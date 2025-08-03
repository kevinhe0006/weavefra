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