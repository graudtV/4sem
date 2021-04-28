#include <variant>
#include <iostream>

int main()
{
	std::variant<char, int> var;
	printf("%zu\n", sizeof (var));


	return 0;
}