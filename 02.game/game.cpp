#include "game.h"
#include "snake_game.h"
#include <cassert>

namespace game {

void GameStage::finish_stage(std::unique_ptr<GameStage> next)
{
	m_next = std::move(next);
	m_is_finished = true;
}

Game::Game(View *view) :
	m_view(view),
	m_stage(std::make_unique<GreetingStage>(view))
	{}

void Game::tick()
{
	assert(m_stage != nullptr);
	m_stage->tick();
	if (m_stage->is_finished()) {
		m_stage = std::move(m_stage->next());
		if (m_stage)
			m_stage->init();
	}
}

void Game::draw_frame()
{
	if (m_stage)
		m_stage->draw_frame();
}

} // game namespace end