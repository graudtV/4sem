#include "console.h"
#include <sys/ioctl.h>
#include <sys/select.h>
#include <unistd.h>

namespace console {

static int getchar_buf = EOF;

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
	return os << c.foreground << c.background;
}

int getc_noblock(FILE *f)
{
	    struct timeval tv = { 0L, 0L };
    fd_set fds;
    FD_ZERO(&fds);
    FD_SET(0, &fds);
    return select(1, &fds, NULL, NULL, &tv);
	return getc(f);
}

void TermiosState::enable_raw_input()
	{ cfmakeraw(&state); }

TermiosState get_termios_state(FILE *f)
{
	TermiosState s;
	if (tcgetattr(fileno(f), &s.state) == -1)
		throw std::system_error(std::error_code(errno, std::generic_category()),
			"console::get_termios_state()");
	return s;
}

void set_termios_state(FILE *f, const TermiosState& s)
{
	if (tcsetattr(fileno(f), TCSANOW, &s.state) != 0)
		throw std::system_error(std::error_code(errno, std::generic_category()),
			"console::set_termios_state()");
}

bool kbhit()
{
	if (getchar_buf != EOF)
		return true;

	struct timeval tv = { 0L, 0L };
	fd_set fds;
	FD_ZERO(&fds);
	FD_SET(STDIN_FILENO, &fds);

	int res = select(1, &fds, NULL, NULL, &tv);
	if (res < -1)
		throw std::system_error(std::error_code(errno, std::generic_category()),
			"console::kbhit()");
	return res;
}

/*  Acquirs one byte exactly from file without buffering (except for 1-symbol buffer
 * for ungetting symbols */
int getchar_direct()
{
	int res;
	unsigned char c;

	if (getchar_buf != EOF) {
		res = getchar_buf;
		getchar_buf = EOF;
		return res;
	}
	if ((res = read(0, &c, sizeof(c))) == -1)
		throw std::system_error(std::error_code(errno, std::generic_category()),
			"console::getc_direct()");
	return (res == 0) ? EOF : c;
}

/* similar to std::ungetc(ch, stdin), but works with getc_direct()
 * If buffer is already filled, it will be overriden by ch */
void ungetchar_direct(int ch)
	{ getchar_buf = ch; }

/* never blocks if kbhit() is true, otherwise may block */
KeyboardSymbol getkey()
{
	int ch;
	if ((ch = getchar_direct()) == EOF || ch != '\e' || !kbhit()) // EOF or common char or ESC without any other symbols
		return ch;
	ch = getchar_direct();
	if (ch != '[') { // just esc, but not an escape sequence
		ungetchar_direct(ch);
		return '\e';
	}
	ch = getchar_direct();
	switch (ch) {
	case 'A': return SpecialSymbol::eUpArrow;
	case 'B': return SpecialSymbol::eDownArrow;
	case 'C': return SpecialSymbol::eRightArrow;
	case 'D': return SpecialSymbol::eLeftArrow;
	default: return SpecialSymbol::eUnknown;
	}
}

std::string to_string(KeyboardSymbol c)
{
	if (c.is_common_char())
		return std::string("<") + std::to_string(c.to_char()) + ">";
	switch (c.to_special_symbol()) {
	case SpecialSymbol::eUpArrow:		return "<up arrow>";
	case SpecialSymbol::eDownArrow:		return "<down arrow>";
	case SpecialSymbol::eRightArrow:	return "<right arrow>";
	case SpecialSymbol::eLeftArrow:		return "<left arrow>";
	case SpecialSymbol::eUnknown:		return "<unknown special symbol>";
	}
}

ConsoleSize get_console_size(FILE *f)
{
	struct winsize w;
	if (ioctl(fileno(f), TIOCGWINSZ, &w) == -1)
		throw std::system_error(std::error_code(errno, std::generic_category()),
			"console::get_console_size()");
	return { w.ws_col, w.ws_row };	
}

} // console namespace end