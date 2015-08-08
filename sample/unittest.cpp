#include <type_traits>
#include <iostream>

#include <boost/numeric/ublas/vector.hpp>

#include <tlnc/expressions.hpp>
#include <tlnc/derivative.hpp>

#include <CppUTest/TestHarness.h>
#include <CppUTest/CommandLineTestRunner.h>

template <typename T, typename U>
struct type_test : std::is_same<std::decay_t<T>, std::decay_t<U>>{
};

template <typename T>
using vector_t = boost::numeric::ublas::vector<T>;

#include <typeinfo>
#include <cxxabi.h>

template <typename T>
const char *name()
{
	return abi::__cxa_demangle(typeid(T).name(), 0, 0, 0);
}

TEST_GROUP(FunctionTest)
{
};

TEST(FunctionTest, Derivative)
{
	using namespace tlnc;
	using namespace tlnc::expressions;

	using tlnc::sin;
	using tlnc::cos;

	constexpr auto f = x<0> * x<1> + sin(x<0>);

	auto g = [](const auto &x){
		return x(0) * x(1) + sin(x(0));
	};

	auto h = [](const auto &x){
		return x(1) + cos(x(0));
	};

	vector_t<double> v(2);
	v(0) = 1.0;
	v(1) = 2.0;

	CHECK((type_test<decltype(f), op_add<op_mul<vector_arg<0>, vector_arg<1>>, expressions::sin<vector_arg<0>>>>{}));
	CHECK((type_test<decltype(derivative(f, x<0>)), op_add<vector_arg<1>, expressions::cos<vector_arg<0>>>>{}));

	CHECK(f(v) == g(v));
	CHECK((derivative(f, x<0>)(v) == h(v)));
}

int main(int argc, char **argv)
{
	return CommandLineTestRunner::RunAllTests(argc, argv);
}

