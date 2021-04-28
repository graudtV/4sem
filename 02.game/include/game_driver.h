#pragma once

#include "game.h"
#include "view.h"
#include "controller.h"

namespace game {

class GameDriver {
public:
	GameDriver(View *view, Controller *controller) :
		m_view(view),
		m_controller(controller) {}

	void run_game()
	{
		SnakeGame game(m_view);
		m_controller->start_controlling(&game, m_view);
		m_view->run_event_loop();
	}
private:
	View *m_view;
	Controller *m_controller;
};

} // game namespace end