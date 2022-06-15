#include "../headers/types.hh"

// struct Vec2D
// public

Amaretto::Vec2D::Vec2D(pos_t p_x, pos_t p_y):
	x(p_x),
	y(p_y)
{}

Amaretto::Vec2D::Vec2D(size_t p_x, size_t p_y):
	x(static_cast<pos_t>(p_x)),
	y(static_cast<pos_t>(p_y))
{}

bool Amaretto::Vec2D::operator <(const Rect &p_rect) const {
	return x < p_rect.pos.x or y < p_rect.pos.y;
}

bool Amaretto::Vec2D::operator >(const Rect &p_rect) const {
	Vec2D rectEndPos(p_rect.pos + p_rect.size - Vec2D(1, 1));

	return x > rectEndPos.x or y > rectEndPos.y;
}

bool Amaretto::Vec2D::operator ==(const Vec2D &p_pos) const {
	return x == p_pos.x and y == p_pos.y;
}

bool Amaretto::Vec2D::operator !=(const Vec2D &p_pos) const {
	return not (*this == p_pos);
}

Amaretto::Vec2D Amaretto::Vec2D::operator +(const Vec2D &p_pos) const {
	return Vec2D(x + p_pos.x, y + p_pos.y);
}

Amaretto::Vec2D Amaretto::Vec2D::operator -(const Vec2D &p_pos) const {
	return Vec2D(x - p_pos.x, y - p_pos.y);
}

Amaretto::Vec2D Amaretto::Vec2D::operator *(const Vec2D &p_pos) const {
	return Vec2D(x * p_pos.x, y * p_pos.y);
}

Amaretto::Vec2D Amaretto::Vec2D::operator /(const Vec2D &p_pos) const {
	return Vec2D(x / p_pos.x, y / p_pos.y);
}

Amaretto::Vec2D& Amaretto::Vec2D::operator +=(const Vec2D &p_pos) {
	x += p_pos.x;
	y += p_pos.y;

	return *this;
}

Amaretto::Vec2D& Amaretto::Vec2D::operator -=(const Vec2D &p_pos) {
	x -= p_pos.x;
	y -= p_pos.y;

	return *this;
}

Amaretto::Vec2D& Amaretto::Vec2D::operator *=(const Vec2D &p_pos) {
	x *= p_pos.x;
	y *= p_pos.y;

	return *this;
}

Amaretto::Vec2D& Amaretto::Vec2D::operator /=(const Vec2D &p_pos) {
	x /= p_pos.x;
	y /= p_pos.y;

	return *this;
}

// struct Rect
// public

Amaretto::Rect::Rect(Vec2D p_pos, Vec2D p_size):
	pos (p_pos),
	size(p_size)
{}

Amaretto::Rect::Rect(pos_t p_posx, pos_t p_posy, pos_t p_sizex, pos_t p_sizey):
	pos (p_posx,  p_posy),
	size(p_sizex, p_sizey)
{}

Amaretto::Rect::Rect(size_t p_posx, size_t p_posy, size_t p_sizex, size_t p_sizey):
	pos (static_cast<pos_t>(p_posx),  static_cast<pos_t>(p_posy)),
	size(static_cast<pos_t>(p_sizex), static_cast<pos_t>(p_sizey))
{}

bool Amaretto::Rect::operator ==(const Rect &p_rect) const {
	return pos == p_rect.pos and size == p_rect.size;
}

bool Amaretto::Rect::operator !=(const Rect &p_rect) const {
	return not (*this == p_rect);
}

// struct Border
// public

Amaretto::Border::Border(
	char_t p_tlChar,
	char_t p_trChar,
	char_t p_brChar,
	char_t p_blChar,
	char_t p_hChar,
	char_t p_vChar
):
	tlChar(p_tlChar),
	trChar(p_trChar),
	brChar(p_brChar),
	blChar(p_blChar),
	hChar(p_hChar),
	vChar(p_vChar)
{}

// struct CharInfo
// public

Amaretto::CharInfo::CharInfo(char_t p_ch, Color p_fg, Color p_bg):
	ch(p_ch),
	fg(p_fg),
	bg(p_bg)
{}

Amaretto::CharInfo::CharInfo(char_t p_ch, Color p_clr):
	ch(p_ch),
	fg(p_clr),
	bg(p_clr)
{}

// class Pattern
// public

Amaretto::Pattern::Pattern(const std::vector<CharInfo> &p_patt, Dir p_dir):
	m_size(p_patt.size(), static_cast<size_t>(1)),
	m_patt(p_patt),
	m_dir(p_dir)
{}

Amaretto::Pattern::Pattern(const Vec2D &p_size, const std::vector<CharInfo> &p_patt, Dir p_dir):
	m_size(p_size),
	m_patt(p_patt),
	m_dir(p_dir)
{
	m_patt.resize(m_size.x * m_size.y, CharInfo(' '));
}

Amaretto::Pattern::Pattern(const Vec2D &p_size, Dir p_dir):
	m_size(p_size),
	m_dir(p_dir)
{
	m_patt.resize(m_size.x * m_size.x, CharInfo(' '));
}

void Amaretto::Pattern::SetDir(Dir p_dir) {
	m_dir = p_dir;
}

Amaretto::Dir Amaretto::Pattern::GetDir() const {
	return m_dir;
}

auto Amaretto::Pattern::Begin() {
	return m_patt.begin();
}

auto Amaretto::Pattern::End() {
	return m_patt.end();
}

auto Amaretto::Pattern::Begin() const {
	return m_patt.begin();
}

auto Amaretto::Pattern::End() const {
	return m_patt.end();
}

Amaretto::size_t Amaretto::Pattern::Length() const {
	return m_patt.size();
}

Amaretto::Vec2D Amaretto::Pattern::GetSize() const {
	return m_size;
}

void Amaretto::Pattern::SetSize(const Vec2D &p_size) {
	m_size = p_size;

	m_patt.resize(m_size.x * m_size.y, CharInfo(' '));
}

void Amaretto::Pattern::StrFrom(const Vec2D &p_pos, const string &p_str, Color p_fg, Color p_bg) {
	size_t pos = m_size.x * p_pos.y + p_pos.x;

	for (auto ch : p_str) {
		m_patt.at(pos) = CharInfo(ch, p_fg, p_bg);

		++ pos;
	}
}

Amaretto::CharInfo &Amaretto::Pattern::operator [](size_t p_idx) {
	return m_patt[p_idx];
}

const Amaretto::CharInfo &Amaretto::Pattern::operator [](size_t p_idx) const {
	return m_patt[p_idx];
}

Amaretto::CharInfo &Amaretto::Pattern::At(const Vec2D &p_pos) {
	return m_patt.at(m_size.x * p_pos.y + p_pos.x);
}

const Amaretto::CharInfo &Amaretto::Pattern::At(const Vec2D &p_pos) const {
	return m_patt.at(m_size.x * p_pos.y + p_pos.x);
}

const std::vector<Amaretto::CharInfo> &Amaretto::Pattern::GetVector() const {
	return m_patt;
}

void Amaretto::Pattern::SetVector(const std::vector<CharInfo> &p_patt) {
	m_patt = p_patt;
}

auto Amaretto::Pattern::begin() {
	return Begin();
}

auto Amaretto::Pattern::end() {
	return End();
}

auto Amaretto::Pattern::begin() const {
	return Begin();
}

auto Amaretto::Pattern::end() const {
	return End();
}
