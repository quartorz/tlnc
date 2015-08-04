#include <tlnc/expressions.hpp>

#include <iostream>

#include <typeinfo>
#include <cxxabi.h>

template <typename T>
const char *name()
{
	return abi::__cxa_demangle(typeid(T).name(), 0, 0, nullptr);
}

int main()
{
	using namespace tlnc;
	auto f = (x<0> + x<1>) * (x<1> + x<0>) * x<0> * (x<1> + x<0>);

	std::cout << name<decltype(f)>() << std::endl;
	std::cout << name<decltype(f.expand())>() << std::endl;
	std::cout << name<decltype((x<0> + TLNC_C(0.0) * x<0> + TLNC_C(1.0) * x<0>).reduction())>() << std::endl;
}

