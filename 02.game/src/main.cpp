#include <iostream>
#include "cli_view.h"
#include "human_controller.h"
#include "game_driver.h"
// #include "unistd.h"

int main()
{
	auto view = &game::CLIView::get_instance();
	auto controller = std::make_unique<game::HumanController>();
	game::GameDriver driver(view, controller.get());
	driver.run_game();

	// // view->subscribe_keyboard(callback1);
	// // view->subscribe_view_resize(resize_callback);
	// view->run_event_loop();

	// view->draw_greeting_screen();
	// //view->draw_snake_segment({5, 0});
	// view->draw_rabbit_segment({5, 0});
	// view->clear_segment({6, 1});
	// getchar();
	// view->draw_game_screen();
	// view->draw_score(228);
	// view->draw_record(337);
	// getchar();
	// view->draw_final_screen();
	// getchar();
	
	// view->

	
	return 0;
}