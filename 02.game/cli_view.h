#ifndef CLI_VIEW_H_
#define CLI_VIEW_H_

#include "view.h"
#include "console.h"
#include <vector>

namespace game {

class CLIView : public View {
public:
	CLIView();
	~CLIView();

	void draw_empty_screen() override;
	void draw_greeting_screen() override;
	void draw_game_screen() override;
	void draw_snake(const Snake& snake) override;
	void draw_rabbit(const Rabbit& rabit) override;

	bool next_stage_clicked() override { getchar(); return true; }

	MapExtent min_map_size() override;
	MapExtent max_map_size() override;
	
	bool should_close() const override { return screen_should_close; }

	static void fetch_screen_size();
private:
	console::BackgroundColor m_bg_color = console::BackgroundColor::eCyan;
	static int screen_width;
	static int screen_height;
	static bool screen_should_close;

	void adjust_to_map_size_impl(MapExtent map_size) override {}

	void draw_horizontal_line(int y, console::ColoredChar c);
	void draw_horizontal_line(int y, console::ColoredChar c, int x_from, int x_to);

	void draw_vertical_line(int x, console::ColoredChar c);
	void draw_vertical_line(int x, console::ColoredChar c, int y_from, int y_to);

	void draw_empty_rect(int y_from, int y_to, int x_from, int x_to, console::ColoredChar c);
	void draw_filled_rect(int y_from, int y_to, int x_from, int x_to, console::ColoredChar c);

	void draw_picture(const std::vector<std::string>& picture, int y, int x);

	static void process_sigint() { screen_should_close = true; }
	friend void catch_sigint(int);
};

} // game namespace end

#endif // CLI_VIEW_H_