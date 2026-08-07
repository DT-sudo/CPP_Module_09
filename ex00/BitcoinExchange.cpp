#include "BitcoinExchange.hpp"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>

BitcoinExchange::BitcoinExchange()
{
}

BitcoinExchange::BitcoinExchange(const BitcoinExchange &other)
	: _rates(other._rates)
{
}

BitcoinExchange &BitcoinExchange::operator=(const BitcoinExchange &other)
{
	if (this != &other)
		_rates = other._rates;
	return *this;
}

BitcoinExchange::~BitcoinExchange()
{
}

std::string BitcoinExchange::trim(const std::string &s)
{
	const std::string whitespace = " \t\r\n";
	std::string::size_type strt = s.find_first_not_of(whitespace);
	if (strt == std::string::npos)
		return "";
	std::string::size_type end = s.find_last_not_of(whitespace);
	return s.substr(strt, end - strt + 1);
}

bool BitcoinExchange::VldDate(const std::string &date)
{
	if (date.size() != 10 || date[4] != '-' || date[7] != '-')
		return false;
	for (std::string::size_type i = 0; i < date.size(); ++i)
	{
		if (i == 4 || i == 7)
			continue;
		if (date[i] < '0' || date[i] > '9')
			return false;
	}

	int year = std::atoi(date.substr(0, 4).c_str());
	int month = std::atoi(date.substr(5, 2).c_str());
	int day = std::atoi(date.substr(8, 2).c_str());
	static const int daysInMonth[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

	if (month < 1 || month > 12 || day < 1)
		return false;

	int maxDay = daysInMonth[month - 1];
	bool leap = (year % 4 == 0 && year % 100 != 0) || year % 400 == 0;

	if (month == 2 && leap)
		maxDay = 29;

	return day <= maxDay;
}

bool BitcoinExchange::parsval(const std::string &raw, double &val)
{
	std::istringstream isstr(raw);
	char leftover;

	if (!(isstr >> val))
		return false;
	if (isstr >> leftover)
		return false;

	return true;
}

bool BitcoinExchange::loadDb(const std::string &path)
{
	std::ifstream file(path.c_str());
	if (!file.is_open())
	{
		std::cerr << "Error: could not open database file." << std::endl;
		return false;
	}
	std::string line;
	while (std::getline(file, line))
	{
		std::string::size_type comma = line.find(',');
		if (comma == std::string::npos)
			continue;
		std::string date = trim(line.substr(0, comma));
		std::string rateStr = trim(line.substr(comma + 1));
		double rate;
		if (!VldDate(date) || !parsval(rateStr, rate))
			continue;
		_rates[date] = rate;
	}
	if (_rates.empty())
	{
		std::cerr << "Error: database is empty or invalid." << std::endl;
		return false;
	}
	return true;
}

bool BitcoinExchange::getRate(const std::string &date, double &rate) const
{
	std::map<std::string, double>::const_iterator it = _rates.upper_bound(date);
	if (it == _rates.begin())
		return false;
	--it;
	rate = it->second;
	return true;
}

void BitcoinExchange::prcsLine(const std::string &line) const
{
	std::string::size_type pipe = line.find('|');
	if (pipe == std::string::npos)
	{
		std::cerr << "Error: bad input => " << trim(line) << std::endl;
		return;
	}
	std::string date = trim(line.substr(0, pipe));
	std::string valStr = trim(line.substr(pipe + 1));
	if (!VldDate(date))
	{
		std::cerr << "Error: bad input => " << trim(line) << std::endl;
		return;
	}
	double val;
	if (!parsval(valStr, val))
	{
		std::cerr << "Error: bad input => " << trim(line) << std::endl;
		return;
	}
	if (val < 0)
	{
		std::cerr << "Error: not a positive number." << std::endl;
		return;
	}
	if (val > 1000)
	{
		std::cerr << "Error: too large a number." << std::endl;
		return;
	}
	double rate;
	if (!getRate(date, rate))
	{
		std::cerr << "Error: no exchange rate available on or before "
				  << date << "." << std::endl;
		return;
	}
	std::cout << date << " => " << val << " = " << val * rate << std::endl;
}

void BitcoinExchange::procInptFile(const std::string &path) const
{
	std::ifstream file(path.c_str());
	if (!file.is_open())
	{
		std::cerr << "Error: could not open file." << std::endl;
		return;
	}
	std::string line;
	bool firstLine = true;
	while (std::getline(file, line))
	{
		if (firstLine && trim(line) == "date | value")
		{
			firstLine = false;
			continue;
		}
		firstLine = false;
		if (trim(line).empty())
			continue;
		prcsLine(line);
	}
}
