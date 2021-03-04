#ifndef GAME_STAGES_H_
#define GAME_STAGES_H_

#include "game.h"

#include <list>
#include <vector>
#include <ctime>

namespace game {

struct Coord {
	int x = 0;
	int y = 0;

	Coord(int xx, int yy) : x(xx), y(yy) {}
};

inline bool operator ==(Coord fst, Coord snd) { return fst.x == snd.x && fst.y == snd.y; }
inline bool operator !=(Coord fst, Coord snd) { return !(fst == snd); }

class Rabbit {
public:
	Rabbit(Coord coord) : m_coord(coord) {}

	Coord coord() const { return m_coord; }
private:
	Coord m_coord;
};

enum class SnakeDir { eUnknown, eLeft, eRight, eUp, eDown };

class Snake {
public:
	Snake(Coord c, SnakeDir d) : m_segments{c}, m_dir(d) {}

	void add_segment(Coord c) { m_segments.push_back(c); }
	void pop_last_segment() { m_segments.pop_front(); }
	void change_dir(SnakeDir new_dir) { m_dir = new_dir; }

	const std::list<Coord>& segments() const { return m_segments; }
	SnakeDir direction() const { return m_dir; }
private:
	std::list<Coord> m_segments;
	SnakeDir m_dir;
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
	void init() override;
	void tick() override;
	void draw_frame() override;
private:
	std::vector<Snake> m_snakes;
	std::vector<Rabbit> m_rabbits;
	MapExtent m_map_size;

	Coord find_empty_pos() const;
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