//#include <tlnc/expressions.hpp>

#include <tlnc/expressions/constant.hpp>

#include <iostream>

int main()
{
	using t = D_T(-1.0);
	constexpr auto i1 = I_(0.0);
	constexpr auto i2 = I_(-1.0, 1.0);
	std::cout << i1 << std::endl;
	std::cout << i2 << std::endl;
	std::cout << I_(t{}, 1.0).derivative<int>() << std::endl;
}

