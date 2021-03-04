#include "snake_game.h"
#include "view.h"

namespace game {

void GreetingStage::draw_frame()
{
	m_view->draw_empty_screen();
	m_view->draw_greeting_screen();
	finish_stage(std::make_unique<EventAwaitingStage>(m_view, std::make_unique<SnakeGameStage>(m_view)));
}

void SnakeGameStage::init()
{
	m_map_size = m_view->max_map_size();
	m_view->adjust_to_map_size(m_map_size);

	m_view->draw_empty_screen();

	Coord snake_coord(m_map_size.width / 2, m_map_size.height / 2);
	m_snakes.push_back({snake_coord, SnakeDir::eUp});

	m_rabbits.push_back(find_empty_pos());
}

void SnakeGameStage::tick()
{

}

void SnakeGameStage::draw_frame()
{
	m_view->draw_game_screen();
	for (auto& rabbit : m_rabbits)
		m_view->draw_rabbit(rabbit);
	for (auto& snake : m_snakes)
		m_view->draw_snake(snake);
}

Coord SnakeGameStage::find_empty_pos() const
{
	while (1) {
		int x = rand() % m_map_size.width;
		int y = rand() % m_map_size.height;
		Coord coord(x, y);

		for (auto& rabbit : m_rabbits)
			if (rabbit.coord() == coord)
				continue;
		for (auto& snake : m_snakes)
			for (auto& segment : snake.segments())
				if (segment == coord)
					continue;
		return coord;
	}

}

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