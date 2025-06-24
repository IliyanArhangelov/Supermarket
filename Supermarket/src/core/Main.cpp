#include "core/System.h"
#include "utils/Constants.h"
#include <iostream>

const int MAX_INPUT_SIZE = 1024;

int main()
{
	System supermarket;

	std::cout << "=================================================" << std::endl;
	std::cout << "Welcome to FMI Supermarket Management System!" << std::endl;
	std::cout << "Type 'help' for a list of commands or 'exit' to quit." << std::endl;
	std::cout << "=================================================" << std::endl;

	char userInput[MAX_INPUT_SIZE];

	while (true)
	{
		std::cout << "> ";
		std::cin.getline(userInput, MAX_INPUT_SIZE);

		if (StrOpr::equals(userInput, "exit") || StrOpr::equals(userInput, "quit"))
		{
			break;
		}

		supermarket.action(userInput);
	}

	return 0;
}