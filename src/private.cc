#include "private.hh"

Amaretto::Canvas      Amaretto::g_stdCanvas(Rect(0, 0, 0, 0));
const Amaretto::Rect *Amaretto::g_window = nullptr;

Amaretto::Vec2D Amaretto::g_mousePos(0, 0);
Amaretto::Vec2D Amaretto::g_cursorPos(0, 0);

Amaretto::MouseReport Amaretto::g_reportMouseEvents = MouseReport::None;

#ifdef AMARETTO_PLATFORM_LINUX

Amaretto::size_t Amaretto::g_colorPair = 0;
short            Amaretto::g_currColorPair = 0;

std::unordered_map<wchar_t, char> Amaretto::g_wCharToACSMap;

#elif defined(AMARETTO_PLATFORM_WINDOWS)

COORD  Amaretto::g_prevBufSize;
HANDLE Amaretto::g_stdoutHandle, Amaretto::g_stdinHandle, Amaretto::g_screenHandle;
CONSOLE_SCREEN_BUFFER_INFO Amaretto::g_cBufInfo;

std::vector<CHAR_INFO> Amaretto::g_screen;

bool  Amaretto::g_waitForKeyPress;
DWORD Amaretto::g_eventPos, Amaretto::g_eventReadAmount;
INPUT_RECORD Amaretto::g_inBuffer[INPUT_RECORD_SIZE];

DWORD Amaretto::g_prevStdinMode;
UINT  Amaretto::g_prevCodePage;

bool Amaretto::g_rMouseDown = false, Amaretto::g_lMouseDown = false;

#endif // AMARETTO_PLATFORM_WINDOWS
