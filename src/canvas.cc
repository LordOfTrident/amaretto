#include "../headers/canvas.hh"
#include "private.hh"

// class Canvas
// public

Amaretto::Canvas::Canvas(const Rect &p_rect, bool p_record, const CharInfo &p_bufferFillCh):
	m_fg(Color::Default),
	m_bg(Color::Default),

#ifdef AMARETTO_PLATFORM_LINUX

	m_currColorPair(0),

#elif defined(AMARETTO_PLATFORM_WINDOWS)

	m_currAttr(0),

#endif // AMARETTO_PLATFORM_WINDOWS

	m_record(p_record),
	m_bufferSize(p_rect.size),
	m_bufferFillCh(p_bufferFillCh),
	m_rect(p_rect)
{
	m_buffer.resize(m_bufferSize.y, {});

	for (auto &row : m_buffer)
		row.resize(m_bufferSize.x, m_bufferFillCh);
}

void Amaretto::Canvas::DrawRecordBuffer() {
	Vec2D pos(0, 0);

	for (; pos.y < m_bufferSize.y; ++ pos.y) {
		for (pos.x = 0; pos.x < m_bufferSize.x; ++ pos.x) {
			SetColors(m_buffer[pos.y][pos.x].fg, m_buffer[pos.y][pos.x].bg);
			SetCharUnsafe(m_buffer[pos.y][pos.x].ch, pos + m_rect.pos);
		}
	}
}

void Amaretto::Canvas::RecordDrawing(bool p_record) {
	m_record = p_record;

	if (m_record and m_bufferSize != m_rect.size) {
		m_bufferSize = m_rect.size;

		m_buffer.resize(m_bufferSize.y, {});

		for (auto &row : m_buffer)
			row.resize(m_bufferSize.x, m_bufferFillCh);
	}
}

void Amaretto::Canvas::DrawChar(char_t p_ch, Vec2D p_pos, bool p_clr) {
#ifdef AMARETTO_PLATFORM_LINUX

	if (p_clr and g_currColorPair != m_currColorPair) {
		attroff(COLOR_PAIR(g_currColorPair));
		attron(COLOR_PAIR(m_currColorPair));
	}

#endif // AMARETTO_PLATFORM_LINUX

	// convert the canvas position into screen position
	p_pos += m_rect.pos;

	//    p_pos.x > m_rect.pos.x + m_rect.size.x or p_pos.y ...
	// or p_pos.x < m_rect.pos.x  or p_pos.y ...
	if (p_pos > m_rect or p_pos < m_rect)
		return;

	drawFunc_t drawChar = p_clr? &Canvas::SetCharUnsafe : &Canvas::ChangeCharUnsafe;

	drawChar(*this, p_ch, p_pos);

#ifdef AMARETTO_PLATFORM_LINUX

	move(g_cursorPos.y, g_cursorPos.x);

#endif // AMARETTO_PLATFORM_LINUX
}

Amaretto::char_t Amaretto::Canvas::GetChar(Vec2D p_pos) const {
	// return nothing if p_pos is out of bounds
	if (p_pos > m_rect or p_pos < m_rect)
		return '\0';
	else if (p_pos > *g_window or p_pos < *g_window)
		return '\0';

	return GetCharUnsafe(p_pos);

#ifdef AMARETTO_PLATFORM_LINUX

	move(g_cursorPos.y, g_cursorPos.x);

#endif // AMARETTO_PLATFORM_LINUX
}

Amaretto::char_t Amaretto::Canvas::GetCharScreen(Vec2D p_pos) const {
	p_pos += m_rect.pos;

	if (p_pos > m_rect or p_pos < m_rect)
		return '\0';
	else if (p_pos > *g_window or p_pos < *g_window)
		return '\0';

	return GetCharScreenUnsafe(p_pos);

#ifdef AMARETTO_PLATFORM_LINUX

	move(g_cursorPos.y, g_cursorPos.x);

#endif // AMARETTO_PLATFORM_LINUX
}

void Amaretto::Canvas::ColorChar(Vec2D p_pos) {
#ifdef AMARETTO_PLATFORM_LINUX

	if (g_currColorPair != m_currColorPair) {
		attroff(COLOR_PAIR(g_currColorPair));
		attron(COLOR_PAIR(m_currColorPair));
	}

#endif // AMARETTO_PLATFORM_LINUX

	p_pos += m_rect.pos;

	if (p_pos > m_rect or p_pos < m_rect)
		return;

	ColorCharUnsafe(p_pos);

#ifdef AMARETTO_PLATFORM_LINUX

	move(g_cursorPos.y, g_cursorPos.x);

#endif // AMARETTO_PLATFORM_LINUX
}

void Amaretto::Canvas::DrawHLine(char_t p_ch, Vec2D p_pos, pos_t p_endX, bool p_clr) {
#ifdef AMARETTO_PLATFORM_LINUX

	if (p_clr and g_currColorPair != m_currColorPair) {
		attroff(COLOR_PAIR(g_currColorPair));
		attron(COLOR_PAIR(m_currColorPair));
	}

#endif // AMARETTO_PLATFORM_LINUX

	// if the size is 1, just draw a single char and dont waste time
	if (p_pos.x == p_endX) {
		DrawChar(p_ch, p_pos, p_clr);

		return;
	}

	p_pos  += m_rect.pos;
	p_endX += m_rect.pos.x;

	Vec2D endPos(m_rect.pos + m_rect.size - Vec2D(1, 1));

	// check bounds
	if (p_pos.y < m_rect.pos.y or p_pos.y > endPos.y)
		return;

	if (p_pos.x < m_rect.pos.x)
		p_pos.x = m_rect.pos.x;
	else if (p_pos.x > endPos.x)
		p_pos.x = endPos.x;

	if (p_endX < m_rect.pos.x)
		p_endX = m_rect.pos.x;
	else if (p_endX > endPos.x)
		p_endX = endPos.x;

	// make sure the end pos is bigger than the start pos
	if (p_endX < p_pos.x)
		std::swap(p_pos.x, p_endX);

	drawFunc_t drawChar = p_clr? &Canvas::SetCharUnsafe : &Canvas::ChangeCharUnsafe;

	for (; p_pos.x <= p_endX; ++ p_pos.x)
		drawChar(*this, p_ch, p_pos);

#ifdef AMARETTO_PLATFORM_LINUX

	move(g_cursorPos.y, g_cursorPos.x);

#endif // AMARETTO_PLATFORM_LINUX
}

void Amaretto::Canvas::DrawHLine(const string &p_str, Vec2D p_pos, pos_t p_endX, bool p_clr) {
#ifdef AMARETTO_PLATFORM_LINUX

	if (p_clr and g_currColorPair != m_currColorPair) {
		attroff(COLOR_PAIR(g_currColorPair));
		attron(COLOR_PAIR(m_currColorPair));
	}

#endif // AMARETTO_PLATFORM_LINUX

	if (p_pos.x == p_endX) {
		DrawChar(p_str[0], p_pos, p_clr);

		return;
	}

	p_pos  += m_rect.pos;
	p_endX += m_rect.pos.x;

	Vec2D endPos(m_rect.pos + m_rect.size - Vec2D(1, 1));

	if (p_pos.y < m_rect.pos.y or p_pos.y > endPos.y)
		return;

	if (p_pos.x < m_rect.pos.x)
		p_pos.x = m_rect.pos.x;
	else if (p_pos.x > endPos.x)
		p_pos.x = endPos.x;

	if (p_endX < m_rect.pos.x)
		p_endX = m_rect.pos.x;
	else if (p_endX > endPos.x)
		p_endX = endPos.x;

	if (p_endX < p_pos.x)
		std::swap(p_pos.x, p_endX);

	drawFunc_t drawChar = p_clr? &Canvas::SetCharUnsafe : &Canvas::ChangeCharUnsafe;

	size_t strPos = 0;

	// here we have to draw the line manually on linux too
	for (; p_pos.x <= p_endX; ++ p_pos.x) {
		drawChar(*this, p_str[strPos], p_pos);

		++ strPos;
		if (strPos >= p_str.length())
			strPos = 0;
	}

#ifdef AMARETTO_PLATFORM_LINUX

	move(g_cursorPos.y, g_cursorPos.x);

#endif // AMARETTO_PLATFORM_LINUX
}

void Amaretto::Canvas::DrawHLine(const Pattern &p_patt, Vec2D p_pos, pos_t p_endX) {
	if (p_pos.x == p_endX) {
		const CharInfo &chInfo(p_patt[0]);

		SetColors(chInfo.fg, chInfo.bg);
		DrawChar(chInfo.ch, p_pos);

		return;
	}

	p_pos  += m_rect.pos;
	p_endX += m_rect.pos.x;

	Vec2D endPos(m_rect.pos + m_rect.size - Vec2D(1, 1));

	if (p_pos.y < m_rect.pos.y or p_pos.y > endPos.y)
		return;

	if (p_pos.x < m_rect.pos.x)
		p_pos.x = m_rect.pos.x;
	else if (p_pos.x > endPos.x)
		p_pos.x = endPos.x;

	if (p_endX < m_rect.pos.x)
		p_endX = m_rect.pos.x;
	else if (p_endX > endPos.x)
		p_endX = endPos.x;

	if (p_endX < p_pos.x)
		std::swap(p_pos.x, p_endX);

	Dir dir = p_patt.GetDir();

	// yes, this is a repetitive way, but it is the most optimized one

	Vec2D pattPos(0, 0);

	Color prevFg = m_fg, prevBg = m_bg;

	if (dir == Dir::Vertical) {
		for (; p_pos.x <= p_endX; ++ p_pos.x) {
			const CharInfo &chInfo(p_patt.At(pattPos));

			SetColors(chInfo.fg, chInfo.bg);
			SetCharUnsafe(chInfo.ch, p_pos);

			++ pattPos.y;

			if (pattPos.y >= p_patt.GetSize().y)
				pattPos.y = 0;
		}
	} else {
		for (; p_pos.x <= p_endX; ++ p_pos.x) {
			const CharInfo &chInfo(p_patt.At(pattPos));

			SetColors(chInfo.fg, chInfo.bg);
			SetCharUnsafe(chInfo.ch, p_pos);

			++ pattPos.x;

			if (pattPos.x >= p_patt.GetSize().x)
				pattPos.x = 0;
		}
	}

	if (prevFg != m_fg)
		SetFgColor(prevFg);

	if (prevBg != m_bg)
		SetBgColor(prevBg);

#ifdef AMARETTO_PLATFORM_LINUX

	move(g_cursorPos.y, g_cursorPos.x);

#endif // AMARETTO_PLATFORM_LINUX
}

void Amaretto::Canvas::ColorHLine(Vec2D p_pos, pos_t p_endX) {
#ifdef AMARETTO_PLATFORM_LINUX

	if (g_currColorPair != m_currColorPair) {
		attroff(COLOR_PAIR(g_currColorPair));
		attron(COLOR_PAIR(m_currColorPair));
	}

#endif // AMARETTO_PLATFORM_LINUX

	if (p_pos.x == p_endX) {
		ColorChar(p_pos);

		return;
	}

	p_pos  += m_rect.pos;
	p_endX += m_rect.pos.x;

	Vec2D endPos(m_rect.pos + m_rect.size - Vec2D(1, 1));

	if (p_pos.y < m_rect.pos.y or p_pos.y > endPos.y)
		return;

	if (p_pos.x < m_rect.pos.x)
		p_pos.x = m_rect.pos.x;
	else if (p_pos.x > endPos.x)
		p_pos.x = endPos.x;

	if (p_endX < m_rect.pos.x)
		p_endX = m_rect.pos.x;
	else if (p_endX > endPos.x)
		p_endX = endPos.x;

	if (p_endX < p_pos.x)
		std::swap(p_pos.x, p_endX);

	for (; p_pos.x <= p_endX; ++ p_pos.x)
		ColorCharUnsafe(p_pos);

#ifdef AMARETTO_PLATFORM_LINUX

	move(g_cursorPos.y, g_cursorPos.x);

#endif // AMARETTO_PLATFORM_LINUX
}

void Amaretto::Canvas::DrawVLine(char_t p_ch, Vec2D p_pos, pos_t p_endY, bool p_clr) {
#ifdef AMARETTO_PLATFORM_LINUX

	if (p_clr and g_currColorPair != m_currColorPair) {
		attroff(COLOR_PAIR(g_currColorPair));
		attron(COLOR_PAIR(m_currColorPair));
	}

#endif // AMARETTO_PLATFORM_LINUX

	if (p_pos.y == p_endY) {
		DrawChar(p_ch, p_pos, p_clr);

		return;
	}

	p_pos  += m_rect.pos;
	p_endY += m_rect.pos.y;

	Vec2D endPos(m_rect.pos + m_rect.size - Vec2D(1, 1));

	if (p_pos.x < m_rect.pos.x or p_pos.x > endPos.x)
		return;

	if (p_pos.y < m_rect.pos.y)
		p_pos.y = m_rect.pos.y;
	else if (p_pos.y > endPos.y)
		p_pos.y = endPos.y;

	if (p_endY < m_rect.pos.y)
		p_endY = m_rect.pos.y;
	else if (p_endY > endPos.y)
		p_endY = endPos.y;

	if (p_endY < p_pos.y)
		std::swap(p_pos.y, p_endY);

	drawFunc_t drawChar = p_clr? &Canvas::SetCharUnsafe : &Canvas::ChangeCharUnsafe;

	for (; p_pos.y <= p_endY; ++ p_pos.y)
		drawChar(*this, p_ch, p_pos);

#ifdef AMARETTO_PLATFORM_LINUX

	move(g_cursorPos.y, g_cursorPos.x);

#endif // AMARETTO_PLATFORM_LINUX
}

void Amaretto::Canvas::DrawVLine(const string &p_str, Vec2D p_pos, pos_t p_endY, bool p_clr) {
#ifdef AMARETTO_PLATFORM_LINUX

	if (p_clr and g_currColorPair != m_currColorPair) {
		attroff(COLOR_PAIR(g_currColorPair));
		attron(COLOR_PAIR(m_currColorPair));
	}

#endif // AMARETTO_PLATFORM_LINUX

	if (p_pos.y == p_endY) {
		DrawChar(p_str[0], p_pos, p_clr);

		return;
	}

	p_pos  += m_rect.pos;
	p_endY += m_rect.pos.y;

	Vec2D endPos(m_rect.pos + m_rect.size - Vec2D(1, 1));

	if (p_pos.x < m_rect.pos.x or p_pos.x > endPos.x)
		return;

	if (p_pos.y < m_rect.pos.y)
		p_pos.y = m_rect.pos.y;
	else if (p_pos.y > endPos.y)
		p_pos.y = endPos.y;

	if (p_endY < m_rect.pos.y)
		p_endY = m_rect.pos.y;
	else if (p_endY > endPos.y)
		p_endY = endPos.y;

	if (p_endY < p_pos.y)
		std::swap(p_pos.y, p_endY);

	drawFunc_t drawChar = p_clr? &Canvas::SetCharUnsafe : &Canvas::ChangeCharUnsafe;

	size_t strPos = 0;

	for (; p_pos.y <= p_endY; ++ p_pos.y) {
		drawChar(*this, p_str[strPos], p_pos);

		++ strPos;
		if (strPos >= p_str.length())
			strPos = 0;
	}

#ifdef AMARETTO_PLATFORM_LINUX

	move(g_cursorPos.y, g_cursorPos.x);

#endif // AMARETTO_PLATFORM_LINUX
}

void Amaretto::Canvas::DrawVLine(const Pattern &p_patt, Vec2D p_pos, pos_t p_endY) {
	if (p_pos.y == p_endY) {
		const CharInfo &chInfo(p_patt[0]);

		SetColors(chInfo.fg, chInfo.bg);
		DrawChar(chInfo.ch, p_pos);

		return;
	}

	p_pos  += m_rect.pos;
	p_endY += m_rect.pos.y;

	Vec2D endPos(m_rect.pos + m_rect.size - Vec2D(1, 1));

	if (p_pos.x < m_rect.pos.x or p_pos.x > endPos.x)
		return;

	if (p_pos.y < m_rect.pos.y)
		p_pos.y = m_rect.pos.y;
	else if (p_pos.y > endPos.y)
		p_pos.y = endPos.y;

	if (p_endY < m_rect.pos.y)
		p_endY = m_rect.pos.y;
	else if (p_endY > endPos.y)
		p_endY = endPos.y;

	if (p_endY < p_pos.y)
		std::swap(p_pos.y, p_endY);

	Dir dir = p_patt.GetDir();
	Vec2D pattPos(0, 0);

	Color prevFg = m_fg, prevBg = m_bg;

	if (dir == Dir::Horizontal) {
		for (; p_pos.y <= p_endY; ++ p_pos.y) {
			const CharInfo &chInfo(p_patt.At(pattPos));

			SetColors(chInfo.fg, chInfo.bg);
			SetCharUnsafe(chInfo.ch, p_pos);

			++ pattPos.y;

			if (pattPos.y >= p_patt.GetSize().y)
				pattPos.y = 0;
		}
	} else {
		for (; p_pos.y <= p_endY; ++ p_pos.y) {
			const CharInfo &chInfo(p_patt.At(pattPos));

			SetColors(chInfo.fg, chInfo.bg);
			SetCharUnsafe(chInfo.ch, p_pos);

			++ pattPos.x;

			if (pattPos.x >= p_patt.GetSize().x)
				pattPos.x = 0;
		}
	}

	if (prevFg != m_fg)
		SetFgColor(prevFg);

	if (prevBg != m_bg)
		SetBgColor(prevBg);

#ifdef AMARETTO_PLATFORM_LINUX

	move(g_cursorPos.y, g_cursorPos.x);

#endif // AMARETTO_PLATFORM_LINUX
}

void Amaretto::Canvas::ColorVLine(Vec2D p_pos, pos_t p_endY) {
#ifdef AMARETTO_PLATFORM_LINUX

	if (g_currColorPair != m_currColorPair) {
		attroff(COLOR_PAIR(g_currColorPair));
		attron(COLOR_PAIR(m_currColorPair));
	}

#endif // AMARETTO_PLATFORM_LINUX

	if (p_pos.y == p_endY) {
		ColorChar(p_pos);

		return;
	}

	p_pos  += m_rect.pos;
	p_endY += m_rect.pos.y;

	Vec2D endPos(m_rect.pos + m_rect.size - Vec2D(1, 1));

	if (p_pos.x < m_rect.pos.x or p_pos.x > endPos.x)
		return;

	if (p_pos.y < m_rect.pos.y)
		p_pos.y = m_rect.pos.y;
	else if (p_pos.y > endPos.y)
		p_pos.y = endPos.y;

	if (p_endY < m_rect.pos.y)
		p_endY = m_rect.pos.y;
	else if (p_endY > endPos.y)
		p_endY = endPos.y;

	if (p_endY < p_pos.y)
		std::swap(p_pos.y, p_endY);

	for (; p_pos.y <= p_endY; ++ p_pos.y)
		ColorCharUnsafe(p_pos);

#ifdef AMARETTO_PLATFORM_LINUX

	move(g_cursorPos.y, g_cursorPos.x);

#endif // AMARETTO_PLATFORM_LINUX
}

void Amaretto::Canvas::PrintString(const string &p_str, Vec2D p_pos, bool p_wrap, bool p_clr) {
#ifdef AMARETTO_PLATFORM_LINUX

	if (p_clr and g_currColorPair != m_currColorPair) {
		attroff(COLOR_PAIR(g_currColorPair));
		attron(COLOR_PAIR(m_currColorPair));
	}

#endif // AMARETTO_PLATFORM_LINUX

	p_pos += m_rect.pos;

	Vec2D endPos(m_rect.pos + m_rect.size - Vec2D(1, 1));

	if (p_pos.y > endPos.y)
		return;

	drawFunc_t drawChar = p_clr? &Canvas::SetCharUnsafe : &Canvas::ChangeCharUnsafe;

	pos_t start = p_pos.x;
	for (size_t i = 0; i < p_str.length(); ++ i) {
		wchar_t ch = p_str[i];

		if (p_wrap and ch == '\n') {
			p_pos.x = start;
			++ p_pos.y;

			if (p_pos.y > endPos.y)
				break;

			continue;
		}

		if (p_pos.x > endPos.x) {
			if (p_wrap) {
				p_pos.x = start;
				++ p_pos.y;

				if (p_pos.y > endPos.y)
					break;
			} else
				goto l_skip;
		}

		if ((p_str[i] & 128) == 0)
			ch = p_str[i] & 127;
		else if ((p_str[i] & 224) == 192 && i + 1 < p_str.length()) {
			ch =
				(p_str[i]     & 31) << 6 |
				(p_str[i + 1] & 63);

			++ i;
		} else if ((p_str[i] & 240) == 224 && i + 2 < p_str.length()) {
			ch =
				(p_str[i]     & 15) << 12 |
				(p_str[i + 1] & 63) << 6  |
				(p_str[i + 2] & 63);

			i += 2;
		} else if (i + 3 < p_str.length()) {
			ch =
				(p_str[i]     & 7)  << 18 |
				(p_str[i + 1] & 63) << 12 |
				(p_str[i + 2] & 63) << 6  |
				(p_str[i + 3] & 63);

			i += 4;
		}

		drawChar(*this, ch, p_pos);

	l_skip:
		++ p_pos.x;
	}

#ifdef AMARETTO_PLATFORM_LINUX

	move(g_cursorPos.y, g_cursorPos.x);

#endif // AMARETTO_PLATFORM_LINUX
}

void Amaretto::Canvas::PrintString(const wstring &p_str, Vec2D p_pos, bool p_wrap, bool p_clr) {
#ifdef AMARETTO_PLATFORM_LINUX

	if (p_clr and g_currColorPair != m_currColorPair) {
		attroff(COLOR_PAIR(g_currColorPair));
		attron(COLOR_PAIR(m_currColorPair));
	}

#endif // AMARETTO_PLATFORM_LINUX

	p_pos += m_rect.pos;

	Vec2D endPos(m_rect.pos + m_rect.size - Vec2D(1, 1));

	if (p_pos.y > endPos.y)
		return;

	drawFunc_t drawChar = p_clr? &Canvas::SetCharUnsafe : &Canvas::ChangeCharUnsafe;

	pos_t start = p_pos.x;
	for (auto ch : p_str) {
		if (p_wrap and ch == '\n') {
			p_pos.x = start;
			++ p_pos.y;

			if (p_pos.y > endPos.y)
				break;

			continue;
		}

		if (p_pos.x > endPos.x) {
			if (p_wrap) {
				p_pos.x = start;
				++ p_pos.y;

				if (p_pos.y > endPos.y)
					break;
			} else
				goto l_skip;
		}

		drawChar(*this, ch, p_pos);

	l_skip:
		++ p_pos.x;
	}

#ifdef AMARETTO_PLATFORM_LINUX

	move(g_cursorPos.y, g_cursorPos.x);

#endif // AMARETTO_PLATFORM_LINUX
}

void Amaretto::Canvas::PrintString(size_t p_num, const Vec2D &p_pos, bool p_wrap, bool p_clr) {
	PrintString(std::to_string(p_num), p_pos, p_wrap, p_clr);
}

void Amaretto::Canvas::FillRect(char_t p_ch, Rect p_rect, bool p_clr) {
#ifdef AMARETTO_PLATFORM_LINUX

	if (p_clr and g_currColorPair != m_currColorPair) {
		attroff(COLOR_PAIR(g_currColorPair));
		attron(COLOR_PAIR(m_currColorPair));
	}

#endif // AMARETTO_PLATFORM_LINUX

	if (p_rect.size.x == 0 or p_rect.size.y == 0)
		return; // if the size is 0 we wont draw it
	else if (p_rect.size == Vec2D(1, 1)) {
		DrawChar(p_ch, p_rect.pos, p_clr);

		return;
	}

	Vec2D origPos(p_rect.pos);
	p_rect.pos += m_rect.pos;

	Vec2D endPos(m_rect.pos + m_rect.size - Vec2D(1, 1));

	if (p_rect.pos > endPos)
		return;
	else if (p_rect.pos + p_rect.size < m_rect.pos)
		return;

	if (p_rect.pos.x < m_rect.pos.x) {
		p_rect.size.x += origPos.x;
		p_rect.pos.x   = m_rect.pos.x;
	}

	if (p_rect.pos.y < m_rect.pos.y) {
		p_rect.size.y += origPos.y;
		p_rect.pos.y   = m_rect.pos.y;
	}


	Vec2D end(p_rect.pos + p_rect.size - Vec2D(1, 1));
	Vec2D pos(p_rect.pos);

	if (end.x > endPos.x)
		end.x = endPos.x;

	if (end.y > endPos.y)
		end.y = endPos.y;

	drawFunc_t drawChar = p_clr? &Canvas::SetCharUnsafe : &Canvas::ChangeCharUnsafe;

	for (; pos.y <= end.y; ++ pos.y) {
		for (pos.x = p_rect.pos.x; pos.x <= end.x; ++ pos.x)
			drawChar(*this, p_ch, pos);
	}

#ifdef AMARETTO_PLATFORM_LINUX

	move(g_cursorPos.y, g_cursorPos.x);

#endif // AMARETTO_PLATFORM_LINUX
}

void Amaretto::Canvas::FillRect(const string &p_str, Rect p_rect, bool p_clr) {
#ifdef AMARETTO_PLATFORM_LINUX

	if (p_clr and g_currColorPair != m_currColorPair) {
		attroff(COLOR_PAIR(g_currColorPair));
		attron(COLOR_PAIR(m_currColorPair));
	}

#endif // AMARETTO_PLATFORM_LINUX

	if (p_rect.size.x == 0 or p_rect.size.y == 0)
		return;
	else if (p_rect.size == Vec2D(1, 1)) {
		DrawChar(p_str[0], p_rect.pos, p_clr);

		return;
	}

	Vec2D origPos(p_rect.pos);
	p_rect.pos += m_rect.pos;

	Vec2D endPos(m_rect.pos + m_rect.size - Vec2D(1, 1));

	if (p_rect.pos > endPos)
		return;
	else if (p_rect.pos + p_rect.size < m_rect.pos)
		return;

	if (p_rect.pos.x < m_rect.pos.x) {
		p_rect.size.x += origPos.x;
		p_rect.pos.x   = m_rect.pos.x;
	}

	if (p_rect.pos.y < m_rect.pos.y) {
		p_rect.size.y += origPos.y;
		p_rect.pos.y   = m_rect.pos.y;
	}

	Vec2D end(p_rect.pos + p_rect.size - Vec2D(1, 1));
	Vec2D pos(p_rect.pos);

	if (end.x > endPos.x)
		end.x = endPos.x;

	if (end.y > endPos.y)
		end.y = endPos.y;

	drawFunc_t drawChar = p_clr? &Canvas::SetCharUnsafe : &Canvas::ChangeCharUnsafe;

	for (; pos.y <= end.y; ++ pos.y) {
		size_t strPos = 0;

		for (pos.x = p_rect.pos.x; pos.x <= end.x; ++ pos.x) {
			drawChar(*this, p_str[strPos], pos);

			++ strPos;
			if (strPos >= p_str.length())
				strPos = 0;
		}
	}

#ifdef AMARETTO_PLATFORM_LINUX

	move(g_cursorPos.y, g_cursorPos.x);

#endif // AMARETTO_PLATFORM_LINUX
}

void Amaretto::Canvas::FillRect(const Pattern &p_patt, Rect p_rect) {
	if (p_rect.size.x == 0 or p_rect.size.y == 0)
		return;
	else if (p_rect.size == Vec2D(1, 1)) {
		const CharInfo &chInfo(p_patt[0]);

		SetColors(chInfo.fg, chInfo.bg);
		DrawChar(chInfo.ch, p_rect.pos);

		return;
	}

	Vec2D origPos(p_rect.pos);
	p_rect.pos += m_rect.pos;

	Vec2D endPos(m_rect.pos + m_rect.size - Vec2D(1, 1));

	if (p_rect.pos > endPos)
		return;
	else if (p_rect.pos + p_rect.size < m_rect.pos)
		return;

	if (p_rect.pos.x < m_rect.pos.x) {
		p_rect.size.x += origPos.x;
		p_rect.pos.x   = m_rect.pos.x;
	}

	if (p_rect.pos.y < m_rect.pos.y) {
		p_rect.size.y += origPos.y;
		p_rect.pos.y   = m_rect.pos.y;
	}

	Vec2D end(p_rect.pos + p_rect.size - Vec2D(1, 1));
	Vec2D pos(p_rect.pos);

	if (end.x > endPos.x)
		end.x = endPos.x;

	if (end.y > endPos.y)
		end.y = endPos.y;

	Dir dir = p_patt.GetDir();
	Vec2D pattPos(0, 0);

	Color prevFg = m_fg, prevBg = m_bg;

	if (dir == Dir::Vertical) {
		for (; pos.x <= end.x; ++ pos.x) {
			for (pos.y = p_rect.pos.y; pos.y <= end.y; ++ pos.y) {
				const CharInfo &chInfo(p_patt.At(pattPos));

				SetColors(chInfo.fg, chInfo.bg);
				SetCharUnsafe(chInfo.ch, pos);

				++ pattPos.x;

				if (pattPos.x >= p_patt.GetSize().x)
					pattPos.x = 0;
			}

			pattPos.x = 0;
			++ pattPos.y;

			if (pattPos.y >= p_patt.GetSize().y)
				pattPos.y = 0;
		}
	} else {
		for (; pos.x <= end.x; ++ pos.x) {
			for (pos.y = p_rect.pos.y; pos.y <= end.y; ++ pos.y) {
				const CharInfo &chInfo(p_patt.At(pattPos));

				SetColors(chInfo.fg, chInfo.bg);
				SetCharUnsafe(chInfo.ch, pos);

				++ pattPos.y;

				if (pattPos.y >= p_patt.GetSize().y)
					pattPos.y = 0;
			}

			pattPos.y = 0;
			++ pattPos.x;

			if (pattPos.x >= p_patt.GetSize().x)
				pattPos.x = 0;
		}
	}

	if (prevFg != m_fg)
		SetFgColor(prevFg);

	if (prevBg != m_bg)
		SetBgColor(prevBg);

#ifdef AMARETTO_PLATFORM_LINUX

	move(g_cursorPos.y, g_cursorPos.x);

#endif // AMARETTO_PLATFORM_LINUX
}

void Amaretto::Canvas::ColorFillRect(Rect p_rect) {
#ifdef AMARETTO_PLATFORM_LINUX

	if (g_currColorPair != m_currColorPair) {
		attroff(COLOR_PAIR(g_currColorPair));
		attron(COLOR_PAIR(m_currColorPair));
	}

#endif // AMARETTO_PLATFORM_LINUX

	if (p_rect.size.x == 0 or p_rect.size.y == 0)
		return;
	else if (p_rect.size == Vec2D(1, 1)) {
		ColorChar(p_rect.pos);

		return;
	}

	Vec2D origPos(p_rect.pos);
	p_rect.pos += m_rect.pos;

	Vec2D endPos(m_rect.pos + m_rect.size - Vec2D(1, 1));

	if (p_rect.pos > endPos)
		return;
	else if (p_rect.pos + p_rect.size < m_rect.pos)
		return;

	if (p_rect.pos.x < m_rect.pos.x) {
		p_rect.size.x += origPos.x;
		p_rect.pos.x   = m_rect.pos.x;
	}

	if (p_rect.pos.y < m_rect.pos.y) {
		p_rect.size.y += origPos.y;
		p_rect.pos.y   = m_rect.pos.y;
	}

	Vec2D end(p_rect.pos + p_rect.size - Vec2D(1, 1));
	Vec2D pos(p_rect.pos);

	if (end.x > endPos.x)
		end.x = endPos.x;

	if (end.y > endPos.y)
		end.y = endPos.y;

	for (; pos.y <= end.y; ++ pos.y) {
		for (pos.x = p_rect.pos.x; pos.x <= end.x; ++ pos.x)
			ColorCharUnsafe(pos);
	}

#ifdef AMARETTO_PLATFORM_LINUX

	move(g_cursorPos.y, g_cursorPos.x);

#endif // AMARETTO_PLATFORM_LINUX
}

void Amaretto::Canvas::FillRect(char_t p_ch, bool p_clr) {
#ifdef AMARETTO_PLATFORM_LINUX

	if (p_clr and g_currColorPair != m_currColorPair) {
		attroff(COLOR_PAIR(g_currColorPair));
		attron(COLOR_PAIR(m_currColorPair));
	}

#endif // AMARETTO_PLATFORM_LINUX

	Vec2D endPos(m_rect.pos + m_rect.size - Vec2D(1, 1));

	drawFunc_t drawChar = p_clr? &Canvas::SetCharUnsafe : &Canvas::ChangeCharUnsafe;

	Vec2D pos(0, 0);

	for (pos.y = m_rect.pos.y; pos.y <= endPos.y; ++ pos.y) {
		for (pos.x = m_rect.pos.x; pos.x <= endPos.x; ++ pos.x)
			drawChar(*this, p_ch, pos);
	}

#ifdef AMARETTO_PLATFORM_LINUX

	move(g_cursorPos.y, g_cursorPos.x);

#endif // AMARETTO_PLATFORM_LINUX
}

void Amaretto::Canvas::FillRect(const string &p_str, bool p_clr) {
#ifdef AMARETTO_PLATFORM_LINUX

	if (p_clr and g_currColorPair != m_currColorPair) {
		attroff(COLOR_PAIR(g_currColorPair));
		attron(COLOR_PAIR(m_currColorPair));
	}

#endif // AMARETTO_PLATFORM_LINUX

	Vec2D endPos(m_rect.pos + m_rect.size - Vec2D(1, 1));

	drawFunc_t drawChar = p_clr? &Canvas::SetCharUnsafe : &Canvas::ChangeCharUnsafe;

	Vec2D pos(0, 0);

	for (pos.y = m_rect.pos.y; pos.y <= endPos.y; ++ pos.y) {
		size_t strPos = 0;

		for (pos.x = m_rect.pos.x; pos.x <= endPos.x; ++ pos.x) {
			drawChar(*this, p_str[strPos], pos);

			++ strPos;
			if (strPos >= p_str.length())
				strPos = 0;
		}
	}

#ifdef AMARETTO_PLATFORM_LINUX

	move(g_cursorPos.y, g_cursorPos.x);

#endif // AMARETTO_PLATFORM_LINUX
}

void Amaretto::Canvas::FillRect(const Pattern &p_patt) {
	Vec2D endPos(m_rect.pos + m_rect.size - Vec2D(1, 1));
	Vec2D startPos(m_rect.pos);

	Dir dir = p_patt.GetDir();
	Vec2D pattPos(0, 0);

	Color prevFg = m_fg, prevBg = m_bg;

	Vec2D pos(0, 0);

	if (dir == Dir::Vertical) {
		for (pos.x = startPos.x; pos.x <= endPos.x; ++ pos.x) {
			for (pos.y = startPos.y; pos.y <= endPos.y; ++ pos.y) {
				const CharInfo &chInfo(p_patt.At(pattPos));

				SetColors(chInfo.fg, chInfo.bg);
				SetCharUnsafe(chInfo.ch, pos);

				++ pattPos.x;

				if (pattPos.x >= p_patt.GetSize().x)
					pattPos.x = 0;
			}

			pattPos.x = 0;
			++ pattPos.y;

			if (pattPos.y >= p_patt.GetSize().y)
				pattPos.y = 0;
		}
	} else {
		for (pos.x = startPos.x; pos.x <= endPos.x; ++ pos.x) {
			for (pos.y = startPos.y; pos.y <= endPos.y; ++ pos.y) {
				const CharInfo &chInfo(p_patt.At(pattPos));

				SetColors(chInfo.fg, chInfo.bg);
				SetCharUnsafe(chInfo.ch, pos);

				++ pattPos.y;

				if (pattPos.y >= p_patt.GetSize().y)
					pattPos.y = 0;
			}

			pattPos.y = 0;
			++ pattPos.x;

			if (pattPos.x >= p_patt.GetSize().x)
				pattPos.x = 0;
		}
	}

	if (prevFg != m_fg)
		SetFgColor(prevFg);

	if (prevBg != m_bg)
		SetBgColor(prevBg);

#ifdef AMARETTO_PLATFORM_LINUX

	move(g_cursorPos.y, g_cursorPos.x);

#endif // AMARETTO_PLATFORM_LINUX
}

void Amaretto::Canvas::ColorFillRect() {
#ifdef AMARETTO_PLATFORM_LINUX

	if (g_currColorPair != m_currColorPair) {
		attroff(COLOR_PAIR(g_currColorPair));
		attron(COLOR_PAIR(m_currColorPair));
	}

#endif // AMARETTO_PLATFORM_LINUX

	Vec2D endPos(m_rect.pos + m_rect.size - Vec2D(1, 1));
	Vec2D pos(0, 0);

	for (pos.y = m_rect.pos.y; pos.y <= endPos.y; ++ pos.y) {
		for (pos.x = m_rect.pos.x; pos.x <= endPos.x; ++ pos.x)
			ColorCharUnsafe(pos);
	}

#ifdef AMARETTO_PLATFORM_LINUX

	move(g_cursorPos.y, g_cursorPos.x);

#endif // AMARETTO_PLATFORM_LINUX
}

void Amaretto::Canvas::DrawRect(char_t p_ch, Rect p_rect, bool p_clr) {
	if (p_rect.size.x == 0 or p_rect.size.y == 0)
		return;
	else if (p_rect.size == Vec2D(1, 1)) {
		DrawChar(p_ch, p_rect.pos, p_clr);

		return;
	}

	Vec2D endPos(p_rect.pos + p_rect.size - Vec2D(1, 1));

	// lets get lazy here ;)
	DrawHLine(p_ch, p_rect.pos, endPos.x, p_clr);
	DrawHLine(p_ch, Vec2D(p_rect.pos.x, endPos.y), endPos.x, p_clr);

	DrawVLine(p_ch, Vec2D(p_rect.pos.x, p_rect.pos.y), endPos.y, p_clr);
	DrawVLine(p_ch, Vec2D(endPos.x,     p_rect.pos.y), endPos.y, p_clr);
}

void Amaretto::Canvas::DrawRect(const string &p_str, Rect p_rect, bool p_clr) {
	if (p_rect.size.x == 0 or p_rect.size.y == 0)
		return;
	else if (p_rect.size == Vec2D(1, 1)) {
		DrawChar(p_str[0], p_rect.pos, p_clr);

		return;
	}

	Vec2D endPos(p_rect.pos + p_rect.size - Vec2D(1, 1));

	DrawHLine(p_str, p_rect.pos, endPos.x, p_clr);
	DrawHLine(p_str, Vec2D(p_rect.pos.x, endPos.y), endPos.x, p_clr);

	DrawVLine(p_str, Vec2D(p_rect.pos.x, p_rect.pos.y), endPos.y, p_clr);
	DrawVLine(p_str, Vec2D(endPos.x,     p_rect.pos.y), endPos.y, p_clr);
}

void Amaretto::Canvas::DrawRect(const Pattern &p_patt, Rect p_rect) {
	if (p_rect.size.x == 0 or p_rect.size.y == 0)
		return;
	else if (p_rect.size == Vec2D(1, 1)) {
		const CharInfo &chInfo(p_patt[0]);

		SetColors(chInfo.fg, chInfo.bg);
		DrawChar(chInfo.ch, p_rect.pos);

		return;
	}

	Vec2D endPos(p_rect.pos + p_rect.size - Vec2D(1, 1));

	DrawHLine(p_patt, p_rect.pos, endPos.x);
	DrawHLine(p_patt, Vec2D(p_rect.pos.x, endPos.y), endPos.x);

	DrawVLine(p_patt, Vec2D(p_rect.pos.x, p_rect.pos.y), endPos.y);
	DrawVLine(p_patt, Vec2D(endPos.x,     p_rect.pos.y), endPos.y);
}

void Amaretto::Canvas::ColorRect(Rect p_rect) {
	if (p_rect.size.x == 0 or p_rect.size.y == 0)
		return;
	else if (p_rect.size == Vec2D(1, 1)) {
		ColorChar(p_rect.pos);

		return;
	}

	Vec2D endPos(p_rect.pos + p_rect.size - Vec2D(1, 1));

	ColorHLine(p_rect.pos, endPos.x);
	ColorHLine(Vec2D(p_rect.pos.x, endPos.y), endPos.x);

	ColorVLine(Vec2D(p_rect.pos.x, p_rect.pos.y), endPos.y);
	ColorVLine(Vec2D(endPos.x,     p_rect.pos.y), endPos.y);
}

void Amaretto::Canvas::DrawRect(char_t p_ch, bool p_clr) {
	Vec2D size(m_rect.size - Vec2D(1, 1));

	DrawHLine(p_ch, Vec2D(0, 0),      size.x, p_clr);
	DrawHLine(p_ch, Vec2D(0, size.y), size.x, p_clr);

	DrawVLine(p_ch, Vec2D(0,      1), size.y - 1, p_clr);
	DrawVLine(p_ch, Vec2D(size.x, 1), size.y - 1, p_clr);
}

void Amaretto::Canvas::DrawRect(const string &p_str, bool p_clr) {
	Vec2D size(m_rect.size - Vec2D(1, 1));

	DrawHLine(p_str, Vec2D(0, 0),      size.x, p_clr);
	DrawHLine(p_str, Vec2D(0, size.y), size.x, p_clr);

	DrawVLine(p_str, Vec2D(0,      1), size.y - 1, p_clr);
	DrawVLine(p_str, Vec2D(size.x, 1), size.y - 1, p_clr);
}

void Amaretto::Canvas::DrawRect(const Pattern &p_patt) {
	Vec2D size(m_rect.size - Vec2D(1, 1));

	DrawHLine(p_patt, Vec2D(0, 0),      size.x);
	DrawHLine(p_patt, Vec2D(0, size.y), size.x);

	DrawVLine(p_patt, Vec2D(0,      1), size.y - 1);
	DrawVLine(p_patt, Vec2D(size.x, 1), size.y - 1);
}

void Amaretto::Canvas::ColorRect() {
	Vec2D size(m_rect.size - Vec2D(1, 1));

	ColorHLine(Vec2D(0, 0),      size.x);
	ColorHLine(Vec2D(0, size.y), size.x);

	ColorVLine(Vec2D(0,      1), size.y - 1);
	ColorVLine(Vec2D(size.x, 1), size.y - 1);
}

void Amaretto::Canvas::DrawBorder(const Border &p_border, Rect p_rect, bool p_clr) {
	if (p_rect.size.x == 0 or p_rect.size.y == 0)
		return;
	else if (p_rect.size == Vec2D(1, 1)) {
		DrawChar(p_border.tlChar, p_rect.pos, p_clr);

		return;
	}

#ifdef AMARETTO_PLATFORM_LINUX

	Color prevFg = m_fg;

#elif defined(AMARETTO_PLATFORM_WINDOWS)

	WORD prevAttr = m_currAttr;

#endif // AMARETTO_PLATFORM_WINDOWS

	Vec2D endPos(p_rect.pos + p_rect.size - Vec2D(1, 1));

	DrawHLine(p_border.hChar, Vec2D(p_rect.pos.x, endPos.y), endPos.x, p_clr);
	DrawVLine(p_border.vChar, Vec2D(endPos.x, p_rect.pos.y), endPos.y, p_clr);

	DrawChar(p_border.brChar, endPos, p_clr);

	SetFgColor(p_border.shadeColor);

	DrawHLine(p_border.hChar, p_rect.pos, endPos.x, p_clr);
	DrawVLine(p_border.vChar, p_rect.pos, endPos.y, p_clr);

	DrawChar(p_border.tlChar, p_rect.pos, p_clr);
	DrawChar(p_border.blChar, Vec2D(p_rect.pos.x, endPos.y), p_clr);

#ifdef AMARETTO_PLATFORM_LINUX

	SetFgColor(prevFg);

#elif defined(AMARETTO_PLATFORM_WINDOWS)

	m_currAttr = prevAttr;

#endif // AMARETTO_PLATFORM_WINDOWS

	DrawChar(p_border.trChar, Vec2D(endPos.x, p_rect.pos.y), p_clr);
}

void Amaretto::Canvas::DrawBorder(const Border &p_border, bool p_clr) {
#ifdef AMARETTO_PLATFORM_LINUX

	Color prevFg = m_fg;

#elif defined(AMARETTO_PLATFORM_WINDOWS)

	WORD prevAttr = m_currAttr;

#endif // AMARETTO_PLATFORM_WINDOWS

	Vec2D size(m_rect.size - Vec2D(1, 1));

	DrawHLine(p_border.hChar, Vec2D(1, size.y), size.x - 1, p_clr);
	DrawVLine(p_border.vChar, Vec2D(size.x, 1), size.y - 1, p_clr);

	DrawChar(p_border.brChar, size, p_clr);

	SetFgColor(p_border.shadeColor);

	DrawHLine(p_border.hChar, Vec2D(1, 0), size.x - 1, p_clr);
	DrawVLine(p_border.vChar, Vec2D(0, 1), size.y - 1, p_clr);

	DrawChar(p_border.tlChar, Vec2D(0, 0), p_clr);
	DrawChar(p_border.blChar, Vec2D(0, size.y), p_clr);

#ifdef AMARETTO_PLATFORM_LINUX

	SetFgColor(prevFg);

#elif defined(AMARETTO_PLATFORM_WINDOWS)

	m_currAttr = prevAttr;

#endif // AMARETTO_PLATFORM_WINDOWS

	DrawChar(p_border.trChar, Vec2D(size.x, 0), p_clr);
}

void Amaretto::Canvas::DrawLine(char_t p_ch, Vec2D p_posA, Vec2D p_posB, bool p_clr) {
#ifdef AMARETTO_PLATFORM_LINUX

	if (p_clr and g_currColorPair != m_currColorPair) {
		attroff(COLOR_PAIR(g_currColorPair));
		attron(COLOR_PAIR(m_currColorPair));
	}

#endif // AMARETTO_PLATFORM_LINUX

	p_posA += m_rect.pos;
	p_posB += m_rect.pos;

	// Bresenhams line algorithm

	bool swap = std::fabs(p_posB.y - p_posA.y) > std::fabs(p_posB.x - p_posA.x);
	if (swap) {
		std::swap(p_posA.x, p_posA.y);
		std::swap(p_posB.x, p_posB.y);
	}

	if (p_posA.x > p_posB.x) {
		std::swap(p_posA.x, p_posB.x);
		std::swap(p_posA.y, p_posB.y);
	}

	drawFunc_t drawChar = p_clr? &Canvas::SetCharUnsafe : &Canvas::ChangeCharUnsafe;

	float distX = p_posB.x - p_posA.x;
	float distY = std::fabs(p_posB.y - p_posA.y);
	float error = distX / 2;
	pos_t stepY = (p_posA.y < p_posB.y)? 1 : -1;

	for (; p_posA.x <= p_posB.x; ++ p_posA.x) {
		drawChar(*this, p_ch, swap? Vec2D(p_posA.y, p_posA.x) : p_posA);

		error -= distY;
		if (error < 0) {
			p_posA.y += stepY;
			error    += distX;
		}
	}

#ifdef AMARETTO_PLATFORM_LINUX

	move(g_cursorPos.y, g_cursorPos.x);

#endif // AMARETTO_PLATFORM_LINUX
}

void Amaretto::Canvas::DrawLine(const string &p_str, Vec2D p_posA, Vec2D p_posB, bool p_clr) {
#ifdef AMARETTO_PLATFORM_LINUX

	if (p_clr and g_currColorPair != m_currColorPair) {
		attroff(COLOR_PAIR(g_currColorPair));
		attron(COLOR_PAIR(m_currColorPair));
	}

#endif // AMARETTO_PLATFORM_LINUX

	bool swap = std::fabs(p_posB.y - p_posA.y) > std::fabs(p_posB.x - p_posA.x);
	if (swap) {
		std::swap(p_posA.x, p_posA.y);
		std::swap(p_posB.x, p_posB.y);
	}

	if (p_posA.x > p_posB.x) {
		std::swap(p_posA.x, p_posB.x);
		std::swap(p_posA.y, p_posB.y);
	}

	drawFunc_t drawChar = p_clr? &Canvas::SetCharUnsafe : &Canvas::ChangeCharUnsafe;

	float distX = p_posB.x - p_posA.x;
	float distY = std::fabs(p_posB.y - p_posA.y);
	float error = distX / 2;
	pos_t stepY = (p_posA.y < p_posB.y)? 1 : -1;

	size_t strPos = 0;

	for (; p_posA.x <= p_posB.x; ++ p_posA.x) {
		drawChar(*this, p_str[strPos], swap? Vec2D(p_posA.y, p_posA.x) : p_posA);

		error -= distY;
		if (error < 0) {
			p_posA.y += stepY;
			error    += distX;
		}

		++ strPos;
		if (strPos >= p_str.length())
			strPos = 0;
	}

#ifdef AMARETTO_PLATFORM_LINUX

	move(g_cursorPos.y, g_cursorPos.x);

#endif // AMARETTO_PLATFORM_LINUX
}

void Amaretto::Canvas::DrawLine(const Pattern &p_patt, Vec2D p_posA, Vec2D p_posB) {
	bool swap = std::fabs(p_posB.y - p_posA.y) > std::fabs(p_posB.x - p_posA.x);
	if (swap) {
		std::swap(p_posA.x, p_posA.y);
		std::swap(p_posB.x, p_posB.y);
	}

	if (p_posA.x > p_posB.x) {
		std::swap(p_posA.x, p_posB.x);
		std::swap(p_posA.y, p_posB.y);
	}

	float distX = p_posB.x - p_posA.x;
	float distY = std::fabs(p_posB.y - p_posA.y);
	float error = distX / 2;
	pos_t stepY = (p_posA.y < p_posB.y)? 1 : -1;

	Dir dir = p_patt.GetDir();
	Vec2D pattPos(0, 0);

	Color prevFg = m_fg, prevBg = m_bg;

	if (dir == Dir::Auto) {
		for (; p_posA.x <= p_posB.x; ++ p_posA.x) {
			const CharInfo &chInfo(p_patt.At(pattPos));

			SetColors(chInfo.fg, chInfo.bg);
			DrawChar(chInfo.ch, swap? Vec2D(p_posA.y, p_posA.x) : p_posA);

			error -= distY;
			if (error < 0) {
				p_posA.y += stepY;
				error    += distX;
			}

			++ pattPos.x;

			if (pattPos.x >= p_patt.GetSize().x)
				pattPos.x = 0;
		}
	} else {
		bool pattSwap = swap;
		if (dir == Dir::Vertical)
			pattSwap = not pattSwap;

		Vec2D pattSize(p_patt.GetSize());
		if (pattSwap)
			std::swap(pattSize.x, pattSize.y);

		for (; p_posA.x <= p_posB.x; ++ p_posA.x) {
			const CharInfo &chInfo(p_patt.At(pattSwap? Vec2D(pattPos.y, pattPos.x) : pattPos));

			SetColors(chInfo.fg, chInfo.bg);
			DrawChar(chInfo.ch, swap? Vec2D(p_posA.y, p_posA.x) : p_posA);

			error -= distY;
			if (error < 0) {
				p_posA.y += stepY;
				error    += distX;

				pattPos.y += stepY;

				if (pattPos.y >= pattSize.y)
					pattPos.y = 0;
			}

			++ pattPos.x;

			if (pattPos.x >= pattSize.x)
				pattPos.x = 0;
		}
	}

	if (prevFg != m_fg)
		SetFgColor(prevFg);

	if (prevBg != m_bg)
		SetBgColor(prevBg);

#ifdef AMARETTO_PLATFORM_LINUX

	move(g_cursorPos.y, g_cursorPos.x);

#endif // AMARETTO_PLATFORM_LINUX
}

void Amaretto::Canvas::ColorLine(Vec2D p_posA, Vec2D p_posB) {
#ifdef AMARETTO_PLATFORM_LINUX

	if (g_currColorPair != m_currColorPair) {
		attroff(COLOR_PAIR(g_currColorPair));
		attron(COLOR_PAIR(m_currColorPair));
	}

#endif // AMARETTO_PLATFORM_LINUX

	bool swap = std::fabs(p_posB.y - p_posA.y) > std::fabs(p_posB.x - p_posA.x);
	if (swap) {
		std::swap(p_posA.x, p_posA.y);
		std::swap(p_posB.x, p_posB.y);
	}

	if (p_posA.x > p_posB.x) {
		std::swap(p_posA.x, p_posB.x);
		std::swap(p_posA.y, p_posB.y);
	}

	float distX = p_posB.x - p_posA.x;
	float distY = std::fabs(p_posB.y - p_posA.y);
	float error = distX / 2;
	pos_t stepY = (p_posA.y < p_posB.y)? 1 : -1;

	for (; p_posA.x <= p_posB.x; ++ p_posA.x) {
		ColorCharUnsafe(swap? Vec2D(p_posA.y, p_posA.x) : p_posA);

		error -= distY;
		if (error < 0) {
			p_posA.y += stepY;
			error    += distX;
		}
	}

#ifdef AMARETTO_PLATFORM_LINUX

	move(g_cursorPos.y, g_cursorPos.x);

#endif // AMARETTO_PLATFORM_LINUX
}

void Amaretto::Canvas::SetColors(Color p_fg, Color p_bg) {
#ifdef AMARETTO_PLATFORM_LINUX

	attroff(COLOR_PAIR(g_currColorPair));

	// make sure we wont create a duplicated color pair (waste of color pairs)
	m_currColorPair = CheckForColorPair(p_fg, p_bg);
	g_currColorPair = m_currColorPair;

	attron(COLOR_PAIR(m_currColorPair));

#elif defined(AMARETTO_PLATFORM_WINDOWS)

	// place the colors into the char_t attribute
	WORD bg = static_cast<WORD>(p_bg);

	if (p_bg == Color::Default)
		bg = static_cast<WORD>(Color::Black);
	else if (bg != 0)
		bg *= COLOR_BACKGROUND_OFFSET; // background color offset in attribute

	WORD fg = static_cast<WORD>(p_fg);

	if (p_fg == Color::Default)
		fg = static_cast<WORD>(Color::White);

	m_currAttr = static_cast<WORD>(fg) | bg; // save the attribute

#endif // AMARETTO_PLATFORM_WINDOWS

	m_fg = p_fg;
	m_bg = p_bg;
}

void Amaretto::Canvas::SetFgColor(Color p_fg) {
#ifdef AMARETTO_PLATFORM_LINUX

	attroff(COLOR_PAIR(g_currColorPair));

	m_currColorPair = CheckForColorPair(p_fg, m_bg);
	g_currColorPair = m_currColorPair;

	attron(COLOR_PAIR(m_currColorPair));

#elif defined(AMARETTO_PLATFORM_WINDOWS)

	WORD bg = static_cast<WORD>(m_bg);

	if (m_bg == Color::Default)
		bg = static_cast<WORD>(Color::Black);
	else if (bg != 0)
		bg *= COLOR_BACKGROUND_OFFSET;

	WORD fg = static_cast<WORD>(p_fg);

	if (p_fg == Color::Default)
		fg = static_cast<WORD>(Color::White);

	m_currAttr = static_cast<WORD>(fg) | bg;

#endif // AMARETTO_PLATFORM_WINDOWS

	m_fg = p_fg;
}

void Amaretto::Canvas::SetBgColor(Color p_bg) {
#ifdef AMARETTO_PLATFORM_LINUX

	attroff(COLOR_PAIR(g_currColorPair));

	m_currColorPair = CheckForColorPair(m_fg, p_bg);
	g_currColorPair = m_currColorPair;

	attron(COLOR_PAIR(m_currColorPair));

#elif defined(AMARETTO_PLATFORM_WINDOWS)

	WORD bg = static_cast<WORD>(p_bg);

	if (p_bg == Color::Default)
		bg = static_cast<WORD>(Color::Black);
	else if (bg != 0)
		bg *= COLOR_BACKGROUND_OFFSET;

	WORD fg = static_cast<WORD>(m_fg);

	if (m_fg == Color::Default)
		fg = static_cast<WORD>(Color::White);

	m_currAttr = static_cast<WORD>(fg) | bg;

#endif // AMARETTO_PLATFORM_WINDOWS

	m_bg = p_bg;
}

Amaretto::Color Amaretto::Canvas::GetFgColor() const {
	return m_fg;
}

Amaretto::Color Amaretto::Canvas::GetBgColor() const {
	return m_bg;
}

void Amaretto::Canvas::SetCursorPos(Vec2D p_pos) {
	p_pos += m_rect.pos;

	Vec2D endPos(m_rect.pos + m_rect.size - Vec2D(1, 1));

	if (p_pos.x < m_rect.pos.x)
		p_pos.x = m_rect.pos.x;

	if (p_pos.y < m_rect.pos.y)
		p_pos.y = m_rect.pos.y;

	if (p_pos.x > endPos.x)
		p_pos.x = endPos.x;

	if (p_pos.y > endPos.y)
		p_pos.y = endPos.y;

	g_cursorPos = p_pos;

#ifdef AMARETTO_PLATFORM_LINUX

	move(p_pos.y, p_pos.x);

#elif defined(AMARETTO_PLATFORM_WINDOWS)

	SetConsoleCursorPosition(
		g_stdoutHandle,
		{
			static_cast<SHORT>(p_pos.x),
			static_cast<SHORT>(p_pos.y)
		}
	);

#endif // AMARETTO_PLATFORM_WINDOWS
}

const Amaretto::Vec2D &Amaretto::Canvas::GetCursorPos() const {
	return g_cursorPos;
}

void Amaretto::Canvas::SetRect(const Rect &p_rect) {
	m_rect.size = p_rect.size;
	m_rect.pos  = p_rect.pos;

	if (m_record) {
		m_bufferSize = m_rect.size;

		m_buffer.resize(m_bufferSize.y, {});

		for (auto &row : m_buffer)
			row.resize(m_bufferSize.x, m_bufferFillCh);
	}
}

const Amaretto::Rect &Amaretto::Canvas::GetRect() const {
	return m_rect;
}

void Amaretto::Canvas::SetPos(const Vec2D &p_pos) {
	m_rect.pos = p_pos;
}

const Amaretto::Vec2D &Amaretto::Canvas::GetPos() const {
	return m_rect.pos;
}

void Amaretto::Canvas::CenterPos(const Rect &p_rect) {
	m_rect.pos = CalcCenterPos(m_rect.size, p_rect);
}

void Amaretto::Canvas::SetSize(const Vec2D &p_size) {
	m_rect.size = p_size;

	if (m_record) {
		m_bufferSize = m_rect.size;

		m_buffer.resize(m_bufferSize.y, {});

		for (auto &row : m_buffer)
			row.resize(m_bufferSize.x, m_bufferFillCh);
	}
}

const Amaretto::Vec2D &Amaretto::Canvas::GetSize() const {
	return m_rect.size;
}

void Amaretto::Canvas::SetRecordBuffer(const Buffer &p_buffer) {
	m_buffer = p_buffer;

	m_buffer.resize(m_bufferSize.y, {});

	for (auto &row : m_buffer)
		row.resize(m_bufferSize.x, m_bufferFillCh);
}

const Amaretto::Buffer &Amaretto::Canvas::GetRecordBuffer() const {
	return m_buffer;
}

const Amaretto::Vec2D &Amaretto::Canvas::GetRecordBufferSize() const {
	return m_bufferSize;
}

void Amaretto::Canvas::SetRecordBufferFillChar(const CharInfo &p_bufferFillCh) {
	m_bufferFillCh = p_bufferFillCh;
}

const Amaretto::CharInfo &Amaretto::Canvas::GetRecordBufferFillChar() const {
	return m_bufferFillCh;
}

// private

void Amaretto::Canvas::SetCharUnsafe(char_t p_ch, const Vec2D &p_pos) {
	// basic char drawing function to shorten code

#ifdef AMARETTO_PLATFORM_LINUX

	if (p_ch >= g_wCharStart) {

#	ifdef AMARETTO_WIDECHAR

		cchar_t cch;
		wchar_t wchs[CCHARW_MAX] = {p_ch, 0, 0, 0, 0};

		setcchar(&cch, wchs, 0, 0, nullptr);

		mvadd_wch(p_pos.y, p_pos.x, &cch);

#	else

		if (g_wCharToACSMap.count(p_ch) == 0)
			mvaddch(p_pos.y, p_pos.x, '?');
		else {
			attron(A_ALTCHARSET);

			mvaddch(p_pos.y, p_pos.x, g_wCharToACSMap[p_ch]);

			attroff(A_ALTCHARSET);
		}

#	endif // AMARETTO_WIDECHAR

	} else {
		if (p_ch < 32 or p_ch == 127)
			p_ch = 32;

		mvaddch(p_pos.y, p_pos.x, p_ch);
	}

#elif defined(AMARETTO_PLATFORM_WINDOWS)

	if (p_pos < *g_window or p_pos > *g_window)
		return;

	if (p_ch < 32 or p_ch == 127)
		p_ch = 32;

	// converting 2D pos to 1D pos
	size_t pos = p_pos.y * g_window->size.x + p_pos.x;

	g_screen[pos].Char.UnicodeChar = p_ch;
	g_screen[pos].Attributes       = m_currAttr;

#endif // AMARETTO_PLATFORM_WINDOWS

	if (m_record) {
		Vec2D pos = p_pos - m_rect.pos;

		m_buffer[pos.y][pos.x].ch = p_ch;
		m_buffer[pos.y][pos.x].fg = m_fg;
		m_buffer[pos.y][pos.x].bg = m_bg;
	}
}

void Amaretto::Canvas::ChangeCharUnsafe(char_t p_ch, const Vec2D &p_pos) {
	Color fg = Color::Default, bg = Color::Default;

#ifdef AMARETTO_PLATFORM_LINUX
#	ifdef AMARETTO_WIDECHAR

	cchar_t cchIn;
	mvin_wch(p_pos.y, p_pos.x, &cchIn);

	if (m_record) {
		attr_t  attrs;
		short   colorPair;
		wchar_t wch;
		getcchar(&cchIn, &wch, &attrs, &colorPair, nullptr);

		short a, b;
		pair_content(colorPair, &a, &b);

		fg = static_cast<Color>(a);
		bg = static_cast<Color>(b);
	}

	attron(cchIn.attr);

#	else

	chtype ch = mvinch(p_pos.y, p_pos.x);

	if (m_record) {
		short colorPair = ch & A_COLOR;
		short a, b;
		pair_content(PAIR_NUMBER(colorPair), &a, &b);

		fg = static_cast<Color>(a);
		bg = static_cast<Color>(b);
	}

	attron(ch & A_ATTRIBUTES);

#	endif // AMARETTO_WIDECHAR

	if (p_ch >= g_wCharStart) {

#	ifdef AMARETTO_WIDECHAR

		cchar_t cch;
		wchar_t wchs[CCHARW_MAX] = {p_ch, 0, 0, 0, 0};

		setcchar(&cch, wchs, 0, 0, nullptr);

		mvadd_wch(p_pos.y, p_pos.x, &cch);

#	else

		if (g_wCharToACSMap.count(p_ch) == 0)
			mvaddch(p_pos.y, p_pos.x, '?');
		else {
			attron(A_ALTCHARSET);

			mvaddch(p_pos.y, p_pos.x, g_wCharToACSMap[p_ch]);

			attroff(A_ALTCHARSET);
		}

#	endif // AMARETTO_WIDECHAR

	} else {
		if (p_ch < 32 or p_ch == 127)
			p_ch = 32;

		mvaddch(p_pos.y, p_pos.x, p_ch);
	}

#	ifdef AMARETTO_WIDECHAR

	attroff(cchIn.attr);
	attron(COLOR_PAIR(m_currColorPair));

#	else

	attroff(ch & A_ATTRIBUTES);
	attron(COLOR_PAIR(m_currColorPair));

#	endif // AMARETTO_WIDECHAR

#elif defined(AMARETTO_PLATFORM_WINDOWS)

	if (p_pos < *g_window or p_pos > *g_window)
		return;

	if (p_ch < 32 or p_ch == 127)
		p_ch = 32;

	// converting 2D pos to 1D pos
	size_t pos = p_pos.y * g_window->size.x + p_pos.x;

	g_screen[pos].Char.UnicodeChar = p_ch;

	WORD a = g_screen[pos].Attributes & 0xF;
	WORD b = g_screen[pos].Attributes & 0xF * COLOR_BACKGROUND_OFFSET;

	fg = static_cast<Color>(a);
	bg = static_cast<Color>(b);

#endif // AMARETTO_PLATFORM_WINDOWS

	if (m_record) {
		Vec2D pos = p_pos - m_rect.pos;

		m_buffer[pos.y][pos.x].ch = p_ch;
		m_buffer[pos.y][pos.x].fg = fg;
		m_buffer[pos.y][pos.x].bg = bg;
	}
}

Amaretto::char_t Amaretto::Canvas::GetCharUnsafe(const Vec2D &p_pos) const {
	return m_buffer[p_pos.y][p_pos.x].ch;
}

Amaretto::char_t Amaretto::Canvas::GetCharScreenUnsafe(const Vec2D &p_pos) const {

#ifdef AMARETTO_PLATFORM_LINUX

#	ifdef AMARETTO_WIDECHAR

	cchar_t cchIn;
	mvin_wch(p_pos.y, p_pos.x, &cchIn);

	return cchIn.chars[0];

#	else

	return mvinch(p_pos.y, p_pos.x);

#	endif // AMARETTO_WIDECHAR
#elif defined(AMARETTO_PLATFORM_WINDOWS)

	return g_screen[p_pos.y * g_window->size.x + p_pos.x].Char.AsciiChar;

#endif // AMARETTO_PLATFORM_WINDOWS
}

void Amaretto::Canvas::ColorCharUnsafe(const Vec2D &p_pos) {
	wchar_t buffCh;
#ifdef AMARETTO_PLATFORM_LINUX
#	ifdef AMARETTO_WIDECHAR

	cchar_t cchIn, cch;
	mvin_wch(p_pos.y, p_pos.x, &cchIn);
	buffCh = cchIn.chars[0];

	setcchar(&cch, cchIn.chars, cchIn.attr, m_currColorPair, nullptr);

	mvadd_wch(p_pos.y, p_pos.x, &cch);

#	else

	chtype ch = mvinch(p_pos.y, p_pos.x);
	buffCh = ch;

	if (ch & A_ALTCHARSET) {
		attron(A_ALTCHARSET);

		mvaddch(p_pos.y, p_pos.x, ch & A_CHARTEXT);

		attroff(A_ALTCHARSET);
	} else
		mvaddch(p_pos.y, p_pos.x, ch & A_CHARTEXT);

#	endif // AMARETTO_WIDECHAR
#elif defined(AMARETTO_PLATFORM_WINDOWS)

	if (p_pos < *g_window or p_pos > *g_window)
		return;

	size_t pos = p_pos.y * g_window->size.x + p_pos.x;

	g_screen[pos].Attributes = m_currAttr;
	buffCh = g_screen[pos].Char.UnicodeChar;

#endif // AMARETTO_PLATFORM_WINDOWS

	if (m_record) {
		Vec2D pos = p_pos - m_rect.pos;

		m_buffer[pos.y][pos.x].ch = buffCh;
		m_buffer[pos.y][pos.x].fg = m_fg;
		m_buffer[pos.y][pos.x].bg = m_bg;
	}
}

#ifdef AMARETTO_PLATFORM_LINUX

uint8_t Amaretto::Canvas::CheckForColorPair(Color p_fg, Color p_bg) {
	// if the color is not available, find a replacement
	if (static_cast<int>(p_fg) >= COLORS)
		p_fg = static_cast<Color>(static_cast<int>(p_fg) - COLORS);

	if (static_cast<int>(p_bg) >= COLORS)
		p_bg = static_cast<Color>(static_cast<int>(p_bg) - COLORS);

	// check if the color pair exists
	for (size_t i = 0; i <= g_colorPair; ++ i) {
		short fg, bg;
		pair_content(i, &fg, &bg);

		if (static_cast<short>(p_fg) == fg and static_cast<short>(p_bg) == bg)
			return i;
	}

	// color pair doesnt exist
	++ g_colorPair;
	init_pair(g_colorPair, static_cast<short>(p_fg), static_cast<short>(p_bg));

	return g_colorPair;
}

#endif // AMARETTO_PLATFORM_LINUX
