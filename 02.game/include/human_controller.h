#pragma once

#include "controller.h"
#include <iostream> // temp

namespace game {

class HumanController : public Controller {
public:
	void start_controlling(SnakeGame *game, View *view)
	{ std::cerr << "controlling ha-ha" << std::endl; }
};

} // game namespace end