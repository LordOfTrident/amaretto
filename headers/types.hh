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

#	define COLORS_COUNT 17

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

#	define COLOR_BACKGROUND_OFFSET 16

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

	using pos_t  = int32_t;
	using char_t = uint8_t;

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

		// if more colors are added to the enum, the linux COLORS_COUNT macro has to
		// be changed too
	}; // enum class Color

	struct Rect;
	struct Vec2D {
		Vec2D(pos_t p_x = 0, pos_t p_y = 0);
		Vec2D(size_t p_x, size_t p_y);

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

	struct Border {
		Border(
			char_t p_tlChar = '+',
			char_t p_trChar = '+',
			char_t p_brChar = '+',
			char_t p_blChar = '+',
			char_t p_hChar  = '-',
			char_t p_vChar  = '|'
		);

		char_t tlChar, trChar, brChar, blChar, hChar, vChar;
	}; // struct Border

	struct CharInfo {
		CharInfo(char_t p_ch, Color p_fg, Color p_bg);
		CharInfo(char_t p_ch, Color p_clr = Color::Default);

		char_t ch;
		Color fg, bg;
	};

	enum class Dir : uint8_t {
		Auto,
		Horiz,
		Vert
	};

	class Pattern {
	public:
		Pattern(const std::vector<CharInfo> &p_patt, Dir p_dir = Dir::Auto);
		Pattern(const Vec2D &p_size, const std::vector<CharInfo> &p_patt, Dir p_dir = Dir::Auto);
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

		void StrFrom(const Vec2D &p_pos, const string &p_str, Color p_fg, Color p_bg);

		CharInfo       &operator [](size_t p_idx);
		const CharInfo &operator [](size_t p_idx) const;

		CharInfo       &At(const Vec2D &p_pos);
		const CharInfo &At(const Vec2D &p_pos) const;

		const std::vector<CharInfo> &GetVector() const;
		void SetVector(const std::vector<CharInfo> &p_patt);

		// because of range-based for loops

		auto begin();
		auto end();

		auto begin() const;
		auto end() const;

	private:
		Vec2D m_size;
		std::vector<CharInfo> m_patt;

		Dir m_dir;
	}; // class Pattern
}

#endif // not TYPES_HH__HEADER_GUARD__
