#include "RPN.hpp"

#include <iostream>

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "Error" << std::endl;
		return 1;
	}
	RPN rpn;
	long rslt;
	if (!rpn.eval(argv[1], rslt))
	{
		std::cerr << "Error" << std::endl;
		return 1;
	}
	std::cout << rslt << std::endl;
	return 0;
}
