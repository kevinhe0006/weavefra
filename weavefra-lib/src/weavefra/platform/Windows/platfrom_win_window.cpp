#include "weavefra/platform/platform.h"
#include "weavefra/Core/logger.h"
#include "weavefra/Core/EventBus/GlobalEventBus.h"

#if _OS_WINDOWS
    typedef struct Window_internal_State
    {
        HWND m_hwnd;
    	HINSTANCE m_hinstance;
    	HDC m_hdc;
    } Window_internal_State;

    LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	Platform_Window::Platform_Window() noexcept = default;

    Platform_Window::Platform_Window(WindowDesc w)
        : windowdesc(w)
    {
        state.Window_internal_State = std::malloc(sizeof(Window_internal_State));
        Window_internal_State *State = static_cast<Window_internal_State *>(state.Window_internal_State);
		if(!State) { WF_ERROR_LOG("no window state , at : %s : %s", __FILE__, __LINE__ ); }
		std::memset(State, 0, sizeof(*State));

        State->m_hinstance = GetModuleHandleA(0);

    	const char class_name[] = "M_window_class";

    	WNDCLASSEXA m_windowclass = { };

    	m_windowclass.lpfnWndProc = WindowProc;
    	m_windowclass.hInstance = State->m_hinstance;
    	m_windowclass.lpszClassName = class_name;
    	m_windowclass.hCursor = LoadCursor(NULL, IDC_ARROW);
    	m_windowclass.hIcon = LoadIcon(NULL, IDI_WINLOGO);
    	m_windowclass.cbSize = sizeof(WNDCLASSEX);
    	m_windowclass.cbClsExtra = DLGWINDOWEXTRA;
    	m_windowclass.style = CS_OWNDC;
		m_windowclass.cbClsExtra = 0;
		m_windowclass.cbWndExtra = 0;
		m_windowclass.hbrBackground = nullptr;

    	if(!RegisterClassExA(&m_windowclass)) { MessageBoxA(0, "Windows RegisterClass fail", "Error", MB_ICONEXCLAMATION | MB_OK); 
			WF_ERROR_LOG("RegisterClassExA failed (class may already exist).");}

		u32	window_style = WS_OVERLAPPEDWINDOW | WS_SYSMENU | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_OVERLAPPEDWINDOW 
        | WS_VISIBLE | CS_DBLCLKS;
		window_style |= WS_THICKFRAME;
		window_style |= WS_MAXIMIZE;
		window_style |= WS_MINIMIZE;
		DWORD exstyle = WS_EX_OVERLAPPEDWINDOW;
		RECT serface_rect = {0, 0, 0, 0};
		AdjustWindowRectEx(&serface_rect, window_style, 0, exstyle);

		int WX = serface_rect.left;
		int WY = serface_rect.top;

		windowdesc.width += serface_rect.right - serface_rect.left;
		windowdesc.height += serface_rect.bottom - serface_rect.top;

    	State->m_hwnd = CreateWindowExA(
    		exstyle,
    		class_name,
    		windowdesc.title,
    		window_style,
    		CW_USEDEFAULT, CW_USEDEFAULT,
    		windowdesc.width,
    		windowdesc.height,
    		NULL,
    		NULL,
    		State->m_hinstance,
    		NULL
    	);

    	if (State->m_hwnd == NULL)
    	{
    		MessageBoxExA(0, "Windows handle init fail", "Error", MB_ICONEXCLAMATION | MB_OK
					, MAKELANGID(LANG_NEUTRAL, SUBLANG_NEUTRAL));
			WF_FATAL_LOG("Window create fail");
			return;
    	}

		event.type = platform_EventType::Window;
		event.window.height = windowdesc.height;
		event.window.width = windowdesc.width;
		event.window.type = EventWindow::EventWindowType::Create;
        
        weavefra::GlobalEventBus::emit(event);

    	PIXELFORMATDESCRIPTOR pfd =
    	{
    		sizeof(PIXELFORMATDESCRIPTOR),
    		1,
    		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,    // Flags
    		PFD_TYPE_RGBA,        // The kind of framebuffer. RGBA or palette.
    		32,                   // Colordepth of the framebuffer.
    		0, 0, 0, 0, 0, 0,
    		0,
    		0,
    		0,
    		0, 0, 0, 0,
    		24,                   // Number of bits for the depthbuffer
    		8,                    // Number of bits for the stencilbuffer
    		0,                    // Number of Aux buffers in the framebuffer.
    		PFD_MAIN_PLANE,
    		0,
    		0, 0, 0
    	};


    	State->m_hdc = GetDC(State->m_hwnd);

    	int m_format = ChoosePixelFormat(State->m_hdc, &pfd);
    	SetPixelFormat(State->m_hdc, m_format, &pfd);

    	WindowExist = true;
    }

    Platform_Window::~Platform_Window()
    {
        Window_internal_State *State = static_cast<Window_internal_State *>(state.Window_internal_State);
		if(!State) { return ; }
    	MessageBoxA(0, "byby?", "Info", MB_ICONEXCLAMATION | MB_OK | MB_HELP);
		DestroyWindow(State->m_hwnd);
    	UnregisterClassA("M_window_class", State->m_hinstance);
    	ReleaseDC(State->m_hwnd, State->m_hdc);
		std::free(state.Window_internal_State);
		state.Window_internal_State = nullptr;
    }

    bool Platform_Window::ProcessMessages()
    {
        Window_internal_State *State = static_cast<Window_internal_State *>(state.Window_internal_State);
		if(!State) { WF_ERROR_LOG("no window state, at : %s : %s", __FILE__, __LINE__ ); return false; }
    	MSG msg = { };
    	while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
    	{
    		if (msg.message == WM_QUIT)
    		{
    			WindowExist = false;
    			return false;
    		}
    		TranslateMessage(&msg);
    		DispatchMessage(&msg);
    	}
    	return true;
    }

    void Platform_Window::WSwapBuffers() const
    {
        Window_internal_State *State = static_cast<Window_internal_State *>(state.Window_internal_State);
		if(!State) { WF_ERROR_LOG("no window state, at : %s : %s", __FILE__, __LINE__ ); }
    	::SwapBuffers(State->m_hdc);
    }

    void Platform_Window::Show_Platform_Window(bool should_activate) const
    {
        Window_internal_State *State = static_cast<Window_internal_State *>(state.Window_internal_State);
		if(!State) { WF_ERROR_LOG("no window state, at : %s : %s", __FILE__, __LINE__ ); }
		ShowWindow(State->m_hwnd, (should_activate ? SW_SHOW : SW_SHOWNOACTIVATE));
		UpdateWindow(State->m_hwnd);
    }
	Platform_Window::Platform_Window(Platform_Window&& o) noexcept { *this = std::move(o); }
	Platform_Window& Platform_Window::operator=(Platform_Window&& o) noexcept
	{
		if (this == &o) return *this;
		windowdesc  = o.windowdesc;
		event       = o.event;
		state       = o.state;
		WindowExist = o.WindowExist;
		o.state.Window_internal_State = nullptr;
		o.WindowExist = false;
		return *this;
	}


    enum class MB_BUTTON : u8
    {
        MB_None   = 0,
        MB_Left   = 1,
        MB_Right  = 2,
        MB_Middle = 3,
        MB_X1     = 4,
        MB_X2     = 5,
    };

static bool   s_hasLastMouse = false;
static int32_t s_lastX = 0, s_lastY = 0;

enum class MB : int { Left, Right, Middle, X1, X2 };

static inline void ToMathCoords(HWND hwnd, int cx, int cy, int& outX, int& outY, uint32_t* optW = nullptr, uint32_t* optH = nullptr)
{
    RECT rc{};
    GetClientRect(hwnd, &rc);
    const int w = rc.right - rc.left;
    const int h = rc.bottom - rc.top;
    if (optW) *optW = (w > 0) ? (uint32_t)w : 0;
    if (optH) *optH = (h > 0) ? (uint32_t)h : 0;

    outX = cx - w / 2;
    outY = (h / 2) - cy;
}

static inline void EmitMouseMove(HWND hwnd, int cx, int cy)
{
    platform_Event e{};
    e.type = platform_EventType::Mouse;

    int mx, my; uint32_t W, H;
    ToMathCoords(hwnd, cx, cy, mx, my, &W, &H);

    e.mouse.x = mx;
    e.mouse.y = my;

    if (!s_hasLastMouse)
    {
        e.mouse.dx = 0;
        e.mouse.dy = 0;
        s_hasLastMouse = true;
    }
    else
    {
        e.mouse.dx = mx - s_lastX;
        e.mouse.dy = my - s_lastY;
    }
    s_lastX = mx; s_lastY = my;

    e.mouse.button = u8(MB_BUTTON::MB_None);
    e.mouse.down   = false;
    e.mouse.wheel  = 0.0f;

    weavefra::GlobalEventBus::emit(e);
}

static inline void EmitMouseButton(HWND hwnd, int cx, int cy, uint8_t button, bool down)
{
    platform_Event e{};
    e.type = platform_EventType::Mouse;

    int mx, my;
    ToMathCoords(hwnd, cx, cy, mx, my, nullptr, nullptr);

    e.mouse.x = mx;
    e.mouse.y = my;
    e.mouse.dx = s_hasLastMouse ? (mx - s_lastX) : 0;
    e.mouse.dy = s_hasLastMouse ? (my - s_lastY) : 0;
    s_hasLastMouse = true; s_lastX = mx; s_lastY = my;

    e.mouse.button = button;    	// MB_Left / MB_Right / MB_Middle / MB_X1 / MB_X2
    e.mouse.down   = down;      	// True = pressed, false = released
    e.mouse.wheel  = 0.0f;      	// Key events do not include scroll wheel

    weavefra::GlobalEventBus::emit(e);
}

static inline void EmitWheel(HWND hwnd, int screenX, int screenY, float steps /* Every 120 is 1.0 */, bool horizontal)
{
    // lParam provides screen coordinates; convert to client first, then to mathematical coordinates
    POINT pt{ screenX, screenY };
    ScreenToClient(hwnd, &pt);

    platform_Event e{};
    e.type = platform_EventType::Mouse;

    int mx, my;
    ToMathCoords(hwnd, pt.x, pt.y, mx, my, nullptr, nullptr);

    e.mouse.x = mx;
    e.mouse.y = my;
    e.mouse.dx = s_hasLastMouse ? (mx - s_lastX) : 0;
    e.mouse.dy = s_hasLastMouse ? (my - s_lastY) : 0;
    s_hasLastMouse = true; s_lastX = mx; s_lastY = my;

    e.mouse.button = u8(MB_BUTTON::MB_None);
    e.mouse.down   = false;

	// Convention: Vertical scroll wheel steps > 0 means up, < 0 means down
	// Horizontal scroll wheel also uses wheel: steps > 0 means right, < 0 means left (if not needed, you can separate it)
    e.mouse.wheel  = steps;

    weavefra::GlobalEventBus::emit(e);
}

static inline void EmitKey(uint32_t vk, bool isDown, bool isRepeat)
{
    platform_Event e{};
    e.type      = platform_EventType::Key;
    e.key.key   = vk;        // Win32 virtual key codes VK_*
    e.key.down  = isDown;    // true = pressed, false = released
    e.key.repeat= isRepeat;  // true = system-generated repeated press (long press)
    weavefra::GlobalEventBus::emit(e);
}

static inline void EmitWindow(HWND hwnd, EventWindow::EventWindowType t)
{
    platform_Event e{};
    e.type = platform_EventType::Window;
    e.window.type = t;

    // width/height are uniformly filled with the current client size (some types may not be used)
    RECT rc{};
    GetClientRect(hwnd, &rc);
    e.window.width  = (rc.right  > rc.left) ? (uint32_t)(rc.right  - rc.left) : 0;
    e.window.height = (rc.bottom > rc.top ) ? (uint32_t)(rc.bottom - rc.top ) : 0;

    weavefra::GlobalEventBus::emit(e);
}


// -- todo: Customize the "draggable area" (client area simulating the title bar). This is just for reference. --
// Return true if the point is in the custom title bar; can change this to an actual check.
static bool InCustomCaptionArea(HWND hwnd, POINT ptClient)
{
    RECT rc{}; GetClientRect(hwnd, &rc);
    const int captionH = 32;// "Fake title bar" height
    return ptClient.y >= 0 && ptClient.y < captionH && ptClient.x >= 0 && ptClient.x < (rc.right - rc.left);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    // -------------------- Basics/Life Cycle --------------------
    case WM_NCCREATE:
        return TRUE;

    case WM_CREATE:
        // Window creation completed
        EmitWindow(hwnd, EventWindow::EventWindowType::Create);
        return 0;
	
    case WM_CLOSE:
        // User request to close (X/Alt+F4 in the upper right corner)
        EmitWindow(hwnd, EventWindow::EventWindowType::Close);
        DestroyWindow(hwnd);
        return 0;
	
    case WM_DESTROY:
        // The window is destroyed and the message loop is exited
        PostQuitMessage(0);
        return 0;

    case WM_SETFOCUS:
        EmitWindow(hwnd, EventWindow::EventWindowType::FocusIn);
        return 0;

    case WM_ERASEBKGND:
        //Returns nonzero if background erasing is complete
        // todo : set background erasing
        return 1;// todo : Full coverage drawing/double buffering
	
    // -------------------- Window size/position/focus --------------------
    case WM_SIZE:
    {
        const UINT type = (UINT)wParam;
        RECT* prc = reinterpret_cast<RECT*>(lParam);
        if (type == SIZE_MINIMIZED)
		{
            EmitWindow(hwnd, EventWindow::EventWindowType::Minimize);
        }
		else if (type == SIZE_RESTORED)
		{
            // Restore from minimize/maximize: send Restore (also carries current width/height)
            EmitWindow(hwnd, EventWindow::EventWindowType::Restore);
            // Restoring also counts as a Resize
            EmitWindow(hwnd, EventWindow::EventWindowType::Resize);
        }
		else
		{
            // Maximize or other size changes
            EmitWindow(hwnd, EventWindow::EventWindowType::Resize);
        }
        return TRUE;
    }
    case WM_MOVE:
        // Position changes (will not change client size)
        EmitWindow(hwnd, EventWindow::EventWindowType::Move);
        return 0;

    case WM_KILLFOCUS:
        EmitWindow(hwnd, EventWindow::EventWindowType::FocusOut);
        return 0;

    case WM_GETMINMAXINFO: {
        auto mmi = reinterpret_cast<MINMAXINFO*>(lParam);
        mmi->ptMinTrackSize = { 320, 240 };
        return 0;
    }

	// -------------------- Keyboard --------------------
    case WM_KEYDOWN:
    case WM_SYSKEYDOWN:
    {
        if(/* todo : System keys beed to intercept */ false)
        {
            const bool down   = (uMsg == WM_SYSKEYDOWN);
            const UINT vk     = (UINT)wParam;
            const bool wasDown= (lParam & (1 << 30)) != 0;
            EmitKey(vk, down, wasDown);
            return 0;
        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    case WM_KEYUP:
    case WM_SYSKEYUP:
    {
        if(/* todo : System keys beed to intercept */false)
        {
            const bool down   = (uMsg == WM_SYSKEYDOWN);
            const UINT vk     = (UINT)wParam;
            const bool wasDown= (lParam & (1 << 30)) != 0;
            EmitKey(vk, down, wasDown);
            return 0;
        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    case WM_CHAR:
        // todo : If there is text input logic, process characters here (different from VK_*)
        return 0;

	// -------------------- Mouse (client area) --------------------
    case WM_MOUSEMOVE:
        EmitMouseMove(hwnd, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam));
        return 0;

    case WM_LBUTTONDOWN: 
        SetCapture(hwnd); 
        EmitMouseButton(hwnd, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), u8(MB_BUTTON::MB_Left), true);
        return 0;

    case WM_LBUTTONUP:
        if (GetCapture() == hwnd) ReleaseCapture();
        EmitMouseButton(hwnd, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), u8(MB_BUTTON::MB_Left), false);
        return 0;

    case WM_RBUTTONDOWN:
        SetCapture(hwnd);
        EmitMouseButton(hwnd, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), u8(MB_BUTTON::MB_Right), true);
        return 0;

    case WM_RBUTTONUP:
        if (GetCapture() == hwnd) ReleaseCapture();
        EmitMouseButton(hwnd, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), u8(MB_BUTTON::MB_Right), false);
        return 0;

    case WM_MBUTTONDOWN:
        SetCapture(hwnd);
        EmitMouseButton(hwnd, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), u8(MB_BUTTON::MB_Middle), true); 
        return 0;

    case WM_MBUTTONUP: 
        if (GetCapture() == hwnd) ReleaseCapture();
        EmitMouseButton(hwnd, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), u8(MB_BUTTON::MB_Middle), false); 
        return 0;

	// X button series returns TRUE
    case WM_XBUTTONDOWN:
        SetCapture(hwnd);
        EmitMouseButton(hwnd, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam),
                        (HIWORD(wParam) == XBUTTON1) ? u8(MB_BUTTON::MB_X1) : u8(MB_BUTTON::MB_X2), true);
        return TRUE; // X button series returns TRUE
    case WM_XBUTTONUP:
        if (GetCapture() == hwnd) ReleaseCapture();
        EmitMouseButton(hwnd, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam),
                        (HIWORD(wParam) == XBUTTON1) ? u8(MB_BUTTON::MB_X1) : u8(MB_BUTTON::MB_X2), false);
        return TRUE;

    case WM_LBUTTONDBLCLK: EmitMouseButton(hwnd, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), u8(MB_BUTTON::MB_Left),   true);  return 0;
    case WM_RBUTTONDBLCLK: EmitMouseButton(hwnd, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), u8(MB_BUTTON::MB_Right),  true);  return 0;
    case WM_MBUTTONDBLCLK: EmitMouseButton(hwnd, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), u8(MB_BUTTON::MB_Middle), true);  return 0;
    case WM_XBUTTONDBLCLK:
        EmitMouseButton(hwnd, GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam),
                        (HIWORD(wParam) == XBUTTON1) ? u8(MB_BUTTON::MB_X1) : u8(MB_BUTTON::MB_X2), true);
        return TRUE;

    case WM_MOUSEWHEEL:
    case WM_MOUSEHWHEEL:
    {
        POINT pt{ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) }; // screen
        ScreenToClient(hwnd, &pt);
        const int  delta = GET_WHEEL_DELTA_WPARAM(wParam);
        const bool horizontal = (uMsg == WM_MOUSEHWHEEL);
        EmitWheel(hwnd, pt.x, pt.y, (float)delta / (float)WHEEL_DELTA, horizontal);
        return 0;
    }

	// -------------------- Mouse (non-client area: transfer to client -> mathematical coordinates and then distribute) ---------------
    case WM_NCMOUSEMOVE:
    case WM_NCLBUTTONDOWN: case WM_NCLBUTTONUP: case WM_NCLBUTTONDBLCLK:
    case WM_NCRBUTTONDOWN: case WM_NCRBUTTONUP: case WM_NCRBUTTONDBLCLK:
    case WM_NCMBUTTONDOWN: case WM_NCMBUTTONUP: case WM_NCMBUTTONDBLCLK:
    case WM_NCXBUTTONDOWN: case WM_NCXBUTTONUP: case WM_NCXBUTTONDBLCLK:
    {
        // lParam is the screen coordinate
        POINT pt{ GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
        ScreenToClient(hwnd, &pt);
	
        switch (uMsg)
        {
        case WM_NCMOUSEMOVE:case WM_NCLBUTTONDBLCLK:case WM_NCRBUTTONDBLCLK:
        case WM_NCLBUTTONDOWN:case WM_NCRBUTTONDOWN:case WM_NCMBUTTONDOWN:  
        case WM_NCLBUTTONUP:case WM_NCRBUTTONUP:case WM_NCMBUTTONUP:    
        case WM_NCMBUTTONDBLCLK:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        case WM_NCXBUTTONDOWN:
            //todo
            //EmitMouseButton(hwnd, pt.x, pt.y, (HIWORD(wParam) == XBUTTON1) ? MB_X1 : MB_X2, true);
            //return TRUE;
        case WM_NCXBUTTONUP:
            //EmitMouseButton(hwnd, pt.x, pt.y, (HIWORD(wParam) == XBUTTON1) ? MB_X1 : MB_X2, false);
            //return TRUE;
        case WM_NCXBUTTONDBLCLK:
            //EmitMouseButton(hwnd, pt.x, pt.y, (HIWORD(wParam) == XBUTTON1) ? MB_X1 : MB_X2, true);
            //return TRUE;
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }
    // -------------------- Others: default processing --------------------
    default:
        return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
#endif