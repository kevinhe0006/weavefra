#include "weavefra/platform/platform.h"

#if _OS_WINDOWS


typedef struct console_interna_state
{
	HANDLE hOut_ = nullptr;
	HANDLE hErr_ = nullptr;
	HANDLE hIn_  = nullptr;
	
	DWORD  outModeOld_ = 0;
	DWORD  inModeOld_  = 0;
	
	bool   vtEnabled_ = false;
	bool   hasConsole_ = false;
	bool   allocatedConsole_ = false;
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
	State->hOut_ = ::GetStdHandle(STD_OUTPUT_HANDLE);
	State->hErr_ = ::GetStdHandle(STD_ERROR_HANDLE);
	State->hIn_  = ::GetStdHandle(STD_INPUT_HANDLE);
	State->hasConsole_ = is_real_console(State->hOut_);
}
Platfrom_Console::~Platfrom_Console()
{
    console_internal_state* State = static_cast<console_internal_state*>(state.consle_state);
	if (State->hasConsole_ && is_real_console(State->hOut_) && State->vtEnabled_) {
	    ::SetConsoleMode(State->hOut_, State->outModeOld_);
	}
	if (State->allocatedConsole_) {
	    ::FreeConsole();
	}

}
bool Platfrom_Console::consoleSet()
{
	console_internal_state* State = static_cast<console_internal_state*>(state.consle_state);
    if (!State->hasConsole_) return false;

    DWORD mode = 0;
    if (::GetConsoleMode(State->hOut_, &State->outModeOld_)) {
        mode = State->outModeOld_ | ENABLE_VIRTUAL_TERMINAL_PROCESSING | DISABLE_NEWLINE_AUTO_RETURN;
        if (::SetConsoleMode(State->hOut_, mode)) {
            State->vtEnabled_ = true;
        }
    }
    ::GetConsoleMode(State->hIn_, &State->inModeOld_);

    ::SetConsoleOutputCP(CP_UTF8);
    return State->vtEnabled_;
}
WORD rgb_to_attr(uint8_t r, uint8_t g, uint8_t b)
{
	WORD attr = 0;
	if (r > 64)  attr |= FOREGROUND_RED;
	if (g > 64)  attr |= FOREGROUND_GREEN;
	if (b > 64)  attr |= FOREGROUND_BLUE;
	if (r > 192 || g > 192 || b > 192) attr |= FOREGROUND_INTENSITY;
	return attr ? attr : (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
}
void Platfrom_Console::consoleWrite(const std::string& message,
                                   uint8_t r, uint8_t g, uint8_t b,
                                   bool ensureNewline)
{
    ::OutputDebugStringA(message.c_str());
    ::OutputDebugStringA("\n");

    HANDLE hOut = ::GetStdHandle(STD_OUTPUT_HANDLE);
    const bool realCon = is_real_console(hOut);

    std::string out = message;
    if (ensureNewline && (out.empty() || out.back() != '\n'))
	{
        out.push_back('\n');
    }

    DWORD mode = 0;
    bool vtEnabled = (realCon && ::GetConsoleMode(hOut, &mode) && (mode & ENABLE_VIRTUAL_TERMINAL_PROCESSING));

    if (vtEnabled)
	{
        std::string prefix = "\x1b[38;2;" +
                             std::to_string(static_cast<unsigned>(r)) + ";" +
                             std::to_string(static_cast<unsigned>(g)) + ";" +
                             std::to_string(static_cast<unsigned>(b)) + "m";
        std::string ansiOut = prefix + out + "\x1b[0m";
        write_bytes(hOut, ansiOut.c_str(), static_cast<DWORD>(ansiOut.size()));
        return;
    }

    if (realCon)
	{
        CONSOLE_SCREEN_BUFFER_INFO csbi{};
        WORD oldAttr = 0;
        if (::GetConsoleScreenBufferInfo(hOut, &csbi))
		{
            oldAttr = csbi.wAttributes;
        }
        WORD attr = rgb_to_attr(r, g, b);
        ::SetConsoleTextAttribute(hOut, attr);
        write_bytes(hOut, out.c_str(), static_cast<DWORD>(out.size()));
        if (oldAttr) ::SetConsoleTextAttribute(hOut, oldAttr);
        return;
    }

    write_bytes(hOut, out.c_str(), static_cast<DWORD>(out.size()));
}
#endif