#include "WindowsWindow.h"

namespace weavefra
{
    WindowsWindow::WindowsWindow()
    {
        
    }

    WindowsWindow::~WindowsWindow()
    {
    }

    void WindowsWindow::OnUpdate()
    {
    }
    uint32_t WindowsWindow::GetWidth() const
    {
        return 0;
    }
    uint32_t WindowsWindow::GetHeight() const
    {
        return 0;
    }
    void WindowsWindow::SetSize(uint32_t width, uint32_t height)
    {
    }
    std::pair<int32_t, int32_t> WindowsWindow::GetPosition() const
    {
        return std::pair<int32_t, int32_t>();
    }
    void WindowsWindow::SetPosition(int32_t x, int32_t y)
    {
    }
    void WindowsWindow::SetTitle(const std::string &title)
    {
    }
    void WindowsWindow::SetDecorated(bool decorated)
    {
    }
    void WindowsWindow::SetResizable(bool resizable)
    {
    }
    void WindowsWindow::Minimize()
    {
    }
    void WindowsWindow::Maximize()
    {
    }
    void WindowsWindow::Restore()
    {
    }
    void WindowsWindow::Close()
    {
    }
    bool WindowsWindow::IsClosed() const
    {
        return false;
    }
    void WindowsWindow::SetFullscreen(bool fullscreen)
    {
    }
    bool WindowsWindow::IsFullscreen() const
    {
        return false;
    }
    void WindowsWindow::SetTransparent(bool transparent)
    {
    }
    void WindowsWindow::SetVSync(bool enabled)
    {
    }
    bool WindowsWindow::IsVSync() const
    {
        return false;
    }
    float WindowsWindow::GetDpiScaling() const
    {
        return 0.0f;
    }
    void WindowsWindow::SetDpiScaling(float percent)
    {
    }
    void WindowsWindow::SetEventCallback(const EventCallbackFn<WindowEvent> &callback)
    {
    }
    void *WindowsWindow::GetNativeWindow() const
    {
        return nullptr;
    }
    std::unique_ptr<window> WindowsWindow::Create(const WindowProps &props)
    {
        if(WF_USE_GPU_API == WF_GPU_API_TYPE::WF_GPU_OPENGL)
        {
            
        }
        else
        {
            
        }
        return std::unique_ptr<WindowsWindow>();
    }
}