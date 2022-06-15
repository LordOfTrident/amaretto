#include "../headers/amaretto.hh"

using namespace Amaretto;

int main() {
	Init();

	CBreak(true);
	//WaitForNextEvent(true); // this is true on default

	SetTermTitle("Amaretto test program");
	ShowCursor(false);

	Pattern pattern1(Vec2D(8, 4), {
		CharInfo(' ', Color::White, Color::White),
		CharInfo(' ', Color::White, Color::White),
		CharInfo(' ', Color::White, Color::Black),
		CharInfo(' ', Color::White, Color::Black),

		CharInfo(' ', Color::White, Color::Blue),
		CharInfo(' ', Color::White, Color::Blue),
		CharInfo(' ', Color::White, Color::Red),
		CharInfo(' ', Color::White, Color::Red),

		CharInfo(' ', Color::White, Color::Black),
		CharInfo(' ', Color::White, Color::Black),
		CharInfo(' ', Color::White, Color::White),
		CharInfo(' ', Color::White, Color::White),

		CharInfo(' ', Color::White, Color::Red),
		CharInfo(' ', Color::White, Color::Red),
		CharInfo(' ', Color::White, Color::Blue),
		CharInfo(' ', Color::White, Color::Blue),


		CharInfo(' ', Color::White, Color::Blue),
		CharInfo(' ', Color::White, Color::Blue),
		CharInfo(' ', Color::White, Color::Red),
		CharInfo(' ', Color::White, Color::Red),

		CharInfo(' ', Color::White, Color::White),
		CharInfo(' ', Color::White, Color::White),
		CharInfo(' ', Color::White, Color::Black),
		CharInfo(' ', Color::White, Color::Black),

		CharInfo(' ', Color::White, Color::Red),
		CharInfo(' ', Color::White, Color::Red),
		CharInfo(' ', Color::White, Color::Blue),
		CharInfo(' ', Color::White, Color::Blue),

		CharInfo(' ', Color::White, Color::Black),
		CharInfo(' ', Color::White, Color::Black),
		CharInfo(' ', Color::White, Color::White),
		CharInfo(' ', Color::White, Color::White)
	});

	Pattern pattern2({
		CharInfo('H', Color::White, Color::Red),
		CharInfo('E', Color::White, Color::Red),
		CharInfo('L', Color::White, Color::Red),
		CharInfo('O', Color::White, Color::Red),

		CharInfo('H', Color::Black, Color::Yellow),
		CharInfo('E', Color::Black, Color::Yellow),
		CharInfo('L', Color::Black, Color::Yellow),
		CharInfo('O', Color::Black, Color::Yellow),

		CharInfo('H', Color::Black, Color::Green),
		CharInfo('E', Color::Black, Color::Green),
		CharInfo('L', Color::Black, Color::Green),
		CharInfo('O', Color::Black, Color::Green),

		CharInfo('H', Color::Black, Color::Cyan),
		CharInfo('E', Color::Black, Color::Cyan),
		CharInfo('L', Color::Black, Color::Cyan),
		CharInfo('O', Color::Black, Color::Cyan)
	}, Dir::Horiz);

	Event event;
	while (event.GetEventType() != EventType::KeyPress) {
		Screen::SetColors(Color::White, Color::BrightWhite);
		Screen::FillRect(". ");

		Canvas imgViewport(Rect(4, 3, 32, 16));
		imgViewport.SetBgColor(Color::Grey);
		imgViewport.FillRect(' ');
		//imgViewport.FillRect(pattern);

		imgViewport.SetFgColor(Color::BrightRed);
		imgViewport.DrawRect(pattern2);

		//imgViewport.FillRect("HI-", Rect(5, 3, 20, 10));
		imgViewport.FillRect(pattern1, Rect(5, 3, 22, 10));

		imgViewport.SetColors(Color::BrightYellow, Color::Blue);
		//imgViewport.DrawHLine("Hello ", Vec2D(2, 1), 21);
		imgViewport.DrawHLine(pattern2, Vec2D(2, 1), 21);
		imgViewport.DrawVLine(pattern2, Vec2D(2, 1), 10);

		imgViewport.DrawLine(pattern2, Vec2D(6, 3), Vec2D(15, 16));
		imgViewport.DrawLine(pattern2, Vec2D(6, 3), Vec2D(32, 8));
		imgViewport.DrawLine(pattern2, Vec2D(6, 3), Vec2D(6, 16));
		imgViewport.DrawLine(pattern2, Vec2D(6, 3), Vec2D(32, 3));

		imgViewport.DrawLine("Epic ", Vec2D(6, 3), Vec2D(32, 16), false);

		Screen::SetFgColor(Color::Grey);
		Screen::PrintString("Amaretto test v1", Vec2D(11, 1));
		Screen::PrintString("Press any key to exit", Vec2D(9, 20));

		Screen::SetFgColor(Color::Black);
		Screen::PrintString(1024, Vec2D(0, 0));

		Screen::DrawChar('A', Vec2D(3, 3), false);
		Screen::DrawChar('A', Vec2D(4, 3), false);

		imgViewport.SetColors(Color::BrightWhite, Color::Magenta);
		imgViewport.ColorFillRect(Rect(26, 12, 8, 5));
		imgViewport.FillRect(pattern1, Rect(-4, -2, 8, 6));

		event = GetEvent();
		Update();
	}

	Finish();

	return 0;
}
