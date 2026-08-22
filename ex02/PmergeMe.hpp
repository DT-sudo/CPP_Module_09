#ifndef PMERGEME_HPP
#define PMERGEME_HPP

#include <algorithm>
#include <climits>
#include <iostream>
#include <sys/time.h>
#include <utility>

#include <deque>
#include <string>
#include <vector>

class PmergeMe
{
	public:
	PmergeMe();
	PmergeMe(const PmergeMe &other);
	PmergeMe &operator=(const PmergeMe &other);
	~PmergeMe();
	bool run(int argc, char **argv);

	private:
	std::vector<int> _vc;
	std::deque<int> _dq;
	double _vcTime;
	double _dqTime;
	static bool prsTkn(const char *tkn, int &val);
	static double elapsedUs(const struct timeval &strt, const struct timeval &end);
	void srtVct(std::vector<int> &vec);
	void srtDq(std::deque<int> &deq);
};

#endif
