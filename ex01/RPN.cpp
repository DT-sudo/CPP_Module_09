#include "RPN.hpp"

#include <sstream>

RPN::RPN()
{
}

RPN::RPN(const RPN &other): _stck(other._stck)
{
}

RPN &RPN::operator=(const RPN &other)
{
	if (this != &other)
		_stck = other._stck;
	return *this;
}

RPN::~RPN()
{
}

bool RPN::isOper(const std::string &tkn)
{
	return tkn == "+" || tkn == "-" || tkn == "*" || tkn == "/";
}

bool RPN::applyOper(char op)
{
	if (_stck.size() < 2)
		return false;
	long right = _stck.top();
	_stck.pop();
	long left = _stck.top();
	_stck.pop();
	if (op == '/' && right == 0)
		return false;

	long rslt;
	if (op == '+')
		rslt = left + right;
	else if (op == '-')
		rslt = left - right;
	else if (op == '*')
		rslt = left * right;
	else
		rslt = left / right;
	_stck.push(rslt);
	return true;
}

bool RPN::eval(const std::string &xpresn, long &rslt)
{
	while (!_stck.empty())
		_stck.pop();

	std::istringstream iss(xpresn);
	std::string tkn;
	while (iss >> tkn)
	{
		if (tkn.size() == 1 && tkn[0] >= '0' && tkn[0] <= '9')
			_stck.push(tkn[0] - '0');
		else if (isOper(tkn))
		{
			if (!applyOper(tkn[0]))
				return false;
		}
		else
			return false;
	}
	if (_stck.size() != 1)
		return false;
	rslt = _stck.top();
	return true;
}
