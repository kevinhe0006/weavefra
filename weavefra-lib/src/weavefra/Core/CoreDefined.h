#pragma once
#include "global_platform.h"

#if PLATFORM_WINDOWS
    #if defined(WEAVEFRA_STATIC)
      #define WF_API
    #elif defined(_WIN32) && defined(_BUILDING_DLL)
      #define WF_API __declspec(dllexport)
    #elif defined(_WIN32) && defined(_USING_DLL)
      #define WF_API __declspec(dllimport)
    #else
      #define WF_API
    #endif
#else
    #define WF_API
#endif

#ifndef WF_Defined_ShouldEnd
    #define WF_Defined_ShouldEnd

    inline bool ShouldEnd = false;

#endif
