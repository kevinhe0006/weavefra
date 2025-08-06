#pragma once

#include "WFpch.h"

#include <GLFW/glfw3.h>
#include "weavefra/Core/windowlib/windowlib.h"

namespace weavefra
{

    class WF_API WindowsWindow;
    struct WF_API WindowsWindowEvent;

    
    class WF_API WindowsWindow : public window
    {
    private:
        WindowProps PropsDate;

        
    public:

        WindowsWindow();
        ~WindowsWindow();

        void OnUpdate() override;
        uint32_t GetWidth() const override;
        uint32_t GetHeight() const override;
        void     SetSize(uint32_t width, uint32_t height) override;
        std::pair<int32_t,int32_t> GetPosition() const override;
        void SetPosition(int32_t x, int32_t y) override;
        void SetTitle(const std::string& title) override;
        void SetDecorated(bool decorated) override;
        void SetResizable(bool resizable) override;
        void Minimize() override;
        void Maximize() override;
        void Restore()  override;
        void Close()    override;
        bool IsClosed() const override;
        void SetFullscreen(bool fullscreen) override;
        bool IsFullscreen() const override;
        void SetTransparent(bool transparent) override;
        void SetVSync(bool enabled) override;
        bool IsVSync() const override;
        float GetDpiScaling() const override;
        void  SetDpiScaling(float percent) override;
        void SetEventCallback(const EventCallbackFn<WindowEvent>& callback) override;
        void* GetNativeWindow() const override;                                                  
        static std::unique_ptr<window> Create(const WindowProps& props);
    };

    struct WF_API WindowsWindowEvent : public WindowEvent
    {
        const uint16_t WindowID;
        WindowsWindow& Window;
        WindowsWindowEvent(uint16_t id, WindowsWindow& window) : WindowID(id), Window(window){}
        ~WindowsWindowEvent(){};
    };
}