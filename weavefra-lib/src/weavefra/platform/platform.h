/* platform.h — Minimal, extensible, and C-compatible platform API */
#pragma once


#include "WFpch.h"

#include "weavefra/Core/CoreDefined.h"
#include "weavefra/Core/EventBus/EventBus.h"
#include "weavefra/Core/logger.h"
#include "weavefra/Core/Asserts.h"

#ifndef PLATFORM_H
#define PLATFORM_H

/* ======== Basic types ========================================================= */

typedef uint8_t   u8;
typedef int8_t    s8;
typedef uint16_t  u16;
typedef int16_t   s16;
typedef uint32_t  u32;
typedef int32_t   s32;
typedef uint64_t  u64;
typedef int64_t   s64;

typedef uintptr_t uptr;
typedef intptr_t  sptr;

#ifndef PLATFORM_NULL
  #define PLATFORM_NULL ((void*)0)
#endif

/* API versioning */
#define PLATFORM_VERSION_MAJOR 0
#define PLATFORM_VERSION_MINOR 1
#define PLATFORM_VERSION_PATCH 0

/* Infinite timeout helper */
#define PLATFORM_WAIT_INFINITE 0xFFFFFFFFu

/* ======== Error codes ========================================================= */

struct WF_API platform_state;


enum Platform_Error
{
    PLATFORM_OK = 0,
    PLATFORM_ERR_UNKNOWN         = -1,
    PLATFORM_ERR_INVALID_ARG     = -2,
    PLATFORM_ERR_NOMEM           = -3,
    PLATFORM_ERR_IO              = -4,
    PLATFORM_ERR_NOT_FOUND       = -5,
    PLATFORM_ERR_BUSY            = -6,
    PLATFORM_ERR_TIMEOUT         = -7,
    PLATFORM_ERR_WOULD_BLOCK     = -8,
    PLATFORM_ERR_NOT_SUPPORTED   = -9,
    PLATFORM_ERR_DISCONNECTED    = -10,
};

/* --- Lifecycle --------------------------------------------------------------- */

typedef struct WindowDesc
{
    uint32_t width = 1280, height = 720;
    const char* title = "window";
    bool resizable = true;
    bool visible = true;
    enum ContextType { OpenGL, Vulkan, Directx, Metal, GNMX, NVN};
    WindowDesc(u32 w = 1280, u32 h = 720, const char* t = "window", bool r = true, bool v = true)
        : width(w), height(h), title(t), resizable(r), visible(v){}
} WF_API WindowDesc;

struct WF_API InitOptions
{
    const char* app_name = "app";
    bool high_dpi_awareness = true;
    WindowDesc Window_Desc;
};

WF_API platform_state* Platform_Init(const InitOptions& = {});        /* Optional: Initialize the global subsystem */  
WF_API Platform_Error  Platform_Shutdown(platform_state *plat_state);    /* Release global resources */  

typedef struct Platform_File_State
{
    void* File_State;
} Platform_File_State;
class WF_API Platform_File
{
private:
    std::string File_path;
    Platform_File_State state;
public:
    Platform_File() = default;
    ~Platform_File() = default;

    void OpenFile();
    void ReadFile();
    void WriteFile();
    void CloseFile();
};

/* --- consle ------------------------------------------------------------------- */
typedef struct Platfrom_Console_State
{
    void* consle_state;
} Platfrom_Console_State;
class WF_API Platfrom_Console
{
private:
    Platfrom_Console_State state;
public:
    Platfrom_Console();
    ~Platfrom_Console();

    void Platform_ConsoleWrite(std::string message, u8 r, u8 g, u8 b);
};

WF_API void* platform_allocate(u64, bool aligned);
WF_API void  platform_free(void* block, bool aligned);
WF_API void* platform_copy_memory(void *dest, const void* source, u64 size);
WF_API void* platform_set_memory(void *dest, s32 value, u64 size); 

class WF_API Platform_Clock
{
private:
    using steady_clock  = std::chrono::steady_clock;
    using system_clock  = std::chrono::system_clock;
    using time_point    = steady_clock::time_point;

    time_point Timestamp_;
    system_clock::time_point system_time_point;
public:
    Platform_Clock();
    explicit Platform_Clock(time_point tp) noexcept
        : Timestamp_(tp), system_time_point(system_clock::now()) {}
    ~Platform_Clock() = default;

    double GetNowTime() const noexcept
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                   steady_clock::now().time_since_epoch()
               ).count();
    }
    double GetTimeDifferent(double t_ms) const noexcept
    {
        return GetNowTime() - t_ms;
    }
    double GetTimeDifferentFromTimestamp() const noexcept
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                   steady_clock::now() - Timestamp_
               ).count();
    }
    double ResetTimestamp() noexcept
    {
        const double elapsed = GetTimeDifferentFromTimestamp();
        Timestamp_           = steady_clock::now();
        system_time_point    = system_clock::now();
        return elapsed;
    }
    double GetSystemNowTime() const noexcept
    {
        return std::chrono::duration_cast<std::chrono::milliseconds>(
           system_clock::now().time_since_epoch()
        ).count();
    }

    time_point GetTimestamp() const noexcept { return Timestamp_; }

    static Platform_Clock now() noexcept { return Platform_Clock(steady_clock::now()); }
    

    static Platform_Clock from_unix_millis(double ms) noexcept
    {
        system_clock::time_point target_sys =
            std::chrono::time_point<system_clock, std::chrono::milliseconds>(
                std::chrono::milliseconds(static_cast<long long>(std::llround(ms)))
            );
        const auto now_sys    = std::chrono::system_clock::now();
        const auto now_steady = std::chrono::steady_clock::now();

        const auto delta      = target_sys - now_sys;

        Platform_Clock c;
        c.Timestamp_         = now_steady + delta;
        c.system_time_point  = target_sys;         
        return c;
    }

    double to_unix_millis() const noexcept
    {
        const auto now_sys    = std::chrono::system_clock::now();
        const auto now_steady = std::chrono::steady_clock::now();
        const auto delta      = Timestamp_ - now_steady;      
        const auto mapped_sys = now_sys + delta;       
        return std::chrono::duration_cast<std::chrono::milliseconds>(
                   mapped_sys.time_since_epoch()
               ).count();
    }

    void SleepMs(double ms)
    {
        if(ms <= 0.0f) { return; }
        std::this_thread::sleep_for(std::chrono::duration<double, std::milli>(ms));
    }
};

struct WF_API EventKey
{
    uint32_t key;
    bool down;
    bool repeat;
};
struct WF_API EventMouse
{
    int32_t x, y;       
    int32_t dx, dy;     
    uint8_t button;     
    bool down;
    float wheel;        
};
struct WF_API EventWindow
{
    enum class EventWindowType { Create, Close, Resize, FocusIn, FocusOut, Move, Minimize, Restore } type;
    uint32_t width, height;
};
enum class platform_EventType : uint8_t { None, Key, Mouse, Window, TextInput, DropFile };
struct WF_API platform_Event : weavefra::Event
{
    platform_EventType type = platform_EventType::None;
    union
    {
        EventKey   key;
        EventMouse mouse;
        EventWindow window;
    };
};

typedef struct Window_State
{
    void* Window_internal_State;
} Window_State;

class WF_API Platform_Window
{
private:
    WindowDesc windowdesc;
    platform_Event event{};
public:
    Window_State state{nullptr};
    bool WindowExist = false;

    Platform_Window() noexcept;
    explicit Platform_Window(WindowDesc);
    ~Platform_Window();

    Platform_Window(const Platform_Window&) = delete;
    Platform_Window& operator=(const Platform_Window&) = delete;
    Platform_Window(Platform_Window&&) noexcept;
    Platform_Window& operator=(Platform_Window&&) noexcept;

    Platform_Window(Platform_Window&) = delete;
    Platform_Window operator&= (Platform_Window) = delete;
    bool ProcessMessages();
    void WSwapBuffers() const;
    void Show_Platform_Window(bool Should_activate = true) const;
};

class WF_API Platform_Thread;

class WF_API Platform_Mutex;

class WF_API Platform_Semaphore;

class WF_API Platform_CondVar;

class WF_API Platform_Socket;

class WF_API Platform_Library;

struct WF_API platform_state
{
    Platform_Clock Clock;
    Platfrom_Console console;
    Platform_Window window;
    Platform_Error Result = PLATFORM_OK;
};

#endif

#ifdef __cplusplus
    extern "C"
    {      
#endif
        //...
#ifdef __cplusplus
    }
#endif