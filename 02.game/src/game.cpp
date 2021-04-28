#include "game.h"
#include "game_objects.h"
#include "view.h"

namespace game {

SnakeGame::SnakeGame(View *view) :
	m_view(view)
{
	const int timeout_ms = 100000;
	m_view->subscribe_timer(std::bind(&SnakeGame::tick, this), timeout_ms);
	m_view->subscribe_view_resize(std::bind(&SnakeGame::handle_view_resize, this, std::placeholders::_1));
	m_view->subscribe_keyboard(std::bind(&SnakeGame::handle_keyboard_event, this, std::placeholders::_1));
}

void SnakeGame::tick()
{
	switch (m_stage) {
	case SnakeGameStage::eDrawGreeting:
		m_view->draw_greeting_screen();
		m_stage = SnakeGameStage::eWaitingForGame;
		break;
	case SnakeGameStage::eWaitingForGame:
		if (m_is_enter_pressed) {
			m_stage = SnakeGameStage::eGame;
			m_is_enter_pressed = false;
		}
		break;
	case SnakeGameStage::eGame:
		m_view->draw_game_screen();
		m_view->draw_score(m_game_score);
		m_view->draw_record(m_game_record);

		update_game_model();
		if (m_is_enter_pressed) {
			m_stage = SnakeGameStage::eGameResults;
			m_is_enter_pressed = false;
		}
		break;
	case SnakeGameStage::eGameResults:
		m_view->draw_final_screen();
		m_view->draw_score(228);
		m_view->draw_record(337);
		m_stage = SnakeGameStage::eWaitingForNewGame;
		break;
	case SnakeGameStage::eWaitingForNewGame:
		if (m_is_enter_pressed) {
			m_view->interrupt_event_loop();
			m_is_enter_pressed = false;
		}
		break;
	default:
		throw std::runtime_error("SnakeGame: unknown stage");
	}
}

void SnakeGame::handle_view_resize(ViewResizeEvent event)
{
	/* TODO: implement this */
}

void SnakeGame::handle_keyboard_event(KeyboardEvent event)
{
	// assert(event.key.is_common_char());
	//std::cerr << "key!!!" << event.key.to_char() << std::endl;
	if (event.key == '\n' || event.key == '\015')
		m_is_enter_pressed = true;
}

void SnakeGame::update_game_model()
{

}

} // game namespace end