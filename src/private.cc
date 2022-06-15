#include "private.hh"

Amaretto::Canvas      Amaretto::g_stdCanvas(Rect(0, 0, 0, 0));
const Amaretto::Rect *Amaretto::g_window(nullptr);

#ifdef AMARETTO_PLATFORM_LINUX

Amaretto::size_t Amaretto::g_colorPair;

#elif defined(AMARETTO_PLATFORM_WINDOWS)

COORD  Amaretto::g_prevBufSize;
HANDLE Amaretto::g_stdoutHandle, Amaretto::g_stdinHandle, Amaretto::g_screenHandle;
CONSOLE_SCREEN_BUFFER_INFO Amaretto::g_cBufInfo;

std::vector<CHAR_INFO> Amaretto::g_screen;

bool  Amaretto::g_waitForKeyPress;
DWORD Amaretto::g_eventPos, Amaretto::g_eventReadAmount;
INPUT_RECORD Amaretto::g_inBuf[INPUT_RECORD_SIZE];

DWORD Amaretto::g_prevStdinMode;

#endif // AMARETTO_PLATFORM_WINDOWS
