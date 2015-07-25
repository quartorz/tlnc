//#include <tlnc/expressions.hpp>

#include <tlnc/expressions.hpp>

#include <iostream>

#include <typeinfo>
#include <cxxabi.h>

template <typename T>
const char *name()
{
	return abi::__cxa_demangle(typeid(T).name(), 0, 0, nullptr);
}

template <typename T>
const char *name(T)
{
	return name<T>();
}

int main()
{
	using t = D_T(-1.0);
	constexpr auto i1 = I_(0.0);
	constexpr auto i2 = I_(-1.0, 1.0);
	std::cout << i1 << std::endl;
	std::cout << i2 << std::endl;
	std::cout << I_(t{}, 1.0).derivative<int>() << std::endl;
	std::cout << name(tlnc::sin(tlnc::x())) << std::endl;
	std::cout << tlnc::sin(tlnc::x())(1.0) << std::endl;

	{
		using namespace tlnc::expressions;
		std::cout << name(op_mul<arg, arg, arg>{}.derivative<arg>()) << std::endl;
		std::cout << name(arg{} + arg{} * arg{} * arg{}) << std::endl;
		std::cout << name(arg{} / arg{}) << std::endl;
		std::cout << name(-arg{}) << std::endl;
		std::cout << name(tlnc::sin(arg{}).derivative<arg>()) << std::endl;

		using expr = decltype(arg{} + tlnc::cos(tlnc::sin(arg{})));
		using memo = expr::make_memo_t<bcl::tuple<>, double>;

		std::cout << name<memo>() << std::endl;
	}
}

