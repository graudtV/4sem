#pragma once

#include "view.h"

namespace game {

enum class SnakeGameStage {
	eDrawGreeting,
	eWaitingForGame,
	eGame,
	eGameResults,
	eWaitingForNewGame
};

class SnakeGame final {
public:
	SnakeGame(View *view);
	
	void tick();

private:
	View *m_view; // not owning access
	SnakeGameStage m_stage = SnakeGameStage::eDrawGreeting;
	bool m_is_enter_pressed = false;
	int m_game_score = 0;
	int m_game_record = 0;

	void handle_view_resize(ViewResizeEvent event);
	void handle_keyboard_event(KeyboardEvent event);
	void update_game_model();
};

} // game namespace end