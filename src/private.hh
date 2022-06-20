#ifndef PRIVATE_HH__HEADER_GUARD__
#define PRIVATE_HH__HEADER_GUARD__

#include <unordered_map> // std::unordered_map

#include "../headers/io.hh"

#ifdef AMARETTO_PLATFORM_WINDOWS
#	define INPUT_RECORD_SIZE 128
#	define COLOR_BACKGROUND_OFFSET 0x10
#endif // AMARETTO_PLATFORM_WINDOWS

namespace Amaretto {
	constexpr int g_wCharStart = 128;

	extern Canvas      g_stdCanvas;
	extern const Rect *g_window;

	extern Vec2D g_mousePos;
	extern Vec2D g_cursorPos;

	extern MouseReport g_reportMouseEvents;

#ifdef AMARETTO_PLATFORM_LINUX

	extern size_t g_colorPair;
	extern short  g_currColorPair;

	extern std::unordered_map<wchar_t, char> g_wCharToACSMap;

#elif defined(AMARETTO_PLATFORM_WINDOWS)

	extern COORD  g_prevBufSize;
	extern HANDLE g_stdoutHandle, g_stdinHandle, g_screenHandle;
	extern CONSOLE_SCREEN_BUFFER_INFO g_cBufInfo;

	extern std::vector<CHAR_INFO> g_screen;

	extern bool  g_waitForKeyPress;
	extern DWORD g_eventPos, g_eventReadAmount;
	extern INPUT_RECORD g_inBuffer[INPUT_RECORD_SIZE];

	extern DWORD g_prevStdinMode;
	extern UINT  g_prevCodePage;

	extern bool g_rMouseDown, g_lMouseDown;

#endif // AMARETTO_PLATFORM_WINDOWS
}

#endif // not PRIVATE_HH__HEADER_GUARD__
