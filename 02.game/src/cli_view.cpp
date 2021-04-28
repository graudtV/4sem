#include "cli_view.h"
#include <unistd.h>
#include <chrono>

namespace game {

namespace {

void catch_winch(int unused) { CLIView::get_instance().fetch_screen_size(); }

const std::vector<std::string> snake_logo = {
	R"(\-----------------------------------------------/)",
	R"(|  ssss   nnn    nn    aaaaa    kk   kk   eeeee |)",
	R"(| ss      nn nn  nn   aa   aa   kk kk     ee    |)",
	R"(|  sss    nn nn  nn   aaaaaaa   kkkk      eeeee |)",
	R"(|    ss   nn  nn nn   aa   aa   kk kk     ee    |)",
	R"(| ssss    nn   nnnn   aa   aa   kk   kk   eeeee |)",
	R"(/-----------------------------------------------\)",
};

/* [fst, last) */
int center_pos(std::string_view s, int fst, int last)
{
	if (last - fst < s.size())
		throw std::invalid_argument("allign_middle(): string size is bigger than range");
	return (last - fst - static_cast<int>(s.size())) / 2 + fst;
}

struct allign_center {
public:
	allign_center(std::string_view s, int y, int x_left, int x_right) :
		m_s(s), m_y(y), m_x(center_pos(s, x_left, x_right)) {}
	friend std::ostream& operator <<(std::ostream& os, allign_center allign)
		{ return os << console::setpos(allign.m_y, allign.m_x) << allign.m_s; }
private:
	std::string_view m_s;
	int m_y, m_x;
};

} // anonymous namespace end

void CLIView::draw_greeting_screen()
{
	draw_game_screen();

	std::cout << m_bg_color << m_text_color;
	int x = std::max((m_screen.width - static_cast<int>(snake_logo[0].size())) / 2, 1);
	int y = std::max((m_screen.height - static_cast<int>(snake_logo.size())) / 2, 1);
	draw_picture(snake_logo, {x, y});

	std::cout << allign_center("Press ENTER to continue...", m_game_area.yend() + 1, 1, m_screen.width + 1);
	std::cout.flush();
}

void CLIView::draw_game_screen()
{
	clear_screen();

	std::cout << m_frame_color;
	draw_filled_rect({{1, 1}, m_screen}, ' '); // frame around game zone

	std::cout << m_bg_color;
	draw_filled_rect({m_game_area.coord(), {m_game_area.width, m_game_area.height + 3}}, ' '); // game zone + status bar for score and record

	std::cout << m_separating_lines_color;

	/* verticle separating lines */
	draw_vertical_line(m_game_area.x - 1, '|', m_game_area.y - 1, m_game_area.yend() + 2);
	draw_vertical_line(m_game_area.xend(), '|', m_game_area.y - 1, m_game_area.yend() + 2);
	
	/* horizontal separating lines */
	draw_horizontal_line(m_game_area.y - 1, '-', m_game_area.x, m_game_area.xend());
	draw_horizontal_line(m_game_area.yend(), '-', m_game_area.x, m_game_area.xend());
	draw_horizontal_line(m_game_area.yend() + 2, '-', m_game_area.x, m_game_area.xend());

	std::cout.flush();
}

void CLIView::draw_final_screen()
{
	//draw_game_screen();
	std::cout << allign_center("Press ENTER to play again...", m_game_area.yend() + 1, 1, m_screen.width + 1);
}

void CLIView::draw_snake_segment(Coord2D coord)
{
	std::cout << m_bg_color << console::Color::eRed
		<< console::setpos(coord.y + m_game_area.y, coord.x + m_game_area.x) << "x";
}
void CLIView::draw_rabbit_segment(Coord2D coord)
{
	std::cout << m_bg_color << console::Color::eRed
		<< console::setpos(coord.y + m_game_area.y, coord.x + m_game_area.x) << "@";
}
void CLIView::clear_segment(Coord2D coord)
{
	std::cout << m_bg_color
		<< console::setpos(coord.y + m_game_area.y, coord.x + m_game_area.x) << ' ';
}

void CLIView::draw_score(int score)
{
	std::cout << m_bg_color << m_text_color << console::setpos(m_game_area.yend() + 1, m_game_area.x + 3)
		<< "Score: " << score;
	std::cout.flush();
}

void CLIView::draw_record(int record)
{
	std::cout << m_bg_color << m_text_color << console::setpos(m_game_area.yend() + 1, m_game_area.xend() - 14)
		<< "Record: " << record;
	std::cout.flush();
}

ExtentRange CLIView::supported_map_range()
{
	Extent2D max_extent = { m_screen.width - 4, m_screen.height - 6};
	return { max_extent, max_extent };
}

void CLIView::run_event_loop()
{
	while (!m_interrupt_loop) {
		if (console::kbhit()) {
			auto key = console::getkey();
			for (auto&& cb : m_keyboard_listeners)
				cb({key});
		}
		if (m_is_terminal_resized) {
			m_is_terminal_resized = false;
			for (auto&& cb : m_view_resize_listeners)
				cb({supported_map_range()});
		}
		for (auto&& listener : m_timer_listeners) {
			Timeout time_delta = Clock::now() - listener.previous_callback_time;
			if (time_delta >= listener.timeout) {
				listener.previous_callback_time += listener.timeout;
				listener.callback();
			}
		}
		usleep(100);
	}
	m_interrupt_loop = false;
}

/* fetches screen size to the current terminal size */
void CLIView::fetch_screen_size()
{
	auto size = console::get_console_size(stdout);

	if (m_screen.width != size.width || m_screen.height != size.height)
		m_is_terminal_resized = true;
	m_screen.width = size.width;
	m_screen.height = size.height;
}

void CLIView::adjust_to_map_size_impl(Extent2D map_size)
{
	/* +1 - because top left corner is (1, 1) */
	Coord2D game_area_pos = {
		(m_screen.width - map_size.width) / 2 + 1,
		(m_screen.height - map_size.height - 4) / 2 + 1 + 1}; // second +1 is for separating line on the top of game area
	m_game_area = { game_area_pos, map_size };
}

CLIView::CLIView()
{
	signal(SIGWINCH, catch_winch);
	fetch_screen_size();
	adjust_to_map_size_impl(supported_map_range().max_extent);

	// printf("%d %d\n", m_screen.width, m_screen.height);
	// printf("%d %d\n", m_game_area.x, m_game_area.y);
	// printf("%d %d\n", m_game_area.width, m_game_area.height);

	std::cout << console::show_cursor(false);

	m_initial_termios_state = console::get_termios_state(stdin);

	auto termios_state = m_initial_termios_state;
	termios_state.enable_raw_input();
	console::set_termios_state(stdin, termios_state);
}

CLIView::~CLIView()
{
	console::set_termios_state(stdin, m_initial_termios_state);
	std::cout << console::show_cursor(true);
	clear_screen();
	std::cout.flush();
}

void CLIView::clear_screen()
{
	std::cout << console::default_colors << console::cls << console::home;
}

void CLIView::draw_horizontal_line(int y, char c)
	{ draw_horizontal_line(y, c, 1, m_screen.width + 1); }

/* x_to is included. Note, that top left corner has x and y = 1 */
void CLIView::draw_horizontal_line(int y, char c, int x_from, int x_to)
{
	assert(y >= 1 && y <= m_screen.height + 1);
	assert(x_from >= 1 && x_to <= m_screen.width + 1);

	std::cout << console::setpos(y, x_from);
	for (; x_from < x_to; ++x_from)
		std::cout << c;
}

void CLIView::draw_vertical_line(int x, char c)
	{ draw_vertical_line(x, c, 1, m_screen.height); }

void CLIView::draw_vertical_line(int x, char c, int y_from, int y_to)
{
	assert(x >= 1 && x <= m_screen.width + 1);
	assert(y_from >= 1 && y_to <= m_screen.height + 1);

	for (; y_from <= y_to; ++y_from)
		std::cout << console::setpos(y_from, x) << c;
}

void CLIView::draw_filled_rect(Rect2D rect, char c)
{
	for (auto y = rect.y; y < rect.y + rect.height; ++y)
		draw_horizontal_line(y, c, rect.x, rect.x + rect.width);
}

void CLIView::draw_picture(const std::vector<std::string>& picture, Coord2D coord)
{
	int x = coord.x, y = coord.y;
	assert(x >= 1 && x <= m_screen.width + 1);
	assert(y >= 1 && y <= m_screen.height + 1);

	for (auto line = picture.begin(); line != picture.end() && y <= m_screen.height; ++line, ++y) {
		std::cout << console::setpos(y, x);
		int column = x;
		for (auto c = line->begin(); c != line->end() && column <= m_screen.width; ++c, ++column)
			std::cout << *c;
	}
	std::cout.flush();
}

} // game namespace end