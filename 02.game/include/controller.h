#pragma once

namespace game {

class SnakeGame;
class View;

class Controller {
public:
	virtual ~Controller() {}
	virtual void start_controlling(SnakeGame *game, View *view) = 0;
private:
};

} // game namespace end