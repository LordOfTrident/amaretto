#ifndef IOHANDLE_HH__HEADER_GUARD__
#define IOHANDLE_HH__HEADER_GUARD__

#include <clocale>   // std::setlocale, LC_CTYPE
#include <stdexcept> // std::runtime_error

#include "canvas.hh"

namespace Amaretto {
	enum class EventType {
		None = 0,
		KeyPress,
		WindowResize,
		Mouse
	}; // enum class EventType

	class Event {
	public:
		Event();

		EventType GetEventType();
		void      SetEventType(EventType p_type);

		uint16_t GetKey();
		void     SetKey(uint16_t p_key);

	private:
		EventType m_eventType;
		uint16_t m_key;
	}; // class Event

	namespace Key {
		enum Key : uint16_t {
			None      = 0,
			Backspace = 127,
			Enter     = 10,
			Escape    = 27,
			Tab       = 9,
			ArrowDown = 258,
			ArrowUp,
			ArrowLeft,
			ArrowRight
		}; // enum Key

		constexpr uint16_t Ctrl(uint16_t p_key) {
			return p_key & 31;
		}

		constexpr uint16_t Shift(uint16_t p_key) {
			return (p_key >= 'a' && p_key <= 'z')? p_key - 32 : p_key;
		}

		constexpr uint16_t Key(char_t p_key) {
			return (p_key >= 'A' && p_key <= 'Z')? p_key + 32 : p_key;
		}
	} // namespace Key

	// main functions

	void Init();
	void Finish();

	void WaitForNextEvent(bool p_wait);
	void CBreak(bool p_cBreak);

	void Delay(size_t p_ms);
	void SetTermTitle(const string &p_title);

	Event GetEvent();

	void ShowCursor(bool p_show);

	bool CanChangeColors();
	bool CanUseColors();
	void ResetColorPairs();
	size_t Colors();

	void Clear();
	void Update();
	void Refresh();

	uint16_t GetLastErrorID();

	const Vec2D &GetWindowSize();

	namespace Screen {
		void DrawChar(char_t p_ch, const Vec2D &p_pos, bool p_clr = true);
		char_t GetChar(const Vec2D &p_pos);

		void DrawHLine(char_t p_ch, const Vec2D &p_pos, pos_t p_endX, bool p_clr = true);
		void DrawHLine(const string &p_str, const Vec2D &p_pos, pos_t p_endX, bool p_clr = true);
		void DrawHLine(const Pattern &p_patt, const Vec2D &p_pos, pos_t p_endX);

		void DrawVLine(char_t p_ch, const Vec2D &p_pos, pos_t p_endY, bool p_clr = true);
		void DrawVLine(const string &p_str, const Vec2D &p_pos, pos_t p_endY, bool p_clr = true);
		void DrawVLine(const Pattern &p_patt, const Vec2D &p_pos, pos_t p_endY);

		void PrintString(
			const string &p_str, const Vec2D &p_pos, bool p_wrap = false, bool p_clr = true
		);
		void PrintString(size_t p_num, const Vec2D &p_pos, bool p_wrap = false, bool p_clr = true);

		void FillRect(char_t p_ch, const Rect &p_rect, bool p_clr = true);
		void FillRect(const string &p_str, const Rect &p_rect, bool p_clr = true);
		void FillRect(const Pattern &p_patt, const Rect &p_rect);

		void FillRect(char_t p_ch, bool p_clr = true);
		void FillRect(const string &p_str, bool p_clr = true);
		void FillRect(const Pattern &p_patt);

		void DrawRect(char_t p_ch, const Rect &p_rect, bool p_clr = true);
		void DrawRect(const string &p_str, const Rect &p_rect, bool p_clr = true);
		void DrawRect(const Pattern &p_patt, const Rect &p_rect);

		void DrawRect(char_t p_ch, bool p_clr = true);
		void DrawRect(const string &p_str, bool p_clr = true);
		void DrawRect(const Pattern &p_patt);

		void DrawBorder(const Border &p_border, const Rect &p_rect, bool p_clr = true);
		void DrawBorder(const Border &p_border, bool p_clr = true);

		void DrawLine(char_t p_ch, const Vec2D &p_posA, const Vec2D &p_posB, bool p_clr = true);
		void DrawLine(
			const string &p_str, const Vec2D &p_posA, const Vec2D &p_posB, bool p_clr = true
		);
		void DrawLine(const Pattern &p_patt, const Vec2D &p_posA, const Vec2D &p_posB);

		void SetColors(Color p_fg, Color p_bg);
		void SetFgColor(Color p_fg);
		void SetBgColor(Color p_bg);

		Color GetFgColor();
		Color GetBgColor();

		void SetCursorPos(const Vec2D &p_pos);

		const Rect &GetRect();
	} // namespace Screen
} // namespace Amaretto

#endif // not IOHANDLE_HH__HEADER_GUARD__
