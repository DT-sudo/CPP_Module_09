#ifndef RPN_HPP
#define RPN_HPP

#include <list>
#include <stack>
#include <string>

class RPN
{
	public:
	RPN();
	RPN(const RPN &other);
	RPN &operator=(const RPN &other);
	~RPN();
	bool eval(const std::string &xpresn, long &rslt);

	private:
	std::stack<long, std::list<long> > _stck;
	bool applyOper(char op);
	static bool isOper(const std::string &tkn);
};

#endif
