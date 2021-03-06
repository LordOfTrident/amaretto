#ifndef CANVAS_HH__HEADER_GUARD__
#define CANVAS_HH__HEADER_GUARD__

#include <cmath>      // std::abs, std::atan2
#include <algorithm>  // std::swap
#include <functional> // std::function

#include "types.hh"

namespace Amaretto {

	class Canvas {
	public:
		Canvas(
			const Rect &p_rect, bool p_record = false,
			const CharInfo &p_bufferFillCh = CharInfo(' ')
		);

		void DrawRecordBuffer();
		void RecordDrawing(bool p_record);

		void   DrawChar(char_t p_ch, Vec2D p_pos, bool p_clr = true);
		char_t GetChar(Vec2D p_pos) const;
		char_t GetCharScreen(Vec2D p_pos) const;

		void ColorChar(Vec2D p_pos);

		void DrawHLine(char_t p_ch, Vec2D p_pos, pos_t p_endX, bool p_clr = true);
		void DrawHLine(const string &p_str, Vec2D p_pos, pos_t p_endX, bool p_clr = true);
		void DrawHLine(const Pattern &p_patt, Vec2D p_pos, pos_t p_endX);

		void ColorHLine(Vec2D p_pos, pos_t p_endX);

		void DrawVLine(char_t p_ch, Vec2D p_pos, pos_t p_endY, bool p_clr = true);
		void DrawVLine(const string &p_str, Vec2D p_pos, pos_t p_endY, bool p_clr = true);
		void DrawVLine(const Pattern &p_patt, Vec2D p_pos, pos_t p_endY);

		void ColorVLine(Vec2D p_pos, pos_t p_endY);

		void PrintString(const string &p_str, Vec2D p_pos, bool p_cut = true, bool p_clr = true);
		void PrintString(const wstring &p_str, Vec2D p_pos, bool p_cut = true, bool p_clr = true);
		void PrintString(size_t p_num, const Vec2D &p_pos, bool p_cut = true, bool p_clr = true);

		void FillRect(char_t p_ch, Rect p_rect, bool p_clr = true);
		void FillRect(const string &p_str, Rect p_rect, bool p_clr = true);
		void FillRect(const Pattern &p_patt, Rect p_rect);

		void ColorFillRect(Rect p_rect);

		void FillRect(char_t p_ch, bool p_clr = true);
		void FillRect(const string &p_str, bool p_clr = true);
		void FillRect(const Pattern &p_patt);

		void ColorFillRect();

		void DrawRect(char_t p_ch, Rect p_rect, bool p_clr = true);
		void DrawRect(const string &p_str, Rect p_rect, bool p_clr = true);
		void DrawRect(const Pattern &p_patt, Rect p_rect);

		void ColorRect(Rect p_rect);

		void DrawRect(char_t p_ch, bool p_clr = true);
		void DrawRect(const string &p_str, bool p_clr = true);
		void DrawRect(const Pattern &p_patt);

		void ColorRect();

		// TODO: make borders have unique colors for each side, corner
		void DrawBorder(const Border &p_border, Rect p_rect, bool p_clr = true);
		void DrawBorder(const Border &p_border, bool p_clr = true);

		void DrawLine(char_t p_ch, Vec2D p_posA, Vec2D p_posB, bool p_clr = true);
		void DrawLine(const string &p_str, Vec2D p_posA, Vec2D p_posB, bool p_clr = true);
		void DrawLine(const Pattern &p_patt, Vec2D p_posA, Vec2D p_posB);

		void ColorLine(Vec2D p_posA, Vec2D p_posB);

		void SetColors(Color p_fg, Color p_bg);
		void SetFgColor(Color p_fg);
		void SetBgColor(Color p_bg);

		Color GetFgColor() const;
		Color GetBgColor() const;

		void         SetCursorPos(Vec2D p_pos);
		const Vec2D &GetCursorPos() const;

		void        SetRect(const Rect &p_rect);
		const Rect &GetRect() const;

		void         SetPos(const Vec2D &p_pos);
		const Vec2D &GetPos() const;
		void CenterPos(const Rect &p_rect);

		void         SetSize(const Vec2D &p_size);
		const Vec2D &GetSize() const;

		void SetRecordBuffer(const Buffer &p_buffer);
		const Buffer &GetRecordBuffer() const;

		const Vec2D &GetRecordBufferSize() const;

		void SetRecordBufferFillChar(const CharInfo &p_bufferFillCh);
		const CharInfo &GetRecordBufferFillChar() const;

	private:
		using drawFunc_t = std::function<void(Canvas&, char_t, const Vec2D&)>;

		void   SetCharUnsafe(char_t p_ch, const Vec2D &p_pos);
		void   ChangeCharUnsafe(char_t p_ch, const Vec2D &p_pos);
		char_t GetCharUnsafe(const Vec2D &p_pos) const;
		char_t GetCharScreenUnsafe(const Vec2D &p_pos) const;
		void   ColorCharUnsafe(const Vec2D &p_pos);

#ifdef AMARETTO_PLATFORM_LINUX

		uint8_t CheckForColorPair(Color p_fg, Color p_bg);

#endif // AMARETTO_PLATFORM_LINUX

		Color m_fg, m_bg;

#ifdef AMARETTO_PLATFORM_LINUX

		short m_currColorPair;

#elif defined(AMARETTO_PLATFORM_WINDOWS)

		WORD m_currAttr;

#endif // AMARETTO_PLATFORM_WINDOWS

		bool     m_record;
		Buffer   m_buffer;
		Vec2D    m_bufferSize;
		CharInfo m_bufferFillCh;

		Rect m_rect;
	}; // class Canvas
}

#endif // not CANVAS_HH__HEADER_GUARD__
