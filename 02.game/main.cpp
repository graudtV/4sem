#include <iostream>
#include "cli_view.h"

// using namespace console;
// using namespace game;

int main()
{
	auto view = std::make_unique<game::CLIView>();
	//while (1)
		view->draw();

	getchar();
	return 0;
}