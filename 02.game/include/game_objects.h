#ifndef GAME_OBJECTS_H_
#define GAME_OBJECTS_H_

#include "game_decls.h"
#include "list"

namespace game {

class Rabbit {
public:
	Rabbit(Coord2D coord) : m_coord(coord) {}

	Coord2D coord() const { return m_coord; }
private:
	Coord2D m_coord;
};

enum class SnakeDir { eUnknown, eLeft, eRight, eUp, eDown };

class Snake {
public:
	Snake(Coord2D c, SnakeDir d) : m_segments{c}, m_dir(d) {}

	void add_segment(Coord2D c) { m_segments.push_back(c); }
	void pop_last_segment() { m_segments.pop_front(); }
	void change_dir(SnakeDir new_dir) { m_dir = new_dir; }

	auto segments() const { return std::make_pair(m_segments.cbegin(), m_segments.cend()); }
	SnakeDir direction() const { return m_dir; }
private:
	std::list<Coord2D> m_segments;
	SnakeDir m_dir;
};

} // game namespace end

#endif // GAME_OBJECTS_H_