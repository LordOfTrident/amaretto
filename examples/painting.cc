#include <array> // std::array

#include "../headers/amaretto.hh"

#define UNDEF 1

using namespace Amaretto;

constexpr std::array<Color, 7> g_availableColors = {
	Color::White,
	Color::Red,
	Color::Yellow,
	Color::Green,
	Color::Cyan,
	Color::Blue,
	Color::Magenta
};
constexpr uint8_t start = 2;

size_t  g_chosenIdx = 0;
Event   g_event;
Vec2D   g_mousePos(0, 0), g_prevMousePos(0, 0);
uint8_t g_draw = false, g_erase = false;
bool    g_running = true, g_update = true, g_showHelp = true;

Canvas g_canvas(Rect(0, 2, UNDEF, UNDEF), true, CharInfo(' ', Color::Black));
Canvas g_colorsMenu(Rect(UNDEF, UNDEF, static_cast<int>(g_availableColors.size()) * 3 + 1, 2));
Canvas g_menu(Rect(0, 0, UNDEF, 2));
Canvas g_helpMenu(Rect(UNDEF, UNDEF, UNDEF, 4));

void RenderHelp() {
	g_helpMenu.SetColors(Color::BrightWhite, Color::Blue);
	g_helpMenu.FillRect(' ');

	Rect rect(Vec2D(0, 0), g_helpMenu.GetSize());

	string line = "LMB - Draw  | ^C - Clear | Scroll - Change Color";
	g_helpMenu.PrintString(line, Vec2D(CalcCenterPos(Vec2D(line.length(), 0), rect).x, 0));

	line = "RMB - Erase | ^H - Show/hide this message       ";
	g_helpMenu.PrintString(line, Vec2D(CalcCenterPos(Vec2D(line.length(), 0), rect).x, 1));

	line = "You can change the color by clicking on the colors in the top";
	g_helpMenu.PrintString(line, Vec2D(CalcCenterPos(Vec2D(line.length(), 0), rect).x, 3));
}

void Render() {
	Screen::SetColors(Color::Black, Color::Blue);
	Screen::FillRect(Char::Transparent2);

	g_canvas.DrawRecordBuffer();

	Screen::SetColors(Color::White, Color::Black);
	Screen::DrawBorder(Border(Color::White), Rect(
		g_canvas.GetPos()  - Vec2D(1, 1),
		g_canvas.GetSize() + Vec2D(2, 2)
	));

	if (g_showHelp)
		RenderHelp();

	g_menu.SetBgColor(Color::Grey);
	g_menu.FillRect(' ');

	g_menu.SetFgColor(Color::White);
	g_menu.PrintString(" ^Q - Quit      ┃", Vec2D(0, 0));
	g_menu.PrintString(" ^H - Help menu ┃", Vec2D(0, 1));

	g_menu.DrawVLine(Char::TVLine, g_colorsMenu.GetPos() - Vec2D(1, 0), 1);

	pos_t pos = 0;
	for (auto color : g_availableColors) {
		bool chosen = color == g_availableColors[g_chosenIdx];
		if (chosen) {
			g_colorsMenu.SetColors(Color::BrightCyan, Color::Grey);
			g_colorsMenu.DrawChar(' ', Vec2D(pos, 0));
			g_colorsMenu.PrintString(L"━━", Vec2D(pos + 1, 1));

			g_colorsMenu.SetBgColor(ColorToBright(color));
			g_colorsMenu.DrawHLine(' ', Vec2D(pos + 1, 0), pos + 2);
		} else {
			g_colorsMenu.SetColors(Color::Grey, color);
			g_colorsMenu.DrawHLine(Char::Transparent2, Vec2D(pos + 1, 0), pos + 2);
		}

		pos += 3;

		if (chosen) {
			g_colorsMenu.SetColors(Color::BrightCyan, Color::Grey);
			g_colorsMenu.DrawChar(' ', Vec2D(pos, 0));
		}
	}
}

int main() {
	Init();

	WaitForNextEvent(true);
	ReportMouseEvents(MouseReport::All);

	SetTermTitle("Paint program v1");

	Screen::ShowCursor(true);
	Screen::SetCursorPos(Vec2D(2, 2));

	g_canvas.SetSize(GetWindowSize() - Vec2D(2, 4));
	g_canvas.SetFgColor(Color::BrightWhite);

	while (g_running) {
		if (g_draw) {
			g_canvas.SetBgColor(g_availableColors[g_chosenIdx]);

			if (g_draw == start) {
				g_canvas.DrawChar(' ', g_mousePos);

				g_draw = true;
			} else
				g_canvas.DrawLine(' ', g_prevMousePos, g_mousePos);

			if (g_showHelp)
				RenderHelp();
		} else if (g_erase) {
			g_canvas.SetBgColor(Color::Black);

			if (g_erase == start) {
				g_canvas.DrawChar(' ', g_mousePos);

				g_erase = true;
			} else
				g_canvas.DrawLine(' ', g_prevMousePos, g_mousePos);

			if (g_showHelp)
				RenderHelp();
		}

		if (g_update) {
			g_canvas.CenterPos(Rect(Vec2D(0, 1), GetWindowSize()));

			g_menu.SetSize(Vec2D(GetWindowSize().x, 2));
			g_colorsMenu.SetPos(Vec2D(GetWindowSize().x - g_colorsMenu.GetSize().x, 0));

			g_helpMenu.SetSize(Vec2D(GetWindowSize().x, g_helpMenu.GetSize().y));
			g_helpMenu.SetPos(Vec2D(0, GetWindowSize().y - g_helpMenu.GetSize().y));

			Render();

			g_update = false;
		}

		g_event = GetEvent();

		switch (g_event.GetEventType()) {
		case EventType::WindowResize:
			Screen::Update();

			g_update = true;

			break;

		case EventType::KeyPress:
			switch (g_event.GetKey()) {
			case CtrlPlus('q'): g_running = false; break;
			case CtrlPlus('h'):
				g_update   = true;
				g_showHelp = not g_showHelp;

				break;

			case CtrlPlus('c'):
				g_update = true;

				g_canvas.SetBgColor(Color::Black);
				g_canvas.FillRect(' ');

				break;

			default: break;
			}

			break;

 		case EventType::Mouse:
			if (g_event.MouseMoved()) {
				g_canvas.SetCursorPos(g_event.GetMousePos() - g_canvas.GetPos());

				g_prevMousePos = g_mousePos;
				g_mousePos     = g_canvas.GetCursorPos() - g_canvas.GetPos();
			}

			switch (g_event.GetMouseState()) {
			case MouseState::LBDown:
				if (not g_event.GetMousePos().IsInRect(g_colorsMenu.GetRect()))
					g_draw = start;

				break;

			case MouseState::LBUp:
				if (g_event.GetMousePos().IsInRect(g_colorsMenu.GetRect())) {
					for (size_t i = 0; i < g_availableColors.size(); ++ i) {
						pos_t pos = GetWindowSize().x - (g_availableColors.size() - i) * 3;

						if (g_event.GetMousePos() == pos or g_event.GetMousePos() == pos + 1) {
							g_update    = true;
							g_chosenIdx = i;

							break;
						}
					}
				}

				g_draw = false;

				break;

			case MouseState::RBDown: g_erase = start; break;
			case MouseState::RBUp:   g_erase = false; break;

			case MouseState::ScrollUp:
				g_update = true;

				if (g_chosenIdx <= 0)
					g_chosenIdx = g_availableColors.size() - 1;
				else
					-- g_chosenIdx;

				break;

			case MouseState::ScrollDown:
				g_update = true;

				if (g_chosenIdx >= g_availableColors.size() - 1)
					g_chosenIdx = 0;
				else
					++ g_chosenIdx;

				break;
			}

			break;
		}
	}

	Finish();

	return 0;
}
