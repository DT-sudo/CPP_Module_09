#include "PmergeMe.hpp"
#include <iostream>

int main(int argc, char **argv)
{
	PmergeMe srtr;
	
	if (!srtr.run(argc, argv))
	{
		std::cerr << "Error" << std::endl;
		return 1;
	}
	return 0;
}
