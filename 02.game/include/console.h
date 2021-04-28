#ifndef CONSOLE_H_
#define CONSOLE_H_

#include <iostream>
#include <termios.h>

namespace console {

/* Represents foreground color in console */
enum class Color : char { eCurrent = -1,
	eBlack = 30, eRed, eGreen, eYellow, eBlue, eMagenta, eCyan, eWhite };

/* Represents background color in console */
enum class BackgroundColor : char { eCurrent = -1,
	eBlack = 40, eRed, eGreen, eYellow, eBlue, eMagenta, eCyan, eWhite };

struct ColorPair {
	Color foreground;
	BackgroundColor background;

	ColorPair(Color fg = Color::eCurrent, BackgroundColor bg = BackgroundColor::eCurrent) :
		foreground(fg), background(bg) {}
	ColorPair(BackgroundColor bg) :
		foreground(Color::eCurrent), background(bg) {}
};

std::ostream& operator <<(std::ostream& os, Color c);
std::ostream& operator <<(std::ostream& os, BackgroundColor c);
std::ostream& operator <<(std::ostream& os, ColorPair c);

inline bool operator ==(ColorPair fst, ColorPair snd) { return fst.foreground == snd.foreground && fst.background == snd.background; }
inline bool operator !=(ColorPair fst, ColorPair snd) { return !(fst == snd); }

struct ColoredChar {
public:
	char ch;
	ColorPair color;

	ColoredChar(char c = '\0', ColorPair color_pair = ColorPair()) :
		ch(c), color(color_pair) {}
	ColoredChar(char c, Color foreground, BackgroundColor background) :
		ch(c), color(foreground, background) {}
	ColoredChar(char c, BackgroundColor background) :
		ch(c), color(background) {}
};

inline std::ostream& operator <<(std::ostream& os, ColoredChar c) { return os << c.color << c.ch; }
inline bool operator ==(ColoredChar fst, ColoredChar snd) { return fst.ch == snd.ch && fst.color == snd.color; }
inline bool operator !=(ColoredChar fst, ColoredChar snd) { return !(fst == snd); }

/* Ostream manipulator, moves cursor to given position.
 * Note. top left corner is (1, 1) */
struct setpos {
public:
	explicit setpos(int y_, int x_) : y(y_), x(x_) {}
	friend std::ostream& operator <<(std::ostream& os, setpos pos) { return os << "\e[" << pos.y << ";" << pos.x << "H"; }

	int y, x;
};

/* Ostream manipulator, shows or hides cursor in console */
struct show_cursor {
public:
	explicit show_cursor(bool show_) : show(show_) {}
	friend std::ostream& operator <<(std::ostream& os, show_cursor cursor) { return os << ((cursor.show) ? "\e[?25h" : "\e[?25l"); }
	bool show;
};

/* Can be used as manipulators */
inline constexpr auto cls = "\e[2J"; // effect is equal to setpos(1, 1)
inline constexpr auto home = "\e[H";
inline constexpr auto default_colors = "\e[m"; // reset foreground and background to default colors

enum class SpecialSymbol : char {
	eUnknown,
	eUpArrow, eDownArrow, eRightArrow, eLeftArrow // don't change the order
};

inline bool is_arrow(SpecialSymbol c) { return c >= SpecialSymbol::eLeftArrow && c <= SpecialSymbol::eDownArrow; }

/* Wrapper for termios interface
 * Note. Proposed usage is like this:
 *   auto state = get_termios_state(stdin);
 *   state.enable_raw_input();
 *   ...
 *   set_termios_state(stdin, state);
 * By default TermiosState holds empty state, so be careful: setting it
 * to file without proper initialisation may reset termios flags
 */
struct TermiosState {
	termios state = {};

	void enable_raw_input();
};

TermiosState get_termios_state(FILE *f);
void set_termios_state(FILE *f, const TermiosState& s);

/*  KeyboardSymbol can store both common chars (chars and EOF) and special symbols,
 * like arrows on a keyboard, which are listed in SpecialSymbol
 *  Such symbols can be read using getkey() */
class KeyboardSymbol {
public:
	KeyboardSymbol(int c = '\0') : m_ch(c) {}
	KeyboardSymbol(SpecialSymbol c) : m_ch(static_cast<int>(c) + 256) {}

	bool is_special_symbol() const { return m_ch >= 256; }
	bool is_common_char() const { return !is_special_symbol(); } // char that can be stored in char (big brain)
	
	int to_char() const { return m_ch; } // behaviour is undefined if is_common_char() == false
	SpecialSymbol to_special_symbol() const { return static_cast<SpecialSymbol>(m_ch - 256); } // behaviour is undefined if is_special_symbol == false

private:
	int16_t m_ch; // special symbols are stores as numbers >= 256

	friend bool operator ==(KeyboardSymbol fst, KeyboardSymbol snd) { return fst.m_ch == snd.m_ch; }
};

inline bool operator !=(KeyboardSymbol fst, KeyboardSymbol snd) { return !(fst == snd); }

bool kbhit();
int getchar_direct();
void ungetchar_direct(int ch);
KeyboardSymbol getkey();

std::string to_string(KeyboardSymbol c);

struct ConsoleSize {
	int width;
	int height;

	ConsoleSize(int width_, int height_) :
		width(width_), height(height_) {}
};

void set_console_size(FILE *f, ConsoleSize sz); // not implemented
ConsoleSize get_console_size(FILE *f);

} // console namespace end

#endif // CONSOLE_H_