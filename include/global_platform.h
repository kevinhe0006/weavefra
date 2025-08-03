#pragma once

// 平台检测
#if defined(_WIN)
    #define PLATFORM_WINDOWS 1
    #define PLATFORM_UNIX 0
#elif defined(_UNIX)
    #define PLATFORM_WINDOWS 0
    #define PLATFORM_UNIX 1
#endif

// 调试模式检测
#ifdef _DEBUG
    #define DEBUG_MODE 1
    #define DEBUG_LOG(msg) std::cerr << "[DEBUG] " << msg << std::endl
#else
    #define DEBUG_MODE 0
    #define DEBUG_LOG(msg)
#endif

