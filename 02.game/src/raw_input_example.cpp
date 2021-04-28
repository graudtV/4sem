#include <iostream>
#include "view.h"
#include "cli_view.h"
#include "console.h"
#include "game.h"

#include "unistd.h"

int main()
{
	auto *view = &game::CLIView::get_instance();

	console::ungetchar_direct('0');
	console::ungetchar_direct('1');

	console::KeyboardSymbol c = '\0';
	while (c != '\e') {
		if (console::kbhit())
			std::cout << "hit " << console::to_string(c = console::getkey()) << std::endl; 
		else
			std::cout << "no" << std::endl;
		usleep(100000);
	}
	
	return 0;
}