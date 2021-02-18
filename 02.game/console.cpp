#include "console.h"

namespace console {

std::ostream& operator <<(std::ostream& os, Color c)
{
	return (c == Color::eCurrent)
		? os
		: os << "\e[" << static_cast<int>(c) << "m";
}

std::ostream& operator <<(std::ostream& os, BackgroundColor c)
{
	return (c == BackgroundColor::eCurrent)
		? os
		: os << "\e[" << static_cast<int>(c) << "m";
}

std::ostream& operator <<(std::ostream& os, ColorPair c)
{
	return (c.background == BackgroundColor::eDefault) // not allowing BackgroundColor::eDefault to override foreground color
		? os << "\e[0;" << static_cast<int>(c.foreground) << "m"
		: os << "\e[" << static_cast<int>(c.foreground) << ";" << static_cast<int>(c.background) << "m";
}

} // console namespace end