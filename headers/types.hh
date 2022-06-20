#ifndef TYPES_HH__HEADER_GUARD__
#define TYPES_HH__HEADER_GUARD__

#include <cstdlib> // std::size_t
#include <cstdint> // std::uint8_t, std::int8_t, std::uint16_t, std::int16_t, std::uint32_t,
                   // std::int32_t
#include <string>  // std::string
#include <utility> // std::pair, std::make_pair
#include <vector>  // std::vector

#include "platform.hh"

#ifdef AMARETTO_PLATFORM_LINUX
#	include <ncurses.h> // terminal I/O

#	ifdef NCURSES_WIDECHAR
#		define AMARETTO_WIDECHAR
#	else
#		pragma message "ncursesw is not available, this means that not all wide chars will work."
#		pragma message "please use `make ... CXX_LIBS=-lncurses` to compile"
#	endif // NCURSES_WIDECHAR

#	define COLOR_GREY          8
#	define COLOR_BRIGHTRED     9
#	define COLOR_BRIGHTGREEN   10
#	define COLOR_BRIGHTYELLOW  11
#	define COLOR_BRIGHTBLUE    12
#	define COLOR_BRIGHTMAGENTA 13
#	define COLOR_BRIGHTCYAN    14
#	define COLOR_BRIGHTWHITE   15
#elif defined(AMARETTO_PLATFORM_WINDOWS)
#	include <windows.h> // windows API

#	define AMARETTO_WIDECHAR

#	define COLOR_BLACK    0
#	define COLOR_RED      FOREGROUND_RED
#	define COLOR_GREEN    FOREGROUND_GREEN
#	define COLOR_YELLOW  (FOREGROUND_GREEN | FOREGROUND_RED)
#	define COLOR_BLUE     FOREGROUND_BLUE
#	define COLOR_MAGENTA (FOREGROUND_BLUE  | FOREGROUND_RED)
#	define COLOR_CYAN    (FOREGROUND_BLUE  | FOREGROUND_GREEN)
#	define COLOR_WHITE   (FOREGROUND_RED   | FOREGROUND_GREEN | FOREGROUND_BLUE)

#	define COLOR_GREY          (COLOR_BLACK   | FOREGROUND_INTENSITY)
#	define COLOR_BRIGHTRED     (COLOR_RED     | FOREGROUND_INTENSITY)
#	define COLOR_BRIGHTGREEN   (COLOR_GREEN   | FOREGROUND_INTENSITY)
#	define COLOR_BRIGHTYELLOW  (COLOR_YELLOW  | FOREGROUND_INTENSITY)
#	define COLOR_BRIGHTBLUE    (COLOR_BLUE    | FOREGROUND_INTENSITY)
#	define COLOR_BRIGHTMAGENTA (COLOR_MAGENTA | FOREGROUND_INTENSITY)
#	define COLOR_BRIGHTCYAN    (COLOR_CYAN    | FOREGROUND_INTENSITY)
#	define COLOR_BRIGHTWHITE   (COLOR_WHITE   | FOREGROUND_INTENSITY)
#endif // AMARETTO_PLATFORM_WINDOWS

namespace Amaretto {
	using std::uint8_t;
	using std::int8_t;

	using std::uint16_t;
	using std::int16_t;

	using std::uint32_t;
	using std::int32_t;

	using std::size_t;

	using std::string;
	using std::wstring;

	using pos_t  = int32_t;
	using char_t = wchar_t;

	namespace Key {
		enum Key : uint16_t {
			None = 0,

			Backspace = 127,
			Enter     = 10,
			Escape    = 27,
			Tab       = 9,

			ArrowDown = 258,
			ArrowUp,
			ArrowLeft,
			ArrowRight,

			CtrlPlusMouse, // these 3 keys wont work normally, only with mouse events
			AltPlusMouse,
			ShiftPlusMouse,

			F1 = 262,
			F2,
			F3,
			F4,
			F5,
			F6,
			F7,
			F8,
			F9,
			F10,
			F11,
			F12,
		}; // enum Key
	} // namespace Key

	constexpr uint16_t CtrlPlus(uint16_t p_key) {
		return p_key & 31;
	}

	constexpr uint16_t ShiftPlus(uint16_t p_key) {
		return (p_key >= 'a' && p_key <= 'z')? p_key - 32 : p_key;
	}

	constexpr uint16_t CharToKey(char_t p_key) {
		return (p_key >= 'A' && p_key <= 'Z')? p_key + 32 : p_key;
	}

	namespace Char {
		enum Char : char_t {
			TLCorner = 0x250c,
			BLCorner = 0x2514,
			TRCorner = 0x2510,
			BRCorner = 0x2518,

			LConnect = 0x2524,
			RConnect = 0x251c,
			BConnect = 0x252c,
			TConnect = 0x2534,

			HLine = 0x2500,
			VLine = 0x2502,

			Plus = 0x253c,

			DTLCorner = 0x2554,
			DBLCorner = 0x255a,
			DTRCorner = 0x2557,
			DBRCorner = 0x255d,

			DLConnect = 0x2563,
			DRConnect = 0x2560,
			DBConnect = 0x2566,
			DTConnect = 0x2569,

			DHLine = 0x2550,
			DVLine = 0x2551,

			DPlus = 0x256c,

			TTLCorner = 0x250f,
			TBLCorner = 0x2517,
			TTRCorner = 0x2513,
			TBRCorner = 0x251b,

			TLConnect = 0x252b,
			TRConnect = 0x2523,
			TBConnect = 0x2533,
			TTConnect = 0x253b,

			THLine = 0x2501,
			TVLine = 0x2503,

			TPlus = 0x254b,

			ScanLine5 = 0x23ba,
			ScanLine4,
			ScanLine3,
			ScanLine2,
			ScanLine1 = 95,

			LArrow = 0x2190,
			UArrow,
			RArrow,
			DArrow,

			Snowman = 0x2603,
			Diamond = 0x25c6,
			Degree  = 0xb0,
			Bullet  = 0xb7,
			PlusMinus    = 0xb1,
			LessEqual    = 0x2264,
			GreaterEqual = 0x2265,
			NotEqual     = 0x2260,
			PI    = 0x3c0,
			Pound = 0xa3,

			Block = 0x2588,

			Transparent3 = 0x2591,
			Transparent2,
			Transparent1,

			TRToBL = 0x2571,
			TLToBR,
			Cross,

			RTLCorner = 0x256d,
			RTRCorner,
			RBRCorner,
			RBLCorner,

			BLBox = 0x2596,
			BRBox,
			TLBox,
			TRCutOut,
			Checkerboard1,
			BRCutOut,
			BLCutOut,
			TRBox,
			Checkerboard2,
			TLCutOut,

			BigDot = 0x25cf,
			SmallDot = 0x2219
		}; // enum Char
	} // namespace Char

	enum class Color : int16_t {
		Default = -1,

		Black   = COLOR_BLACK,
		Blue    = COLOR_BLUE,
		Green   = COLOR_GREEN,
		Cyan    = COLOR_CYAN,
		Red     = COLOR_RED,
		Magenta = COLOR_MAGENTA,
		Yellow  = COLOR_YELLOW,
		White   = COLOR_WHITE,

		Grey          = COLOR_GREY,
		BrightBlue    = COLOR_BRIGHTBLUE,
		BrightGreen   = COLOR_BRIGHTGREEN,
		BrightCyan    = COLOR_BRIGHTCYAN,
		BrightRed     = COLOR_BRIGHTRED,
		BrightMagenta = COLOR_BRIGHTMAGENTA,
		BrightYellow  = COLOR_BRIGHTYELLOW,
		BrightWhite   = COLOR_BRIGHTWHITE
	}; // enum class Color

	constexpr Color ColorToBright(Color p_color) {
		switch (p_color) {
		case Color::Black:   return Color::Grey;
		case Color::Blue:    return Color::BrightBlue;
		case Color::Green:   return Color::BrightGreen;
		case Color::Cyan:    return Color::BrightCyan;
		case Color::Red:     return Color::BrightRed;
		case Color::Magenta: return Color::BrightMagenta;
		case Color::Yellow:  return Color::BrightYellow;
		case Color::White:   return Color::BrightWhite;
		default: return Color::Default;
		}
	}

	struct Rect;
	struct Vec2D {
		Vec2D(pos_t p_x = 0, pos_t p_y = 0);
		Vec2D(size_t p_x, size_t p_y);
		Vec2D(int p_x, size_t p_y);
		Vec2D(size_t p_x, int p_y);

		bool operator <(const Rect &p_rect) const;
		bool operator >(const Rect &p_rect) const;

		bool operator ==(const Vec2D &p_pos) const;
		bool operator !=(const Vec2D &p_pos) const;

		Vec2D operator +(const Vec2D &p_pos) const;
		Vec2D operator -(const Vec2D &p_pos) const;
		Vec2D operator *(const Vec2D &p_pos) const;
		Vec2D operator /(const Vec2D &p_pos) const;

		Vec2D& operator +=(const Vec2D &p_a);
		Vec2D& operator -=(const Vec2D &p_a);
		Vec2D& operator *=(const Vec2D &p_a);
		Vec2D& operator /=(const Vec2D &p_a);

		bool IsInRect(const Rect &p_rect) const;

		pos_t x, y;
	}; // struct Vec2D

	struct Rect {
		Rect(Vec2D p_pos = Vec2D(), Vec2D p_size = Vec2D());

		Rect(pos_t p_posx = 0, pos_t p_posy = 0, pos_t p_sizex = 0, pos_t p_sizey = 0);
		Rect(size_t p_posx, size_t p_posy, size_t p_sizex, size_t p_sizey);

		bool operator ==(const Rect &p_rect) const;
		bool operator !=(const Rect &p_rect) const;

		Vec2D pos, size;
	}; // struct Rect

	inline Vec2D CalcCenterPos(const Vec2D &p_size, const Rect &p_rect) {
		return Vec2D(
			p_rect.size.x / 2 - p_size.x / 2 + p_rect.pos.x,
			p_rect.size.y / 2 - p_size.y / 2 + p_rect.pos.y
		);
	}

	struct Border {
		Border(
			Color  p_shadeColor = Color::BrightWhite,
			char_t p_tlChar = Char::TLCorner,
			char_t p_trChar = Char::TRCorner,
			char_t p_brChar = Char::BRCorner,
			char_t p_blChar = Char::BLCorner,
			char_t p_hChar  = Char::HLine,
			char_t p_vChar  = Char::VLine
		);

		Color shadeColor;

		char_t tlChar, trChar, brChar, blChar, hChar, vChar;
	}; // struct Border

	struct CharInfo {
		CharInfo(char_t p_ch, Color p_fg, Color p_bg);
		CharInfo(char_t p_ch, Color p_clr = Color::Default);

		char_t ch;
		Color fg, bg;
	};

	using Buffer = std::vector<std::vector<CharInfo>>;

	enum class Dir : uint8_t {
		Auto,
		Horizontal,
		Vertical
	};

	class Pattern {
	public:
		Pattern(const Buffer &p_patt, Dir p_dir = Dir::Auto);
		Pattern(const Vec2D &p_size, const Buffer &p_patt, Dir p_dir = Dir::Auto);
		Pattern(const Vec2D &p_size, Dir p_dir = Dir::Auto);

		void SetDir(Dir p_dir);
		Dir  GetDir() const;

		auto Begin();
		auto End();

		auto Begin() const;
		auto End() const;

		size_t Length() const;

		Vec2D GetSize() const;
		void  SetSize(const Vec2D &p_size);

		CharInfo       &operator [](const Vec2D &p_pos);
		const CharInfo &operator [](const Vec2D &p_pos) const;

		CharInfo       &At(const Vec2D &p_pos);
		const CharInfo &At(const Vec2D &p_pos) const;

		const Buffer &GetVector() const;
		void SetVector(const Buffer &p_patt);

		// because of range-based for loops

		auto begin();
		auto end();

		auto begin() const;
		auto end() const;

	private:
		Vec2D m_size;
		Buffer m_patt;

		Dir m_dir;
	}; // class Pattern

	enum class EventType {
		None = 0,

		KeyPress,
		WindowResize,
		Mouse
	}; // enum class EventType

	enum class MouseState {
		None = 0,

		LBDown,
		LBUp,

		RBDown,
		RBUp,

		MBDown,
		MBUp,

		ScrollDown,
		ScrollUp
	}; // enum class MouseState

	enum class MouseReport : uint8_t {
		None = 0,
		All  = 0xFF,
		PressRelease = 1 << 0,
		Movement     = 1 << 1,
		Scroll       = 1 << 2,
		MousePlusKey = 1 << 3
	}; // enum class MouseReport

	constexpr uint8_t operator &(MouseReport p_a, MouseReport p_b) {
		return static_cast<uint8_t>(p_a) & static_cast<uint8_t>(p_b);
	}

	constexpr MouseReport operator |(MouseReport p_a, MouseReport p_b) {
		return static_cast<MouseReport>(static_cast<uint8_t>(p_a) | static_cast<uint8_t>(p_b));
	}

	constexpr bool operator not(MouseReport p_a) {
		return not static_cast<uint8_t>(p_a);
	}

	class Event {
	public:
		Event(
			EventType p_eventType = EventType::None,
			uint16_t p_key = Key::None,
			const Vec2D &p_mousePos = Vec2D(0, 0),
			MouseState p_mouseState = MouseState::None,
			bool p_mouseMoved = false
		);

		EventType GetEventType() const;

		uint16_t GetKey() const;

		const Vec2D &GetMousePos() const;
		MouseState   GetMouseState() const;
		bool MouseMoved() const;

	private:
		EventType m_eventType;

		uint16_t   m_key;
		Vec2D      m_mousePos;
		MouseState m_mouseState;
		bool       m_mouseMoved;
	}; // class Event
}

#endif // not TYPES_HH__HEADER_GUARD__
