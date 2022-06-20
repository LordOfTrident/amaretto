#include <cstdlib>

#include "../headers/io.hh"
#include "private.hh"

void Amaretto::Init(const string &p_locale) {
	std::setlocale(LC_CTYPE, p_locale.c_str());

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

	g_wCharToACSMap[0x250c] = 108; // TLCorner
	g_wCharToACSMap[0x2514] = 109; // BLCorner
	g_wCharToACSMap[0x2510] = 107; // TRCorner
	g_wCharToACSMap[0x2518] = 106; // BRCorner

	g_wCharToACSMap[0x2524] = 117; // LConnect
	g_wCharToACSMap[0x251c] = 116; // RConnect
	g_wCharToACSMap[0x252c] = 119; // BConnect
	g_wCharToACSMap[0x2534] = 118; // TConnect

	g_wCharToACSMap[0x2500] = 113; // HLine
	g_wCharToACSMap[0x2502] = 120; // VLine

	g_wCharToACSMap[0x253c] = 110; // Plus

	g_wCharToACSMap[0x2554] = 67; // DTLCorner
	g_wCharToACSMap[0x255a] = 68; // DBLCorner
	g_wCharToACSMap[0x2557] = 66; // DTRCorner
	g_wCharToACSMap[0x255d] = 65; // DBRCorner

	g_wCharToACSMap[0x2563] = 71; // DLConnect
	g_wCharToACSMap[0x2560] = 70; // DRConnect
	g_wCharToACSMap[0x2566] = 73; // DBConnect
	g_wCharToACSMap[0x2569] = 72; // DTConnect

	g_wCharToACSMap[0x2550] = 82; // DHLine
	g_wCharToACSMap[0x2551] = 89; // DVLine

	g_wCharToACSMap[0x256c] = 69; // DPlus

	g_wCharToACSMap[0x250f] = 76; // TTLCorner
	g_wCharToACSMap[0x2517] = 77; // TBLCorner
	g_wCharToACSMap[0x2513] = 75; // TTRCorner
	g_wCharToACSMap[0x251b] = 74; // TBRCorner

	g_wCharToACSMap[0x252b] = 85; // TLConnect
	g_wCharToACSMap[0x2523] = 84; // TRConnect
	g_wCharToACSMap[0x2533] = 87; // TBConnect
	g_wCharToACSMap[0x253b] = 86; // TTConnect

	g_wCharToACSMap[0x2501] = 81; // THLine
	g_wCharToACSMap[0x2503] = 88; // TVLine

	g_wCharToACSMap[0x254b] = 78; // TPlus

	g_wCharToACSMap[0x23ba] = 111; // ScanLine5
	g_wCharToACSMap[0x23bb] = 112; // ScanLine4
	g_wCharToACSMap[0x23bc] = 114; // ScanLine3
	g_wCharToACSMap[0x23bd] = 115; // ScanLine2

	g_wCharToACSMap[0x2190] = 44;  // LArrow
	g_wCharToACSMap[0x2191] = 45;  // UArrow
	g_wCharToACSMap[0x2192] = 43;  // RArrow
	g_wCharToACSMap[0x2193] = 46;  // DArrow

	g_wCharToACSMap[0x2603] = 105; // Snowman
	g_wCharToACSMap[0x25c6] = 96;  // Diamond
	g_wCharToACSMap[0xb0]   = 102; // Degree
	g_wCharToACSMap[0xb7]   = 126; // Bullet
	g_wCharToACSMap[0xb1]   = 103; // PlusMinus
	g_wCharToACSMap[0x2264] = 121; // LessEqual
	g_wCharToACSMap[0x2265] = 122; // GreaterEqual
	g_wCharToACSMap[0x2260] = 124; // NotEqual
	g_wCharToACSMap[0x3c0]  = 123; // PI
	g_wCharToACSMap[0xa3]   = 125; // Pound

	g_wCharToACSMap[0x2588] = 48;  // Block
	g_wCharToACSMap[0x2592] = 97;  // Transparent2

	g_wCharToACSMap[0x259a] = 104; // Checkerboard1

#elif defined(AMARETTO_PLATFORM_WINDOWS)

	// standard handles
	g_stdoutHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	g_stdinHandle  = GetStdHandle(STD_INPUT_HANDLE);
	g_prevCodePage = GetConsoleOutputCP();

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

	DWORD stdinMode = ENABLE_EXTENDED_FLAGS;
	if (not SetConsoleMode(g_stdinHandle, stdinMode))
		throw std::runtime_error("SetConsoleMode");

	stdinMode =  ENABLE_WINDOW_INPUT | ENABLE_MOUSE_INPUT | ENABLE_PROCESSED_INPUT;
	if (not SetConsoleMode(g_stdinHandle, stdinMode))
		throw std::runtime_error("SetConsoleMode");

	SetConsoleOutputCP(CP_WINUNICODE);
	SetConsoleCP(CP_WINUNICODE);

	ShowCursor(true);

#endif // PLATFOR_WINDOWS

	g_stdCanvas.SetColors(Color::Default, Color::Default);

	Screen::Update(); // init the screen
	Screen::Clear();
}

void Amaretto::Finish() {
#ifdef AMARETTO_PLATFORM_LINUX

	if (g_reportMouseEvents & MouseReport::Movement or g_reportMouseEvents & MouseReport::Scroll)
		putp("\033[?1003l\n"); // disable mouse movement events

	endwin(); // end ncurses

#elif defined(AMARETTO_PLATFORM_WINDOWS)

	// set console back to what it was
	if (not SetConsoleMode(g_stdinHandle, g_prevStdinMode))
		throw std::runtime_error("SetConsoleMode");

	SetConsoleOutputCP(g_prevCodePage);
	SetConsoleCP(g_prevCodePage);

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

void Amaretto::ReportMouseEvents(MouseReport p_report) {
	g_reportMouseEvents = p_report;

#ifdef AMARETTO_PLATFORM_LINUX

	if (g_reportMouseEvents & MouseReport::Movement or g_reportMouseEvents & MouseReport::Scroll)
		putp("\033[?1003h\n"); // enable mouse movement events
	else
		putp("\033[?1003l\n"); // disable mouse movement events

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
	EventType eventType = EventType::None;
	uint16_t  key = Key::None;

	MouseState mouseState = MouseState::None;
	bool       mouseMoved = false;

#ifdef AMARETTO_PLATFORM_LINUX

	uint16_t in = getch();

	// convert input into keys
	switch (in) {
	case 127: case KEY_BACKSPACE: key = Key::Backspace; break;
	case 10:  case KEY_ENTER:     key = Key::Enter;     break;
	case 9:   case KEY_STAB:      key = Key::Tab;       break;
	case 27:        key = Key::Escape;     break;
	case KEY_DOWN:  key = Key::ArrowDown;  break;
	case KEY_UP:    key = Key::ArrowUp;    break;
	case KEY_LEFT:  key = Key::ArrowLeft;  break;
	case KEY_RIGHT: key = Key::ArrowRight; break;

	case KEY_F(1):  key = Key::F1;  break;
	case KEY_F(2):  key = Key::F2;  break;
	case KEY_F(3):  key = Key::F3;  break;
	case KEY_F(4):  key = Key::F4;  break;
	case KEY_F(5):  key = Key::F5;  break;
	case KEY_F(6):  key = Key::F6;  break;
	case KEY_F(7):  key = Key::F7;  break;
	case KEY_F(8):  key = Key::F8;  break;
	case KEY_F(9):  key = Key::F9;  break;
	case KEY_F(10): key = Key::F10; break;
	case KEY_F(11): key = Key::F11; break;
	case KEY_F(12): key = Key::F12; break;

	// set event types on resize and mouse
	case KEY_RESIZE: eventType = EventType::WindowResize; break;
	case KEY_MOUSE:
		if (not g_reportMouseEvents)
			break;

		MEVENT mouseEvent;
		if (getmouse(&mouseEvent) == OK) {
			Vec2D mousePos(mouseEvent.x, mouseEvent.y);

			if (g_mousePos != mousePos) {
				if (g_reportMouseEvents & MouseReport::Movement) {
					mouseMoved = true;

					g_mousePos = mousePos;

					eventType = EventType::Mouse;
				}
			}

			switch (mouseEvent.bstate) {
			case 0x00000002: case 0x10000002:
				if (g_reportMouseEvents & MouseReport::PressRelease) {
					mouseState = MouseState::LBDown;
					eventType  = EventType::Mouse;
				}

				break;

			case 0x00000001: case 0x10000001:
				if (g_reportMouseEvents & MouseReport::PressRelease) {
					mouseState = MouseState::LBUp;
					eventType  = EventType::Mouse;
				}

				break;

			case 0x00000800: case 0x10000800:
				if (g_reportMouseEvents & MouseReport::PressRelease) {
					mouseState = MouseState::RBDown;
					eventType  = EventType::Mouse;
				}

				break;

			case 0x00000400: case 0x10000400:
				if (g_reportMouseEvents & MouseReport::PressRelease) {
					mouseState = MouseState::RBUp;
					eventType  = EventType::Mouse;
				}

				break;

			case 0x00200000:
				if (g_reportMouseEvents & MouseReport::Scroll and not mouseMoved) {
					mouseState = MouseState::ScrollDown;
					eventType  = EventType::Mouse;
				}

				break;

			case 0x00010000:
				if (g_reportMouseEvents & MouseReport::Scroll and not mouseMoved) {
					mouseState = MouseState::ScrollUp;
					eventType  = EventType::Mouse;
				}

				break;

			default: break;
			}

			if (g_reportMouseEvents & MouseReport::MousePlusKey) {
				if (mouseEvent.bstate & BUTTON_SHIFT)
					key = Key::ShiftPlusMouse;
				else if (mouseEvent.bstate & BUTTON_CTRL)
					key = Key::CtrlPlusMouse;
				else if (mouseEvent.bstate & BUTTON_ALT)
					key = Key::AltPlusMouse;
			}
		}

		break;

	// no input
	case static_cast<uint16_t>(-1): break;

	// else assume its a key press
	default: key = in;
	}

	// if event key isnt none, set the event type to key press
	if (key != Key::None and eventType == EventType::None)
		eventType = EventType::KeyPress;

#elif defined(AMARETTO_PLATFORM_WINDOWS)

	// ncurses getch runs refresh automatically, we should keep the same behavior
	// on all platforms
	Screen::Refresh();

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
			if (not ReadConsoleInput(g_stdinHandle, g_inBuffer, INPUT_RECORD_SIZE, &g_eventReadAmount))
				throw std::runtime_error("ReadConsoleInput");
		}

		// a loop to read all events
		for (; g_eventPos < g_eventReadAmount; ++ g_eventPos) {
			switch (g_inBuffer[g_eventPos].EventType) {
			case KEY_EVENT:
				{
					// shorten the input variable
					uint16_t in = g_inBuffer[g_eventPos].Event.KeyEvent.wVirtualKeyCode;

					if (g_inBuffer[g_eventPos].Event.KeyEvent.bKeyDown) {
						switch (in) {
						case VK_RETURN: key = Key::Enter;      break;
						case VK_BACK:   key = Key::Backspace;  break;
						case VK_ESCAPE: key = Key::Escape;     break;
						case VK_DOWN:   key = Key::ArrowDown;  break;
						case VK_UP:     key = Key::ArrowUp;    break;
						case VK_LEFT:   key = Key::ArrowLeft;  break;
						case VK_RIGHT:  key = Key::ArrowRight; break;
						case VK_TAB:    key = Key::Tab;        break;

						case VK_F1:  key = Key::F1;  break;
						case VK_F2:  key = Key::F2;  break;
						case VK_F3:  key = Key::F3;  break;
						case VK_F4:  key = Key::F4;  break;
						case VK_F5:  key = Key::F5;  break;
						case VK_F6:  key = Key::F6;  break;
						case VK_F7:  key = Key::F7;  break;
						case VK_F8:  key = Key::F8;  break;
						case VK_F9:  key = Key::F9;  break;
						case VK_F10: key = Key::F10; break;
						case VK_F11: key = Key::F11; break;
						case VK_F12: key = Key::F12; break;

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
								key = std::toupper(in);
							else
								// else turn it to lower case
								key = std::tolower(in);
						}

						eventType = EventType::KeyPress;

						// saving the current event position
						// (if we found an event, we return it and dont continue the
						// loop, we have to know where we left off)
						++ g_eventPos;

						return Event(eventType, key, g_mousePos, mouseState, mouseMoved);
					}

				l_skip:
					break;
				}

			case FOCUS_EVENT: break;
			case WINDOW_BUFFER_SIZE_EVENT:
				eventType = EventType::WindowResize;
				++ g_eventPos;

				return Event(eventType, key, g_mousePos, mouseState, mouseMoved);

			case MOUSE_EVENT:
				{
					if (not g_reportMouseEvents)
						break;

					Vec2D mousePos(
						g_inBuffer[g_eventPos].Event.MouseEvent.dwMousePosition.X,
						g_inBuffer[g_eventPos].Event.MouseEvent.dwMousePosition.Y
					);

					if (g_mousePos != mousePos) {
						if (g_reportMouseEvents & MouseReport::Movement) {
							mouseMoved = true;

							g_mousePos = mousePos;

							eventType = EventType::Mouse;
						}
					}

					if (g_reportMouseEvents & MouseReport::PressRelease) {
						switch (g_inBuffer[g_eventPos].Event.MouseEvent.dwButtonState) {
						case FROM_LEFT_1ST_BUTTON_PRESSED:
							mouseState = MouseState::LBDown;
							eventType  = EventType::Mouse;

							g_lMouseDown = true;

							break;

						case FROM_LEFT_3RD_BUTTON_PRESSED:
						case RIGHTMOST_BUTTON_PRESSED:
							mouseState = MouseState::RBDown;
							eventType  = EventType::Mouse;

							g_rMouseDown = true;

							break;
						}

						if (eventType != EventType::Mouse) {
							if (g_lMouseDown) {
								g_lMouseDown = false;

								mouseState = MouseState::LBUp;
								eventType  = EventType::Mouse;
							} else if (g_rMouseDown) {
								g_rMouseDown = false;

								mouseState = MouseState::RBUp;
								eventType  = EventType::Mouse;
							}
						}
					}

					if (g_reportMouseEvents & MouseReport::MousePlusKey) {
						DWORD keyState = g_inBuffer[g_eventPos].Event.MouseEvent.dwControlKeyState;
						if (keyState & SHIFT_PRESSED)
							key = Key::ShiftPlusMouse;
						else if (keyState & LEFT_CTRL_PRESSED)
							key = Key::CtrlPlusMouse;
						else if (keyState & LEFT_ALT_PRESSED)
							key = Key::AltPlusMouse;
					}

					if (g_reportMouseEvents & MouseReport::Scroll) {
						if (g_inBuffer[g_eventPos].Event.MouseEvent.dwEventFlags & MOUSE_WHEELED) {
							eventType = EventType::Mouse;

							int8_t dir =
								(g_inBuffer[g_eventPos].Event.MouseEvent.dwButtonState & 0xFF00) >> 8;

							if (dir >= 0)
								mouseState = MouseState::ScrollUp;
							else
								mouseState = MouseState::ScrollDown;
						}
					}

					++ g_eventPos;

					return Event(eventType, key, g_mousePos, mouseState, mouseMoved);
				}

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

	return Event(eventType, key, g_mousePos, mouseState, mouseMoved);
}

void Amaretto::GetEvent(Event &p_event) {
	p_event = GetEvent();
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

void Amaretto::ResetColorPairs() {
	// this function does nothing on windows, we keep it for cross-platformness
#ifdef AMARETTO_PLATFORM_LINUX

	g_colorPair = 0; // reset color pairs so we dont keep the ones we dont need

#endif // AMARETTO_PLATFORM_LINUX
}

Amaretto::size_t Amaretto::Colors() {
#ifdef AMARETTO_PLATFORM_LINUX

	return COLORS;

#elif defined(AMARETTO_PLATFORM_WINDOWS)

	return 16; // right?

#endif // AMARETTO_PLATFORM_WINDOWS
}

bool Amaretto::IsWideChar(char_t p_ch) {
	return p_ch >= g_wCharStart;
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

void Amaretto::Screen::ShowCursor(bool p_show) {
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

void Amaretto::Screen::Clear() {
#ifdef AMARETTO_PLATFORM_LINUX

	erase();

#elif defined(AMARETTO_PLATFORM_WINDOWS)

	for (auto &ch : g_screen) {
		ch.Char.UnicodeChar = ' ';
		ch.Attributes = COLOR_WHITE;
	}

#endif // AMARETTO_PLATFORM_WINDOWS
}

void Amaretto::Screen::Update() {
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

void Amaretto::Screen::Refresh() {
#ifdef AMARETTO_PLATFORM_LINUX

	refresh();

#elif defined(AMARETTO_PLATFORM_WINDOWS)

	// the rectangle we wanna write into (the entire console)
	SMALL_RECT windowRect = {
		0, 0,
		static_cast<SHORT>(g_window->size.x - 1), static_cast<SHORT>(g_window->size.y - 1)
	};

	WriteConsoleOutputW(
		g_screenHandle, g_screen.data(),
		{static_cast<SHORT>(g_window->size.x), static_cast<SHORT>(g_window->size.y)},
		{0, 0},
		&windowRect
	);

	SetConsoleCursorPosition(
		g_stdoutHandle,
		{
			static_cast<SHORT>(g_cursorPos.x),
			static_cast<SHORT>(g_cursorPos.y)
		}
	);

#endif // AMARETTO_PLATFORM_WINDOWS
}

void Amaretto::Screen::DrawRecordBuffer() {
	g_stdCanvas.DrawRecordBuffer();
}

void Amaretto::Screen::RecordDrawing(bool p_record) {
	g_stdCanvas.RecordDrawing(p_record);
}

void Amaretto::Screen::DrawChar(char_t p_ch, const Vec2D &p_pos, bool p_clr) {
	g_stdCanvas.DrawChar(p_ch, p_pos, p_clr);
}

Amaretto::char_t Amaretto::Screen::GetChar(const Vec2D &p_pos) {
	return g_stdCanvas.GetChar(p_pos);
}

Amaretto::char_t Amaretto::Screen::GetCharScreen(const Vec2D &p_pos) {
	return g_stdCanvas.GetCharScreen(p_pos);
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

void Amaretto::Screen::ColorHLine(const Vec2D &p_pos, pos_t p_endX) {
	g_stdCanvas.ColorHLine(p_pos, p_endX);
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

void Amaretto::Screen::ColorVLine(const Vec2D &p_pos, pos_t p_endY) {
	g_stdCanvas.ColorVLine(p_pos, p_endY);
}

void Amaretto::Screen::PrintString(
	const string &p_str, const Vec2D &p_pos, bool p_wrap, bool p_clr
) {
	g_stdCanvas.PrintString(p_str, p_pos, p_wrap, p_clr);
}

void Amaretto::Screen::PrintString(
	const wstring &p_str, const Vec2D &p_pos, bool p_wrap, bool p_clr
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

const Amaretto::Vec2D &Amaretto::Screen::GetCursorPos() {
	return g_stdCanvas.GetCursorPos();
}

const Amaretto::Rect &Amaretto::Screen::GetRect() {
	return g_stdCanvas.GetRect();
}

const Amaretto::Vec2D &Amaretto::Screen::GetPos() {
	return g_stdCanvas.GetPos();
}

const Amaretto::Vec2D &Amaretto::Screen::GetSize() {
	return g_stdCanvas.GetSize();
}

void Amaretto::Screen::SetRecordBuffer(const Buffer &p_buffer) {
	g_stdCanvas.SetRecordBuffer(p_buffer);
}

const Amaretto::Buffer &Amaretto::Screen::GetRecordBuffer() {
	return g_stdCanvas.GetRecordBuffer();
}

const Amaretto::Vec2D &Amaretto::Screen::GetRecordBufferSize() {
	return g_stdCanvas.GetRecordBufferSize();
}

void Amaretto::Screen::SetRecordBufferFillChar(const CharInfo &p_bufferChFill) {
	g_stdCanvas.SetRecordBufferFillChar(p_bufferChFill);
}

const Amaretto::CharInfo &Amaretto::Screen::GetRecordBufferFillChar() {
	return g_stdCanvas.GetRecordBufferFillChar();
}
