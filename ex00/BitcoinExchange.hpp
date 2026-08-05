#ifndef BITCOINEXCHANGE_HPP
#define BITCOINEXCHANGE_HPP

#include <string>
#include <map>

class BitcoinExchange
{
	public:
	BitcoinExchange();
	BitcoinExchange(const BitcoinExchange &other);
	BitcoinExchange &operator=(const BitcoinExchange &other);
	~BitcoinExchange();
	bool loadDb(const std::string &path);
	void procInptFile(const std::string &path) const;

	private:
	std::map<std::string, double> _rates;
	void prcsLine(const std::string &line) const;
	bool getRate(const std::string &date, double &rate) const;
	static bool VldDate(const std::string &date);
	static bool parsval(const std::string &raw, double &val);
	static std::string trim(const std::string &s);
};

#endif
