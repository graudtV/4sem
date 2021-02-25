#ifndef VIEW_H_
#define VIEW_H_

namespace game {

class Snake;
class Rabbit;

class View {
public:
	View() {}
	virtual ~View() {}

	virtual void draw_empty_screen() = 0;
	virtual void draw_greeting_screen() = 0;
	virtual void draw_snake(const Snake& snakes) = 0;
	virtual void draw_rabbit(const Rabbit& rabits) = 0;

	virtual bool next_stage_clicked() = 0;

	virtual bool should_close() const = 0;
};

} // game namespace end

#endif // VIEW_H_