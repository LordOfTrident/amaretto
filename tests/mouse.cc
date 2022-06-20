#include "../headers/amaretto.hh"

using namespace Amaretto;
constexpr int g_availableColors = 7;

int main() {
	Init();

	CBreak(true);
	WaitForNextEvent(true);
	ReportMouseEvents(MouseReport::All);

	SetTermTitle("Paint program v1");

	Screen::ShowCursor(true);

	Event g_event;
	Vec2D g_mousePos(0, 0);
	bool g_draw = false, g_erase = false, g_running = true;

	while (g_running) {
		g_event = GetEvent();

		switch (g_event.GetEventType()) {
		case EventType::WindowResize:
			Screen::Update();

			break;

		case EventType::Mouse:
			if (g_event.MouseMoved()) {
				g_mousePos = g_event.GetMousePos();

				Screen::SetCursorPos(g_mousePos);
			}

			switch (g_event.GetMouseState()) {
			case MouseState::LBDown: g_draw = true; break;
			case MouseState::LBUp:   g_draw = false; break;

			case MouseState::RBDown: g_erase = true;  break;
			case MouseState::RBUp:   g_erase = false; break;
			}

			break;
		}

		if (g_draw) {
			Screen::SetBgColor(Color::White);
			Screen::DrawChar(' ', g_mousePos);
		} else if (g_erase) {
			Screen::SetBgColor(Color::Default);
			Screen::DrawChar(' ', g_mousePos);
		}
	}

	Finish();

	return 0;
}
