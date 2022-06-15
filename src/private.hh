#include "../headers/io.hh"

#ifdef AMARETTO_PLATFORM_WINDOWS
#	define INPUT_RECORD_SIZE 128
#endif // AMARETTO_PLATFORM_WINDOWS

namespace Amaretto {
	extern Canvas      g_stdCanvas;
	extern const Rect *g_window;

#ifdef AMARETTO_PLATFORM_LINUX

	extern size_t g_colorPair;

#elif defined(AMARETTO_PLATFORM_WINDOWS)

	extern COORD  g_prevBufSize;
	extern HANDLE g_stdoutHandle, g_stdinHandle, g_screenHandle;
	extern CONSOLE_SCREEN_BUFFER_INFO g_cBufInfo;

	extern std::vector<CHAR_INFO> g_screen;

	extern bool  g_waitForKeyPress;
	extern DWORD g_eventPos, g_eventReadAmount;
	extern INPUT_RECORD g_inBuf[INPUT_RECORD_SIZE];

	extern DWORD g_prevStdinMode;

#endif // AMARETTO_PLATFORM_WINDOWS
}
