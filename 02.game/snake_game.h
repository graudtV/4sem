#ifndef GAME_STAGES_H_
#define GAME_STAGES_H_

#include "game.h"

#include <list>
#include <vector>

namespace game {

struct Coord {
	int x;
	int y;
};

struct Rabbit {
	Coord coord;
};

enum class SnakeDir { eUnknown, eLeft, eRight, eUp, eDown };

struct Snake {
	std::list<Coord> segments;
	SnakeDir dir;
};

class GreetingStage : public GameStage {
public:
	GreetingStage(View *view) : GameStage(view) {}
	void tick() override {}
	void draw_frame() override;
};

class SnakeGameStage : public GameStage {
public:
	SnakeGameStage(View *view) : GameStage(view) {}
	void tick() override;
	void draw_frame() override;
private:
	std::vector<Snake> m_snakes;
	std::vector<Rabbit> m_rabbits;	
};

class EventAwaitingStage : public GameStage {
public:
	EventAwaitingStage(View *view, std::unique_ptr<GameStage> pending_stage);
	void tick() override;
	void draw_frame() override {}
private:
	std::unique_ptr<GameStage> m_pending_stage;
};

} // game namespace end

#endif // GAME_STAGES_H_