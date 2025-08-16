#pragma once

// 平台检测
#if defined(_OS_WINDOWS)
    #define PLATFORM_WINDOWS 1
    #define PLATFORM_UNIX 0
#elif defined(_OS_LINUX)
    #define PLATFORM_WINDOWS 0
    #define PLATFORM_UNIX 1
#endif

// 调试模式检测
#ifdef _DEBUG
    #define DEBUG_MODE 1
#else
    #define DEBUG_MODE 0
#endif

