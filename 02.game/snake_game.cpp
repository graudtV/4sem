#include "snake_game.h"
#include "view.h"

namespace game {

void GreetingStage::draw_frame()
{
	m_view->draw_empty_screen();
	m_view->draw_greeting_screen();
	finish_stage(std::make_unique<EventAwaitingStage>(m_view, std::make_unique<SnakeGameStage>(m_view)));
}

void SnakeGameStage::tick() {
	finish_stage(nullptr);
}

void SnakeGameStage::draw_frame() {}

EventAwaitingStage::EventAwaitingStage(View *view, std::unique_ptr<GameStage> pending_stage) :
	GameStage(view),
	m_pending_stage(std::move(pending_stage))
	{}

void EventAwaitingStage::tick()
{
	if (m_view->next_stage_clicked())
		finish_stage(std::move(m_pending_stage));
}

} // game namespace end