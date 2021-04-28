#ifndef CLI_VIEW_H_
#define CLI_VIEW_H_

#include "view.h"
#include "console.h"

namespace game {

class CLIView final : public View {
public:
	void draw_greeting_screen() override;
	void draw_game_screen() override;
	void draw_final_screen() override;

	void draw_snake_segment(Coord2D coord) override;
	void draw_rabbit_segment(Coord2D coord) override;
	void clear_segment(Coord2D coord) override;

	void draw_score(int score) override;
	void draw_record(int record) override;

	ExtentRange supported_map_range() override;
	void run_event_loop() override;

	void fetch_screen_size();
	static CLIView& get_instance() { static CLIView only_one; return only_one; }

private:
	void adjust_to_map_size_impl(Extent2D map_size) override;

private:
	const console::BackgroundColor m_bg_color = console::BackgroundColor::eCyan;
	const console::BackgroundColor m_frame_color = console::BackgroundColor::eBlue;
	const console::Color m_separating_lines_color = console::Color::eBlue;
	const console::Color m_text_color = console::Color::eBlue;

	Extent2D m_screen;
	Rect2D m_game_area;
	console::TermiosState m_initial_termios_state;
	bool m_is_terminal_resized = false;

	/* CLIView is singletone */
	CLIView();
	~CLIView();
	CLIView(const CLIView& other) = delete;
	CLIView(CLIView&& other) = delete;
	CLIView& operator =(const CLIView& other) = delete;
	CLIView& operator =(CLIView&& other) = delete;

	void clear_screen();

	void draw_horizontal_line(int y, char c);
	void draw_horizontal_line(int y, char c, int x_from, int x_to);

	void draw_vertical_line(int x, char c);
	void draw_vertical_line(int x, char c, int y_from, int y_to);

	void draw_empty_rect(Rect2D rect, char c);
	void draw_filled_rect(Rect2D rect, char c);

	void draw_picture(const std::vector<std::string>& picture, Coord2D coord);
};

} // game namespace end

#endif // CLI_VIEW_H_