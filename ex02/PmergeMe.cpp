#include "PmergeMe.hpp"

PmergeMe::PmergeMe(): _vcTime(0), _dqTime(0)
{
}

PmergeMe::PmergeMe(const PmergeMe &other): _vc(other._vc), _dq(other._dq), _vcTime(other._vcTime), _dqTime(other._dqTime)
{
}

PmergeMe &PmergeMe::operator=(const PmergeMe &other)
{
	if (this != &other)
	{
		_vc = other._vc;
		_dq = other._dq;
		_vcTime = other._vcTime;
		_dqTime = other._dqTime;
	}
	return *this;
}

PmergeMe::~PmergeMe()
{
}

bool PmergeMe::prsTkn(const char *tkn, int &val)
{
	if (!tkn || !*tkn)
		return false;
	long prsd = 0;
	for (const char *p = tkn; *p; ++p)
	{
		if (*p < '0' || *p > '9')
			return false;
		prsd = prsd * 10 + (*p - '0');
		if (prsd > INT_MAX)
			return false;
	}
	if (prsd == 0)
		return false;
	val = static_cast<int>(prsd);
	return true;
}

double PmergeMe::elapsedUs(const struct timeval &strt, const struct timeval &end)
{
	return (end.tv_sec - strt.tv_sec) * 1000000.0 + (end.tv_usec - strt.tv_usec);
}

void PmergeMe::srtVct(std::vector<int> &vec)
{
	size_t n = vec.size();
	if (n < 2)
		return;

	bool hasStrglr = (n % 2 != 0);
	int strglr = hasStrglr ? vec[n - 1] : 0;

	std::vector<std::pair<int, int> > pairs;
	pairs.reserve(n / 2);
	for (size_t i = 0; i + 1 < n; i += 2)
	{
		int big = vec[i];
		int small = vec[i + 1];
		if (big < small)
			std::swap(big, small);
		pairs.push_back(std::make_pair(big, small));
	}

	std::vector<int> mChainn;
	mChainn.reserve(pairs.size());
	for (size_t i = 0; i < pairs.size(); ++i)
		mChainn.push_back(pairs[i].first);
	srtVct(mChainn);

	std::vector<int> pendd;
	pendd.reserve(pairs.size());
	std::vector<bool> used(pairs.size(), false);
	for (size_t i = 0; i < mChainn.size(); ++i)
	{
		for (size_t j = 0; j < pairs.size(); ++j)
		{
			if (!used[j] && pairs[j].first == mChainn[i])
			{
				pendd.push_back(pairs[j].second);
				used[j] = true;
				break;
			}
		}
	}

	std::vector<int> chainn;
	chainn.reserve(n);
	chainn.push_back(pendd[0]);
	for (size_t i = 0; i < mChainn.size(); ++i)
		chainn.push_back(mChainn[i]);
	std::vector<size_t> aPos(mChainn.size());
	for (size_t i = 0; i < mChainn.size(); ++i)
		aPos[i] = i + 1;

	size_t m = pendd.size();
	size_t jPrev = 1;
	size_t jCur = 3;
	size_t done = 1;
	while (done < m)
	{
		size_t hi = jCur < m ? jCur : m;
		for (size_t j = hi; j > done; --j)
		{
			int val = pendd[j - 1];
			std::vector<int>::iterator limit = chainn.begin() + aPos[j - 1];
			std::vector<int>::iterator pos =
				std::upper_bound(chainn.begin(), limit, val);
			size_t idx = pos - chainn.begin();
			chainn.insert(pos, val);
			for (size_t k = 0; k < aPos.size(); ++k)
				if (aPos[k] >= idx)
					++aPos[k];
		}
		done = hi;
		size_t jNext = jCur + 2 * jPrev;
		jPrev = jCur;
		jCur = jNext;
	}

	if (hasStrglr)
		chainn.insert(std::upper_bound(chainn.begin(), chainn.end(), strglr),
					 strglr);

	vec = chainn;
}

void PmergeMe::srtDq(std::deque<int> &deq)
{
	size_t n = deq.size();
	if (n < 2)
		return;

	bool hasStrglr = (n % 2 != 0);
	int strglr = hasStrglr ? deq[n - 1] : 0;

	std::deque<std::pair<int, int> > pairs;
	for (size_t i = 0; i + 1 < n; i += 2)
	{
		int big = deq[i];
		int small = deq[i + 1];
		if (big < small)
			std::swap(big, small);
		pairs.push_back(std::make_pair(big, small));
	}

	std::deque<int> mChainn;
	for (size_t i = 0; i < pairs.size(); ++i)
		mChainn.push_back(pairs[i].first);
	srtDq(mChainn);

	std::deque<int> pendd;
	std::deque<bool> used(pairs.size(), false);
	for (size_t i = 0; i < mChainn.size(); ++i)
	{
		for (size_t j = 0; j < pairs.size(); ++j)
		{
			if (!used[j] && pairs[j].first == mChainn[i])
			{
				pendd.push_back(pairs[j].second);
				used[j] = true;
				break;
			}
		}
	}

	std::deque<int> chainn;
	chainn.push_back(pendd[0]);
	for (size_t i = 0; i < mChainn.size(); ++i)
		chainn.push_back(mChainn[i]);
	std::deque<size_t> aPos(mChainn.size());
	for (size_t i = 0; i < mChainn.size(); ++i)
		aPos[i] = i + 1;

	size_t m = pendd.size();
	size_t done = 1;
	size_t jPrev = 1;
	size_t jCur = 3;
	while (done < m)
	{
		size_t hi = jCur < m ? jCur : m;
		for (size_t j = hi; j > done; --j)
		{
			int val = pendd[j - 1];
			std::deque<int>::iterator limit = chainn.begin() + aPos[j - 1];
			std::deque<int>::iterator pos =
				std::upper_bound(chainn.begin(), limit, val);
			size_t idx = pos - chainn.begin();
			chainn.insert(pos, val);
			for (size_t k = 0; k < aPos.size(); ++k)
				if (aPos[k] >= idx)
					++aPos[k];
		}
		done = hi;
		size_t jNext = jCur + 2 * jPrev;
		jPrev = jCur;
		jCur = jNext;
	}

	if (hasStrglr)
		chainn.insert(std::upper_bound(chainn.begin(), chainn.end(), strglr),
					 strglr);

	deq = chainn;
}

bool PmergeMe::run(int argc, char **argv)
{
	if (argc < 2)
		return false;
	for (int i = 1; i < argc; ++i)
	{
		int val;
		if (!prsTkn(argv[i], val))
			return false;
	}

	std::cout << "Before:";
	for (int i = 1; i < argc; ++i)
		std::cout << " " << argv[i];
	std::cout << std::endl;

	struct timeval strt;
	struct timeval end;
	int val = 0;

	gettimeofday(&strt, NULL);
	for (int i = 1; i < argc; ++i)
	{
		prsTkn(argv[i], val);
		_vc.push_back(val);
	}
	srtVct(_vc);
	gettimeofday(&end, NULL);
	_vcTime = elapsedUs(strt, end);

	gettimeofday(&strt, NULL);
	for (int i = 1; i < argc; ++i)
	{
		prsTkn(argv[i], val);
		_dq.push_back(val);
	}
	srtDq(_dq);
	gettimeofday(&end, NULL);
	_dqTime = elapsedUs(strt, end);

	std::cout << "After: ";
	for (size_t i = 0; i < _vc.size(); ++i)
		std::cout << " " << _vc[i];
	std::cout << std::endl;

	std::cout << "Time to process a range of " << _vc.size()
			  << " elements with std::vector : " << _vcTime << " us"
			  << std::endl;
	std::cout << "Time to process a range of " << _dq.size()
			  << " elements with std::deque  : " << _dqTime << " us"
			  << std::endl;
	return true;
}
