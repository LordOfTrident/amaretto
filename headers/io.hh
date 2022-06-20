#ifndef IOHANDLE_HH__HEADER_GUARD__
#define IOHANDLE_HH__HEADER_GUARD__

#include <clocale>   // std::setlocale, LC_CTYPE
#include <stdexcept> // std::runtime_error

#include "canvas.hh"

namespace Amaretto {
	void Init(const string &p_locale = "");
	void Finish();

	void WaitForNextEvent(bool p_wait);
	void CBreak(bool p_cBreak);
	void ReportMouseEvents(MouseReport p_report);

	void Delay(size_t p_ms);
	void SetTermTitle(const string &p_title);


	Event GetEvent();
	void  GetEvent(Event &p_event);

	bool CanChangeColors();
	bool CanUseColors();
	void ResetColorPairs();
	size_t Colors();

	bool IsWideChar(char_t p_ch);

	uint16_t GetLastErrorID();

	const Vec2D &GetWindowSize();

	namespace Screen {
		void ShowCursor(bool p_show);

		void Clear();
		void Update();
		void Refresh();

		void DrawRecordBuffer();
		void RecordDrawing(bool p_record);

		void   DrawChar(char_t p_ch, const Vec2D &p_pos, bool p_clr = true);
		char_t GetChar(const Vec2D &p_pos);
		char_t GetCharScreen(const Vec2D &p_pos);

		void DrawHLine(char_t p_ch, const Vec2D &p_pos, pos_t p_endX, bool p_clr = true);
		void DrawHLine(const string &p_str, const Vec2D &p_pos, pos_t p_endX, bool p_clr = true);
		void DrawHLine(const Pattern &p_patt, const Vec2D &p_pos, pos_t p_endX);

		void ColorHLine(const Vec2D &p_pos, pos_t p_endX);

		void DrawVLine(char_t p_ch, const Vec2D &p_pos, pos_t p_endY, bool p_clr = true);
		void DrawVLine(const string &p_str, const Vec2D &p_pos, pos_t p_endY, bool p_clr = true);
		void DrawVLine(const Pattern &p_patt, const Vec2D &p_pos, pos_t p_endY);

		void ColorVLine(const Vec2D &p_pos, pos_t p_endY);

		void PrintString(
			const string &p_str, const Vec2D &p_pos, bool p_wrap = false, bool p_clr = true
		);
		void PrintString(
			const wstring &p_str, const Vec2D &p_pos, bool p_wrap = false, bool p_clr = true
		);
		void PrintString(size_t p_num, const Vec2D &p_pos, bool p_wrap = false, bool p_clr = true);

		void FillRect(char_t p_ch, const Rect &p_rect, bool p_clr = true);
		void FillRect(const string &p_str, const Rect &p_rect, bool p_clr = true);
		void FillRect(const Pattern &p_patt, const Rect &p_rect);

		void ColorFillRect(Rect p_rect);

		void FillRect(char_t p_ch, bool p_clr = true);
		void FillRect(const string &p_str, bool p_clr = true);
		void FillRect(const Pattern &p_patt);

		void ColorFillRect();

		void DrawRect(char_t p_ch, const Rect &p_rect, bool p_clr = true);
		void DrawRect(const string &p_str, const Rect &p_rect, bool p_clr = true);
		void DrawRect(const Pattern &p_patt, const Rect &p_rect);

		void ColorRect(Rect p_rect);

		void DrawRect(char_t p_ch, bool p_clr = true);
		void DrawRect(const string &p_str, bool p_clr = true);
		void DrawRect(const Pattern &p_patt);

		void ColorRect();

		void DrawBorder(const Border &p_border, const Rect &p_rect, bool p_clr = true);
		void DrawBorder(const Border &p_border, bool p_clr = true);

		void DrawLine(char_t p_ch, const Vec2D &p_posA, const Vec2D &p_posB, bool p_clr = true);
		void DrawLine(
			const string &p_str, const Vec2D &p_posA, const Vec2D &p_posB, bool p_clr = true
		);
		void DrawLine(const Pattern &p_patt, const Vec2D &p_posA, const Vec2D &p_posB);

		void ColorLine(Vec2D p_posA, Vec2D p_posB);

		void SetColors(Color p_fg, Color p_bg);
		void SetFgColor(Color p_fg);
		void SetBgColor(Color p_bg);

		Color GetFgColor();
		Color GetBgColor();

		void         SetCursorPos(const Vec2D &p_pos);
		const Vec2D &GetCursorPos();

		const Rect  &GetRect();
		const Vec2D &GetPos();
		const Vec2D &GetSize();

		void SetRecordBuffer(const Buffer &p_buffer);
		const Buffer &GetRecordBuffer();

		const Vec2D &GetRecordBufferSize();

		void SetRecordBufferFillChar(const CharInfo &p_bufferChFill);
		const CharInfo &GetRecordBufferFillChar();
	} // namespace Screen
} // namespace Amaretto

#endif // not IOHANDLE_HH__HEADER_GUARD__
