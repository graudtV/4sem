#ifndef VIEW_H_
#define VIEW_H_

#include "game.h"

namespace game {

class Snake;
class Rabbit;

class View {
public:
	View() {}
	virtual ~View() {}

	virtual void draw_empty_screen() = 0;
	virtual void draw_greeting_screen() = 0;
	virtual void draw_game_screen() = 0;
	virtual void draw_snake(const Snake& snake) = 0;
	virtual void draw_rabbit(const Rabbit& rabit) = 0;

	virtual bool next_stage_clicked() = 0;

	virtual MapExtent min_map_size() = 0;
	virtual MapExtent max_map_size() = 0;
	void adjust_to_map_size(MapExtent map_size); // should be between min and max

	virtual bool should_close() const = 0;
protected:
	virtual void adjust_to_map_size_impl(MapExtent map_size) = 0;
};

} // game namespace end

#endif // VIEW_H_