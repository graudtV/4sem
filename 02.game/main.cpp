#include <iostream>
#include "cli_view.h"
//#include "gui_view.h"
#include "game.h"
#include "unistd.h"

int main()
{
	srand(time(0));

	int opt_cli_view = true;
	std::unique_ptr<game::View> view;
	std::unique_ptr<game::Game> game;

	if (opt_cli_view)
		view = std::make_unique<game::CLIView>();
	else
		; //view = std::make_unique<game::GUIView>();

	game = std::make_unique<game::Game>(view.get());

	while (!game->is_finished() && !view->should_close()) {
		game->tick();
		game->draw_frame();
		usleep(100000);
	}
	return 0;
}