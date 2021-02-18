#ifndef CLI_H_
#define CLI_H_

#include "view.h"
#include "console.h"
#include <vector>

namespace game {

class CLIView : public View {
public:
	CLIView();
	~CLIView();
	void draw() override;

	static void fetch_screen_size();
private:
	static int screen_width;
	static int screen_height;

	void draw_main_screen();
	void draw_screen_frame();

	void draw_horizontal_line(int y, console::ColoredChar c);
	void draw_horizontal_line(int y, console::ColoredChar c, int x_from, int x_to);

	void draw_vertical_line(int x, console::ColoredChar c);
	void draw_vertical_line(int x, console::ColoredChar c, int y_from, int y_to);

	void draw_empty_rect(int y_from, int y_to, int x_from, int x_to, console::ColoredChar c);
	void draw_filled_rect(int y_from, int y_to, int x_from, int x_to, console::ColoredChar c);

	void draw_picture(const std::vector<std::string>& picture, int y, int x);
};

} // game namespace end

#endif // CLI_H_