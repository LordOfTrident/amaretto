#include "../headers/io.hh"
#include "private.hh"

// class Event
// public

Amaretto::Event::Event():
	m_eventType(EventType::None),
	m_key(Key::None)
{}

Amaretto::EventType Amaretto::Event::GetEventType() {
	return m_eventType;
}

void Amaretto::Event::SetEventType(EventType p_type) {
	m_eventType = p_type;
}

Amaretto::uint16_t Amaretto::Event::GetKey() {
	return m_key;
}

void Amaretto::Event::SetKey(uint16_t p_key) {
	m_key = p_key;
}

// main functions

void Amaretto::Init() {
	std::setlocale(LC_CTYPE, "");

#ifdef AMARETTO_PLATFORM_LINUX

	initscr();            // init ncurses
	raw();                // raw mode
	noecho();             // dont echo input
	timeout(-1);          // no getch delay
	keypad(stdscr, true); // keypad input
	mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, nullptr);

	start_color();
	use_default_colors();
	mouseinterval(0);

	ESCDELAY = 0; // no delay when escape is pressed

#elif defined(AMARETTO_PLATFORM_WINDOWS)

	// standard handles
	g_stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	g_stdinHandle  = GetStdHandle(STD_INPUT_HANDLE);

	if (g_stdoutHandle == INVALID_HANDLE_VALUE or g_stdoutHandle == nullptr)
		throw std::runtime_error("GetStdHandle on STD_OUTPUT_HANDLE");

	if (g_stdinHandle == INVALID_HANDLE_VALUE or g_stdoutHandle == nullptr)
		throw std::runtime_error("GetStdHandle on STD_INPUT_HANDLE");

	if (not GetConsoleMode(g_stdinHandle, &g_prevStdinMode))
		throw std::runtime_error("GetConsoleMode");

	g_screenHandle = CreateConsoleScreenBuffer(
		GENERIC_READ | GENERIC_WRITE, 0, nullptr, CONSOLE_TEXTMODE_BUFFER, nullptr
	);

	if (g_screenHandle == INVALID_HANDLE_VALUE or g_screenHandle == nullptr)
		throw std::runtime_error("CreateConsoleScreenBuffer");

	if (not SetConsoleActiveScreenBuffer(g_screenHandle))
		throw std::runtime_error("SetConsoleActiveScreenBuffer");

	DWORD stdinMode = ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_PROCESSED_INPUT;

	if (not SetConsoleMode(g_stdinHandle, stdinMode))
		throw std::runtime_error("SetConsoleMode");

	ShowCursor(true);

#endif // PLATFOR_WINDOWS

	g_stdCanvas.SetColors(Color::Default, Color::Default);

	Update(); // init the screen
	Clear();
}

void Amaretto::Finish() {
#ifdef AMARETTO_PLATFORM_LINUX

	endwin(); // end ncurses

#elif defined(AMARETTO_PLATFORM_WINDOWS)

	// set console back to what it was
	if (not SetConsoleMode(g_stdinHandle, g_prevStdinMode))
		throw std::runtime_error("SetConsoleMode");

#endif // AMARETTO_PLATFORM_WINDOWS
}

void Amaretto::WaitForNextEvent(bool p_wait) {
#ifdef AMARETTO_PLATFORM_LINUX

	nodelay(stdscr, not p_wait);

#elif defined(AMARETTO_PLATFORM_WINDOWS)

	g_waitForKeyPress = p_wait;

#endif // AMARETTO_PLATFORM_WINDOWS
}

void Amaretto::CBreak(bool p_cBreak) {
#ifdef AMARETTO_PLATFORM_LINUX

	if (p_cBreak)
		cbreak();
	else
		nocbreak();

#elif defined(AMARETTO_PLATFORM_WINDOWS)

	SetConsoleCtrlHandler(nullptr, not p_cBreak);

#endif // AMARETTO_PLATFORM_WINDOWS
}

void Amaretto::Delay(size_t p_ms) {
#ifdef AMARETTO_PLATFORM_LINUX

	napms(p_ms);

#elif defined(AMARETTO_PLATFORM_WINDOWS)

	Sleep(p_ms);

#endif // AMARETTO_PLATFORM_WINDOWS
}

void Amaretto::SetTermTitle(const string &p_title) {
#ifdef AMARETTO_PLATFORM_LINUX

	putp(("\033]0;" + p_title + "\007").c_str()); // ansi title escape sequence

#elif defined(AMARETTO_PLATFORM_WINDOWS)

	if (not SetConsoleTitle(p_title.c_str()))
		throw std::runtime_error("SetConsoleTitle");

#endif // AMARETTO_PLATFORM_WINDOWS
}

Amaretto::Event Amaretto::GetEvent() {
	Event event;

#ifdef AMARETTO_PLATFORM_LINUX

	uint16_t in = getch();

	// convert input into keys
	switch (in) {
	case 127: case KEY_BACKSPACE: event.SetKey(Key::Backspace); break;
	case 10:  case KEY_ENTER:     event.SetKey(Key::Enter);     break;
	case 9:   case KEY_STAB:      event.SetKey(Key::Tab);       break;
	case 27:        event.SetKey(Key::Escape);     break;
	case KEY_DOWN:  event.SetKey(Key::ArrowDown);  break;
	case KEY_UP:    event.SetKey(Key::ArrowUp);    break;
	case KEY_LEFT:  event.SetKey(Key::ArrowLeft);  break;
	case KEY_RIGHT: event.SetKey(Key::ArrowRight); break;

	// set event types on resize and mouse
	case KEY_RESIZE: event.SetEventType(EventType::WindowResize); break;
	case KEY_MOUSE:  event.SetEventType(EventType::Mouse);        break;

	// no input
	case static_cast<uint16_t>(-1): break;

	// else assume its a key press
	default: event.SetKey(in);
	}

	// if event key isnt none, set the event type to key press
	if (event.GetKey() != Key::None)
		event.SetEventType(EventType::KeyPress);

#elif defined(AMARETTO_PLATFORM_WINDOWS)

	// ncurses getch runs refresh automatically, we should keep the same behavior
	// on all platforms
	Refresh();

	// a "complicated" system to give out the same results on both platforms

	DWORD eventsCount = 0;
	if (g_eventPos == 0) // g_eventPos explained later in this function
		// check if there are any events pending
		GetNumberOfConsoleInputEvents(g_stdinHandle, &eventsCount);

	// if there are events OR if we want to wait for one
	if (g_waitForKeyPress or eventsCount > 0 or g_eventPos != 0) {
		if (g_eventPos == 0) {
			// read the input
		l_readConsoleInput:
			if (not ReadConsoleInput(g_stdinHandle, g_inBuf, INPUT_RECORD_SIZE, &g_eventReadAmount))
				throw std::runtime_error("ReadConsoleInput");
		}

		// a loop to read all events
		for (; g_eventPos < g_eventReadAmount; ++ g_eventPos) {
			switch (g_inBuf[g_eventPos].EventType) {
			case KEY_EVENT:
				{
					// shorten the input variable
					uint16_t in = g_inBuf[g_eventPos].Event.KeyEvent.wVirtualKeyCode;

					if (g_inBuf[g_eventPos].Event.KeyEvent.bKeyDown) {
						switch (in) {
						case VK_RETURN:  event.SetKey(Key::Enter);      break;
						case VK_BACK:    event.SetKey(Key::Backspace);  break;
						case VK_ESCAPE:  event.SetKey(Key::Escape);     break;
						case VK_DOWN:    event.SetKey(Key::ArrowDown);  break;
						case VK_UP:      event.SetKey(Key::ArrowUp);    break;
						case VK_LEFT:    event.SetKey(Key::ArrowLeft);  break;
						case VK_RIGHT:   event.SetKey(Key::ArrowRight); break;
						case VK_TAB:     event.SetKey(Key::Tab);        break;

						// skip these keys, they arent detectable with ncurses
						case VK_SHIFT:   goto l_skip;
						case VK_CAPITAL: goto l_skip;
						case VK_CONTROL: goto l_skip;
						case VK_MENU:    goto l_skip;

						default:
							// windows input returns the upper case version of
							// letters, we dont want that.
							if (
								(GetKeyState(VK_SHIFT)   & 0x8000) or
								(GetKeyState(VK_CAPITAL) & 0x0001)
							)
								// set the letter to upper case if shift is pressed
								// (just in case)
								event.SetKey(std::toupper(in));
							else
								// else turn it to lower case
								event.SetKey(std::tolower(in));
						}

						// saving the current event position
						// (if we found an event, we return it and dont continue the
						// loop, we have to know where we left off)
						++ g_eventPos;
						event.SetEventType(EventType::KeyPress);

						return event;
					}

				l_skip:
					break;
				}

			case FOCUS_EVENT: break;
			case WINDOW_BUFFER_SIZE_EVENT:
				event.SetEventType(EventType::WindowResize);
				++ g_eventPos;

				return event;

			case MOUSE_EVENT:
				event.SetEventType(EventType::Mouse);
				++ g_eventPos;

				return event;
			default: break;
			}
		}

		// all pending events have been read
		g_eventPos = 0;

		// if there were no events we care about, but the read function should wait
		// for one, we go back and wait for one
		if (g_waitForKeyPress)
			goto l_readConsoleInput;
	}

#endif // AMARETTO_PLATFORM_WINDOWS

	return event;
}

void Amaretto::ShowCursor(bool p_show) {
#ifdef AMARETTO_PLATFORM_LINUX

	curs_set(p_show);

#elif defined(AMARETTO_PLATFORM_WINDOWS)

	CONSOLE_CURSOR_INFO info;
	info.dwSize = 100; // fill up 100% of the character
	info.bVisible = p_show;

	if (SetConsoleCursorInfo(g_screenHandle, &info) == false)
		throw std::runtime_error("SetConsoleCursorInfo");

#endif // AMARETTO_PLATFORM_WINDOWS
}

bool Amaretto::CanChangeColors() {
#ifdef AMARETTO_PLATFORM_LINUX

	return can_change_color();

#elif defined(AMARETTO_PLATFORM_WINDOWS)

	return false; // windows, come on :(

#endif // AMARETTO_PLATFORM_WINDOWS
}

bool Amaretto::CanUseColors() {
#ifdef AMARETTO_PLATFORM_LINUX

	return has_colors();

#elif defined(AMARETTO_PLATFORM_WINDOWS)

	return true;

#endif // AMARETTO_PLATFORM_WINDOWS
}

Amaretto::size_t Amaretto::Colors() {
#ifdef AMARETTO_PLATFORM_LINUX

	return COLORS;

#elif defined(AMARETTO_PLATFORM_WINDOWS)

	return 16; // right?

#endif // AMARETTO_PLATFORM_WINDOWS
}

void Amaretto::ResetColorPairs() {
	// this function does nothing on windows, we keep it for cross-platformness
#ifdef AMARETTO_PLATFORM_LINUX

	g_colorPair = 0; // reset color pairs so we dont keep the ones we dont need

#endif // AMARETTO_PLATFORM_LINUX
}

void Amaretto::Clear() {
#ifdef AMARETTO_PLATFORM_LINUX

	erase();

#elif defined(AMARETTO_PLATFORM_WINDOWS)

	for (auto &ch : g_screen) {
		ch.Char.UnicodeChar = ' ';
		ch.Attributes = COLOR_WHITE;
	}

#endif // AMARETTO_PLATFORM_WINDOWS
}

void Amaretto::Update() {
	Rect window(0, 0, 0, 0);

#ifdef AMARETTO_PLATFORM_LINUX

	window.size.x = getmaxx(stdscr);
	window.size.y = getmaxy(stdscr);

#elif defined(AMARETTO_PLATFORM_WINDOWS)

	if (GetConsoleScreenBufferInfo(g_stdoutHandle, &g_cBufInfo) == false)
		throw std::runtime_error("GetConsoleScreenBufferInfo");

	// dwSize.X and dwSize.Y give the buffer size, not the window size
    window.size.x = g_cBufInfo.srWindow.Right  - g_cBufInfo.srWindow.Left + 1;
    window.size.y = g_cBufInfo.srWindow.Bottom - g_cBufInfo.srWindow.Top  + 1;

	g_screen.resize(window.size.x * window.size.y, {' ', 0});

#endif // AMARETTO_PLATFORM_WINDOWS

	g_stdCanvas.SetRect(window);
	g_window = &g_stdCanvas.GetRect();
}

void Amaretto::Refresh() {
#ifdef AMARETTO_PLATFORM_LINUX

	refresh();

#elif defined(AMARETTO_PLATFORM_WINDOWS)

	// the rectangle we wanna write into (the entire console)
	SMALL_RECT windowRect = {
		0, 0,
		static_cast<SHORT>(g_window->size.x - 1), static_cast<SHORT>(g_window->size.y - 1)
	};

	WriteConsoleOutput(
		g_screenHandle, g_screen.data(),
		{static_cast<SHORT>(g_window->size.x), static_cast<SHORT>(g_window->size.y)},
		{0, 0},
		&windowRect
	);

#endif // AMARETTO_PLATFORM_WINDOWS
}

Amaretto::uint16_t Amaretto::GetLastErrorID() {
#ifdef AMARETTO_PLATFORM_LINUX

	return 0;

#elif defined(AMARETTO_PLATFORM_WINDOWS)

	return GetLastError();

#endif
}

const Amaretto::Vec2D &Amaretto::GetWindowSize() {
	return g_stdCanvas.GetRect().size;
}

// screen functions

void Amaretto::Screen::DrawChar(char_t p_ch, const Vec2D &p_pos, bool p_clr) {
	g_stdCanvas.DrawChar(p_ch, p_pos, p_clr);
}

Amaretto::char_t Amaretto::Screen::GetChar(const Vec2D &p_pos) {
	return g_stdCanvas.GetChar(p_pos);
}

void Amaretto::Screen::DrawHLine(char_t p_ch, const Vec2D &p_pos, pos_t p_endX, bool p_clr) {
	g_stdCanvas.DrawHLine(p_ch, p_pos, p_endX, p_clr);
}

void Amaretto::Screen::DrawHLine(
	const string &p_str, const Vec2D &p_pos, pos_t p_endX, bool p_clr
) {
	g_stdCanvas.DrawHLine(p_str, p_pos, p_endX, p_clr);
}

void Amaretto::Screen::DrawHLine(const Pattern &p_patt, const Vec2D &p_pos, pos_t p_endX) {
	g_stdCanvas.DrawHLine(p_patt, p_pos, p_endX);
}

void Amaretto::Screen::DrawVLine(char_t p_ch, const Vec2D &p_pos, pos_t p_endY, bool p_clr) {
	g_stdCanvas.DrawVLine(p_ch, p_pos, p_endY, p_clr);
}

void Amaretto::Screen::DrawVLine(
	const string &p_str, const Vec2D &p_pos, pos_t p_endY, bool p_clr
) {
	g_stdCanvas.DrawVLine(p_str, p_pos, p_endY, p_clr);
}

void Amaretto::Screen::DrawVLine(const Pattern &p_patt, const Vec2D &p_pos, pos_t p_endY) {
	g_stdCanvas.DrawVLine(p_patt, p_pos, p_endY);
}

void Amaretto::Screen::PrintString(
	const string &p_str, const Vec2D &p_pos, bool p_wrap, bool p_clr
) {
	g_stdCanvas.PrintString(p_str, p_pos, p_wrap, p_clr);
}

void Amaretto::Screen::PrintString(size_t p_num, const Vec2D &p_pos, bool p_wrap, bool p_clr) {
	g_stdCanvas.PrintString(p_num, p_pos, p_wrap, p_clr);
}

void Amaretto::Screen::FillRect(char_t p_ch, const Rect &p_rect, bool p_clr) {
	g_stdCanvas.FillRect(p_ch, p_rect, p_clr);
}

void Amaretto::Screen::FillRect(const string &p_str, const Rect &p_rect, bool p_clr) {
	g_stdCanvas.FillRect(p_str, p_rect, p_clr);
}

void Amaretto::Screen::FillRect(const Pattern &p_patt, const Rect &p_rect) {
	g_stdCanvas.FillRect(p_patt, p_rect);
}

void Amaretto::Screen::FillRect(char_t p_ch, bool p_clr) {
	g_stdCanvas.FillRect(p_ch, p_clr);
}

void Amaretto::Screen::FillRect(const string &p_str, bool p_clr) {
	g_stdCanvas.FillRect(p_str, p_clr);
}

void Amaretto::Screen::FillRect(const Pattern &p_patt) {
	g_stdCanvas.FillRect(p_patt);
}

void Amaretto::Screen::DrawRect(char_t p_ch, const Rect &p_rect, bool p_clr) {
	g_stdCanvas.FillRect(p_ch, p_rect, p_clr);
}

void Amaretto::Screen::DrawRect(const string &p_str, const Rect &p_rect, bool p_clr) {
	g_stdCanvas.FillRect(p_str, p_rect, p_clr);
}

void Amaretto::Screen::DrawRect(const Pattern &p_patt, const Rect &p_rect) {
	g_stdCanvas.FillRect(p_patt, p_rect);
}

void Amaretto::Screen::DrawRect(char_t p_ch, bool p_clr) {
	g_stdCanvas.FillRect(p_ch, p_clr);
}

void Amaretto::Screen::DrawRect(const string &p_str, bool p_clr) {
	g_stdCanvas.FillRect(p_str, p_clr);
}

void Amaretto::Screen::DrawRect(const Pattern &p_patt) {
	g_stdCanvas.FillRect(p_patt);
}

void Amaretto::Screen::DrawBorder(const Border &p_border, const Rect &p_rect, bool p_clr) {
	g_stdCanvas.DrawBorder(p_border, p_rect, p_clr);
}

void Amaretto::Screen::DrawBorder(const Border &p_border, bool p_clr) {
	g_stdCanvas.DrawBorder(p_border, p_clr);
}

void Amaretto::Screen::DrawLine(char_t p_ch, const Vec2D &p_posA, const Vec2D &p_posB, bool p_clr) {
	g_stdCanvas.DrawLine(p_ch, p_posA, p_posB, p_clr);
}

void Amaretto::Screen::DrawLine(
	const string &p_str, const Vec2D &p_posA, const Vec2D &p_posB, bool p_clr
) {
	g_stdCanvas.DrawLine(p_str, p_posA, p_posB, p_clr);
}

void Amaretto::Screen::DrawLine(const Pattern &p_patt, const Vec2D &p_posA, const Vec2D &p_posB) {
	g_stdCanvas.DrawLine(p_patt, p_posA, p_posB);
}

void Amaretto::Screen::SetColors(Color p_fg, Color p_bg) {
	g_stdCanvas.SetColors(p_fg, p_bg);
}

void Amaretto::Screen::SetFgColor(Color p_fg) {
	g_stdCanvas.SetFgColor(p_fg);
}

void Amaretto::Screen::SetBgColor(Color p_bg) {
	g_stdCanvas.SetBgColor(p_bg);
}

Amaretto::Color Amaretto::Screen::GetFgColor() {
	return g_stdCanvas.GetFgColor();
}

Amaretto::Color Amaretto::Screen::GetBgColor() {
	return g_stdCanvas.GetBgColor();
}

void Amaretto::Screen::SetCursorPos(const Vec2D &p_pos) {
	g_stdCanvas.SetCursorPos(p_pos);
}

const Amaretto::Rect &Amaretto::Screen::GetRect() {
	return g_stdCanvas.GetRect();
}
