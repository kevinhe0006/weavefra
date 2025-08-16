#pragma once

#if !defined(_OS_WINDOWS) && (defined(_WIN32) || defined(_WIN64))
#  define _OS_WINDOWS 1
#endif
#if !defined(_OS_LINUX) && defined(__linux__)
#  define _OS_LINUX 1
#endif
#if !defined(_OS_ANDROID) && defined(__ANDROID__)
#  define _OS_ANDROID 1
#endif
#if !defined(_OS_MACOS) && defined(__APPLE__)
#  include <TargetConditionals.h>
#  if defined(TARGET_OS_IPHONE) && TARGET_OS_IPHONE
#    if !defined(_OS_IOS)
#      define _OS_IOS 1
#    endif
#  else
#    define _OS_MACOS 1
#  endif
#endif

#include <iostream>
#include <functional>
#include <memory>
#include <utility>
#include <cmath>
#include <algorithm>
#include <typeindex>
#include <assert.h>
#include <cstdarg>
#include <cstdio>
#include <variant>
#include <chrono>
#include <format>
#include <thread>

#include <string>
#include <cstring>
#include <string_view>
#include <vector>
#include <unordered_map>
#include <sstream>
#include <unordered_set>
#include <cstdint>
#include <atomic>
#include <stdint.h>
#include <stddef.h>
#include <limits.h>

#if _OS_WINDOWS
// Windows-specific
    #define  _WIN32_WINNT   0x0A00
    #include <exception>
    #include <Windows.h>
    #include <windowsx.h>
    #include <wingdi.h>
    #include <consoleapi.h>
    #include <ConsoleApi3.h>
#elif _OS_LINUX
    // Linux-specific
#elif _OS_MACOS
    // macOS-specific
#elif _OS_IOS
    // iOS-specific
#elif _OS_ANDROID
    // Android-specific
#else
    #error "Unsupported platform"
#endif