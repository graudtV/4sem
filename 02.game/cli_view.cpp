#include "cli_view.h"
#include <cstdio>
#include <cstdlib>
#include <sys/ioctl.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <system_error>
#include <iostream>
#include <array>

namespace game {

int CLIView::screen_width = 0;
int CLIView::screen_height = 0;
bool CLIView::screen_should_close = false;

void catch_sigint(int unused) { CLIView::process_sigint(); }

namespace {

void catch_winch(int unused) { CLIView::fetch_screen_size(); }

const std::vector<std::string> snake_logo = {
	R"(\-----------------------------------------------/)",
	R"(|  ssss   nnn    nn    aaaaa    kk   kk   eeeee |)",
	R"(| ss      nn nn  nn   aa   aa   kk kk     ee    |)",
	R"(|  sss    nn nn  nn   aaaaaaa   kkkk      eeeee |)",
	R"(|    ss   nn  nn nn   aa   aa   kk kk     ee    |)",
	R"(| ssss    nn   nnnn   aa   aa   kk   kk   eeeee |)",
	R"(/-----------------------------------------------\)",
};

} // anonymous namespace end

CLIView::CLIView()
{
	signal(SIGWINCH, catch_winch);
	signal(SIGINT, catch_sigint);
	fetch_screen_size();
}

CLIView::~CLIView()
{
	draw_empty_screen();
}

void CLIView::fetch_screen_size()
{
	struct winsize w;
	if (ioctl(STDOUT_FILENO, TIOCGWINSZ, &w) == -1)
		throw std::system_error(std::error_code(errno, std::generic_category()),
			"failed to fetch terminal size");
	screen_width = w.ws_col;
	screen_height = w.ws_row;
}

void CLIView::draw_greeting_screen()
{
	draw_screen_frame();

	std::cout << console::Color::eBlue << console::BackgroundColor::eCyan;
	int x = std::max((screen_width - static_cast<int>(snake_logo[0].size())) / 2, 1);
	int y = std::max((screen_height - static_cast<int>(snake_logo.size())) / 2, 1);
	draw_picture(snake_logo, y, x);

	const char prompt[] = "Press ENTER to continue...";
	x = std::max((screen_width - static_cast<int>(sizeof prompt)) / 2 + 1, 1);
	std::cout << console::setpos(screen_height - 2, x) << prompt;
	std::cout.flush();
}

void CLIView::draw_empty_screen()
{
	std::cout << console::Color::eDefault << console::cls << console::home;
}

void CLIView::draw_screen_frame()
{
	draw_filled_rect(1, screen_height, 1, screen_width,
		{' ', console::BackgroundColor::eCyan});
	draw_empty_rect(1, screen_height, 1, screen_width,
		{' ', console::BackgroundColor::eBlue});
}

void CLIView::draw_horizontal_line(int y, console::ColoredChar c)
	{ draw_horizontal_line(y, c, 1, screen_width); }

/* x_to is included. Note, that top left corner has x and y = 1 */
void CLIView::draw_horizontal_line(int y, console::ColoredChar c, int x_from, int x_to)
{
	assert(y >= 1 && y <= screen_height);
	assert(x_from >= 1 && x_to <= screen_width);

	std::cout << console::setpos(y, 1) << c.color; // optimization for using less escape-sequences
	for (; x_from <= x_to; ++x_from)
		std::cout << c.ch;
}

void CLIView::draw_vertical_line(int x, console::ColoredChar c)
	{ draw_vertical_line(x, c, 1, screen_height); }

void CLIView::draw_vertical_line(int x, console::ColoredChar c, int y_from, int y_to)
{
	assert(x >= 1 && x <= screen_width);
	assert(y_from >= 1 && y_to <= screen_height);

	for (; y_from <= y_to; ++y_from)
		std::cout << console::setpos(y_from, x) << c;
}

void CLIView::draw_empty_rect(int y_from, int y_to, int x_from, int x_to, console::ColoredChar c)
{
	draw_horizontal_line(y_from, c, x_from, x_to);
	draw_horizontal_line(y_to, c, x_from, x_to);
	draw_vertical_line(x_from, c, y_from, y_to);
	draw_vertical_line(x_to, c, y_from, y_to);
}

void CLIView::draw_filled_rect(int y_from, int y_to, int x_from, int x_to, console::ColoredChar c)
{
	for (; y_from <= y_to; ++y_from)
		draw_horizontal_line(y_from, c, x_from, x_to);
}

void CLIView::draw_picture(const std::vector<std::string>& picture, int y, int x)
{
	assert(x >= 1 && x <= screen_width);
	assert(y >= 1 && y <= screen_height);

	for (auto line = picture.begin(); line != picture.end() && y <= screen_height; ++line, ++y) {
		std::cout << console::setpos(y, x);
		int column = x;
		for (auto c = line->begin(); c != line->end() && column <= screen_width; ++c, ++column)
			std::cout << *c;
	}
}

} // game namespace end