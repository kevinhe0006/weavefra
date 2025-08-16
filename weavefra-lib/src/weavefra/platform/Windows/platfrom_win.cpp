#include "weavefra/platform/platform.h"

#if _OS_WINDOWS

    typedef struct Window_internal_State
    {
        HWND m_hwnd;
    	HINSTANCE m_hinstance;
    	HDC m_hdc;
    } Window_internal_State;

    LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
    {
    	switch (uMsg)
    	{
    	case WM_DESTROY:
    		PostQuitMessage(0);
    		return 0;

    	case WM_PAINT:
    	{
    		PAINTSTRUCT ps;
    		HDC hdc = BeginPaint(hwnd, &ps);

    		// All painting occurs here, between BeginPaint and EndPaint.

    		FillRect(hdc, &ps.rcPaint, (HBRUSH)(COLOR_WINDOW + 1));

    		EndPaint(hwnd, &ps);
    	}
    	return 0;

    	}
    	return DefWindowProc(hwnd, uMsg, wParam, lParam);
    }

	Platform_Window::Platform_Window() noexcept = default;

    Platform_Window::Platform_Window(WindowDesc w)
        : windowdesc(w)
    {
        state.Window_internal_State = std::malloc(sizeof(Window_internal_State));
        Window_internal_State *State = static_cast<Window_internal_State *>(state.Window_internal_State);
		if(!State) { WF_ASSERT(false, "no window state"); }
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

		u32	window_style = WS_OVERLAPPEDWINDOW | WS_SYSMENU | WS_CAPTION | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_OVERLAPPEDWINDOW | WS_VISIBLE;
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

		weavefra::Main_EventBus.emit(event);

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
		if(!State) { WF_ASSERT(false, "no window state"); return false; }
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
		if(!State) { WF_ASSERT(false, "no window state"); }
    	::SwapBuffers(State->m_hdc);
    }

    void Platform_Window::Show_Platform_Window(bool should_activate) const
    {
        Window_internal_State *State = static_cast<Window_internal_State *>(state.Window_internal_State);
		if(!State) { WF_ASSERT(false, "no window state"); }
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

    platform_state* Platform_Init(const InitOptions &Opt)
    {
		platform_state* State = (platform_state *)new platform_state{};
		if (!State) { State->Result = Platform_Error::PLATFORM_ERR_DISCONNECTED; return State; };
		State->window = Platform_Window(Opt.Window_Desc);
		State->window.Show_Platform_Window(1);
		State->Result = Platform_Error::PLATFORM_OK;
        return State;
    }

	Platform_Error Platform_Shutdown(platform_state *plat_state)
	{
		if(!plat_state) { WF_ASSERT(false, "platform state not exist"); return Platform_Error::PLATFORM_ERR_NOMEM; }
		delete plat_state;
		return Platform_Error::PLATFORM_OK;
	}

	typedef struct console_interna_state
	{
    	HANDLE  hOut;
    	HANDLE  hErr;
    	HANDLE  hIn;
    	DWORD   outModeOld;
    	DWORD   inModeOld;
    	BOOL    hasConsole;
    	BOOL    vtEnabled;
    	BOOL    allocatedConsole;
	} console_internal_state;

	static bool is_real_console(HANDLE h)
	{
	    if (!h || h == INVALID_HANDLE_VALUE) return false;
	    DWORD type = GetFileType(h);
	    return type == FILE_TYPE_CHAR;
	}
	static void write_bytes(HANDLE h, const char* data, DWORD len)
	{
	    if (!h || h == INVALID_HANDLE_VALUE || !data || !len) return;
	    DWORD written = 0;
	    // WriteFile works for consoles/pipes/redirections; WriteConsole only works for real consoles
	    ::WriteFile(h, data, len, &written, nullptr);
	}

	Platfrom_Console::Platfrom_Console()
	{
    	state.consle_state = std::malloc(sizeof(console_internal_state));
    	console_internal_state* State = static_cast<console_internal_state*>(state.consle_state);
		if (!State) { WF_ASSERT(State, "no console state"); }
    	std::memset(State, 0, sizeof(*State));

    	// Try to attach to the parent process console (e.g. a GUI program started from a terminal)
    	BOOL attached = ::AttachConsole(ATTACH_PARENT_PROCESS);
    	if (!attached)
		{
    	    // If there is no one to attach, allocate a new console
    	    State->allocatedConsole = ::AllocConsole();
    	}

    	State->hOut = ::GetStdHandle(STD_OUTPUT_HANDLE);
    	State->hErr = ::GetStdHandle(STD_ERROR_HANDLE);
    	State->hIn  = ::GetStdHandle(STD_INPUT_HANDLE);
    	State->hasConsole = (State->hOut && State->hOut != INVALID_HANDLE_VALUE);

    	// Record the old mode and try to enable ANSI (VT) support
    	State->vtEnabled = FALSE;
    	if (State->hasConsole && is_real_console(State->hOut))
		{
    	    if (::GetConsoleMode(State->hOut, &State->outModeOld))
			{
    	        DWORD outMode = State->outModeOld | ENABLE_VIRTUAL_TERMINAL_PROCESSING /* 24-bit ANSI */
    	                                         | DISABLE_NEWLINE_AUTO_RETURN;
    	        if (::SetConsoleMode(State->hOut, outMode))
				{
    	            State->vtEnabled = TRUE;
    	        }
    	    }
    	    // Input mode is not required, it is only recorded here for future expansion
    	    ::GetConsoleMode(State->hIn, &State->inModeOld);
    	}

    	//Use UTF-8 output uniformly (optional, but very useful)
    	::SetConsoleOutputCP(CP_UTF8);
	}
	Platfrom_Console::~Platfrom_Console()
	{
	    console_internal_state* State = static_cast<console_internal_state*>(state.consle_state);
		if(State)
		{
			if(State->hasConsole && is_real_console(State->hOut) && State->vtEnabled)
			{
				::SetConsoleMode(State->hOut, State->outModeOld);
			}
			if(State->allocatedConsole)
			{
				::FreeConsole();
			}
			std::free(State);
			state.consle_state = nullptr;
		}

	}
	void Platfrom_Console::Platform_ConsoleWrite(std::string message, u8 r, u8 g, u8 b)
	{
    	console_internal_state* State = static_cast<console_internal_state*>(state.consle_state);
    	if (!State) { WF_ASSERT(State, "no console state"); }

    	// Also send to debugger (VS Output window)
    	::OutputDebugStringA(message.c_str());
    	::OutputDebugStringA("\n");

    	const bool realCon = is_real_console(State->hOut);

    	if (State->vtEnabled && realCon)
		{
    	    // 24-bit ANSI color
    	    std::string out = "\x1b[38;2;" +
    	                      std::to_string((unsigned)r) + ";" +
    	                      std::to_string((unsigned)g) + ";" +
    	                      std::to_string((unsigned)b) + "m" +
    	                      message +
    	                      "\x1b[0m\n";
    	    write_bytes(State->hOut, out.c_str(), (DWORD)out.size());
    	    return;
    	}

    	if (realCon)
		{
    	    // Downgrade to 16 colors
    	    CONSOLE_SCREEN_BUFFER_INFO csbi{};
    	    WORD oldAttr = 0;
    	    if (::GetConsoleScreenBufferInfo(State->hOut, &csbi))
			{
    	        oldAttr = csbi.wAttributes;
    	    }

    	    WORD attr = 0;
    	    if (r > 64) attr |= FOREGROUND_RED;
    	    if (g > 64) attr |= FOREGROUND_GREEN;
    	    if (b > 64) attr |= FOREGROUND_BLUE;
    	    if (r > 192 || g > 192 || b > 192) attr |= FOREGROUND_INTENSITY;

    	    ::SetConsoleTextAttribute(State->hOut, attr);
    	    std::string out = message + "\n";
    	    write_bytes(State->hOut, out.c_str(), (DWORD)out.size());
    	    if (oldAttr) ::SetConsoleTextAttribute(State->hOut, oldAttr);
    	    return;
    	}

    	// Redirected to file/pipe: no color (to avoid polluting the log)
    	std::string out = message + "\n";
    	write_bytes(State->hOut, out.c_str(), (DWORD)out.size());
	}

	
	Platform_Clock::Platform_Clock()
		: Timestamp_(steady_clock::now()), system_time_point(system_clock::now()) {}

	void* platform_allocate(u64 size, bool aligned)
	{
		return malloc(size);
	}
	void platform_free(void* block, bool aligned)
	{
		free(block);
	}
	void* platform_copy_memory(void *dest, const void* source, u64 size)
	{
		return memcpy(dest, source, size);
	}
	void* platform_set_memory(void *dest, s32 value, u64 size)
	{
		return memset(dest, value, size);
	}
	
#endif
