#ifndef CONSOLE_H_
#define CONSOLE_H_

#include <ostream>

namespace console {

enum class Color : char { eCurrent = -1, eDefault = 0,
	eBlack = 30, eRed, eGreen, eYellow, eBlue, eMagenta, eCyan, eWhite };

enum class BackgroundColor : char { eCurrent = -1, eDefault = 0,
	eBlack = 40, eRed, eGreen, eYellow, eBlue, eMagenta, eCyan, eWhite };

struct ColorPair {
	Color foreground;
	BackgroundColor background;

	ColorPair(Color fg = Color::eCurrent, BackgroundColor bg = BackgroundColor::eCurrent) :
		foreground(fg), background(bg) {}
	ColorPair(BackgroundColor bg) :
		background(bg) {}
};

std::ostream& operator <<(std::ostream& os, Color c);
std::ostream& operator <<(std::ostream& os, BackgroundColor c);
std::ostream& operator <<(std::ostream& os, ColorPair c);

struct ColoredChar {
public:
	char ch;
	ColorPair color;

	ColoredChar(char c, ColorPair color_pair = ColorPair()) :
		ch(c), color(color_pair) {}
	ColoredChar(char c, Color foreground, BackgroundColor background) :
		ch(c), color(foreground, background) {}
	ColoredChar(char c, BackgroundColor background) :
		ch(c), color(background) {}
};

inline std::ostream& operator <<(std::ostream& os, ColoredChar c) { return os << c.color << c.ch; }

/* Ostream manipulator
 * Note. top left corner is (1, 1) */
struct setpos {
public:
	setpos(int y_, int x_) : y(y_), x(x_) {}
	friend std::ostream& operator <<(std::ostream& os, setpos pos) { return os << "\e[" << pos.y << ";" << pos.x << "H"; }

	int y, x;
};

inline constexpr auto cls = "\e[2J"; // effect is equal to setpos(1, 1)
inline constexpr auto home = "\e[H";

} // console namespace end

#endif // CONSOLE_H_