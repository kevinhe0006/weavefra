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

#if (defined(WF_USE_OPENGL) + defined(WF_USE_DIRECT3D) + defined(WF_USE_VULKAN)) > 1
    #error"Only one of the interfaces can be defined: WF USE OPENGL, WF USEr DIRECT3D, WF USE_VULKAN"
#endif

#if !defined(WF_USE_OPENGL) && !defined(WF_USE_DIRECT3D) && !defined(WF_USE_VULKAN)
    #define WF_USE_OPENGL
#endif

#ifdef WF_USE_OPENGL
    typedef enum {
        WF_GPU_OPENGL,
        WF_GPU_DIRECT3D,
        WF_GPU_VULKAN
    } WF_GPU_API_TYPE;
    inline WF_GPU_API_TYPE WF_USE_GPU_API = WF_GPU_OPENGL;

#elif defined(WF_USE_DIRECT3D)
    typedef enum {
        WF_GPU_OPENGL,
        WF_GPU_DIRECT3D,
        WF_GPU_VULKAN
    } WF_GPU_API_TYPE;
    inline WF_GPU_API_TYPE WF_USE_GPU_API = WF_USE_DIRECT3D;

#elif defined(WF_USE_VULKAN)
    typedef enum {
        WF_GPU_OPENGL,
        WF_GPU_DIRECT3D,
        WF_GPU_VULKAN
    } WF_GPU_API_TYPE;
    inline WF_GPU_API_TYPE WF_USE_GPU_API = WF_GPU_VULKAN;
#else
    #define WF_USE_OPENGL
    typedef enum {
        WF_GPU_OPENGL,
        WF_GPU_DIRECT3D,
        WF_GPU_VULKAN
    } WF_GPU_API_TYPE;
    inline WF_GPU_API_TYPE WF_USE_GPU_API = WF_GPU_OPENGL;

#endif
