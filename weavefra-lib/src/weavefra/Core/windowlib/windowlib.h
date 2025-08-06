#pragma once

#include "WFpch.h"

#include "weavefra/Core/CoreDefined.h"
#include "weavefra/Core/EventBus/EventBus.h"

namespace weavefra
{
    struct WF_API WindowProps
    {
        std::string  title       = "Untitled";   // Window title
        uint32_t     width       = 1280;         // Window width
        uint32_t     height      = 720;          // Window height
        int32_t      posX        = 100;          // Window initial X coordinate
        int32_t      posY        = 100;          // Window initial Y coordinate
        bool         resizable   = true;         // Whether it is resizable
        bool         decorated   = true;         // Whether it has a border and title bar
        bool         isfullscreen  = false;        // Whether it is full screen
        bool         vsync       = true;         // Whether it is vertically synchronized
        bool         transparent = false;        // Whether it is a transparent window
        float        Scaling  = 100;          // DPI scaling percentage                     

        WindowProps(std::string Tit = "Untitled", uint32_t W  = 1280, uint32_t H = 720, int32_t pX = 100, int32_t pY = 100, 
            bool Resize = true, bool Dec = true, bool isfull = false, bool V = true, bool trans = false, uint8_t scaling = 100)
            : title(Tit), width(W), height(H), posX(pX), posY(pY), resizable(Resize), decorated(Dec), isfullscreen(isfull), vsync(V)
            , transparent(trans), Scaling(scaling)
        {

        }
        //... Icon path, multi-monitor designation, initial minimized/maximized state, etc.
    };
    struct WF_API WindowEvent : public Event
    {
        virtual ~WindowEvent() = 0;
    };

    class WF_API window
    {
    public:
        virtual ~window() = default;

        virtual void OnUpdate() = 0;

        virtual uint32_t GetWidth() const = 0;
        virtual uint32_t GetHeight() const = 0;
        virtual void     SetSize(uint32_t width, uint32_t height) = 0;
        
        virtual std::pair<int32_t,int32_t> GetPosition() const = 0;
        virtual void SetPosition(int32_t x, int32_t y) = 0;
        
        virtual void SetTitle(const std::string& title) = 0;
        virtual void SetDecorated(bool decorated) = 0;
        virtual void SetResizable(bool resizable) = 0;

        virtual void Minimize() = 0;
        virtual void Maximize() = 0;
        virtual void Restore()  = 0;
        virtual void Close()    = 0;
        virtual bool IsClosed() const = 0;

        virtual void SetFullscreen(bool fullscreen) = 0;
        virtual bool IsFullscreen() const = 0;
        virtual void SetTransparent(bool transparent) = 0;

        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync() const = 0;

        virtual float GetDpiScaling() const = 0;
        virtual void  SetDpiScaling(float percent) = 0;

        virtual void SetEventCallback(const EventCallbackFn<WindowEvent>& callback) = 0;

        virtual void* GetNativeWindow() const = 0;

    };
}