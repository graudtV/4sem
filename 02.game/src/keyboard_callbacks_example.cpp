#include <iostream>
#include "view.h"
#include "cli_view.h"
#include "console.h"
#include "game.h"

#include "unistd.h"

void callback1(game::KeyboardEvent e)
{
	std::cout << "key: " << console::to_string(e.key) << std::endl;
	if (e.key == '\e')
		game::CLIView::get_instance().interrupt_event_loop();
}

int main()
{
	auto *view = &game::CLIView::get_instance();
	view->subscribe_keyboard(callback1);
	view->run_event_loop();
	
	return 0;
}