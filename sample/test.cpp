#if defined(_MSC_VER)
#if !defined(_SCL_SECURE_NO_WARNINGS)
#define _SCL_SECURE_NO_WARNINGS
#endif
#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif
#endif

#include <typeinfo>

#include <bcl/literals.hpp>
#include <cti/literals.hpp>
#include <cti/operators.hpp>

#include <tlnc/expressions.hpp>
#include <tlnc/derivative.hpp>
#include <tlnc/call.hpp>
#include <tlnc/jacobian.hpp>

#include <kv/interval.hpp>
#include <kv/rdouble.hpp>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>

#include <cxxabi.h>

template <typename T>
const char *name(T)
{
	return abi::__cxa_demangle(typeid(T).name(), 0, 0, 0);
}

int main()
{
	using interval_t = kv::interval<double>;

	{
		// f(x) = cos(sin(x))
		// g(x) = f'(x)
		// h(x) = g'(x)

		auto f = tlnc::cos(tlnc::sin(tlnc::x<>));
		auto g = tlnc::derivative(f, tlnc::x<>);
		auto h = tlnc::derivative(g);

		auto x = boost::numeric::ublas::vector<double>(2);
		x(0) = 1.0;
		x(1) = 2.0;

		std::cout << "f(x) = cos(sin(x))" << std::endl;
		std::cout << "g(x) = f'(x)" << std::endl;
		std::cout << "h(x) = g'(x)" << std::endl << std::endl;

		std::cout << "f(1.0) = " << f(1.0) << std::endl;
		std::cout << "g(1.0) = " << g(1.0) << std::endl;
		std::cout << "h(1.0) = " << h(1.0) << std::endl;
		std::cout << "h([1.0, 2.0]) = " << h(interval_t(1.0, 2.0)) << std::endl;

		std::cout << "type of f : " << name(f) << std::endl << std::endl;
		std::cout << "type of g : " << name(g) << std::endl << std::endl;
		std::cout << "type of h : " << name(h) << std::endl << std::endl;
		std::cout << name(h.reduction()) << std::endl << std::endl;
	}

	{
		// f(x1, x2) = x1 * x2 + sin(x1)
		// g(x1, x2) = Ýf / Ýx1 = x2 + cos(x1)
		// h(x1, x2) = Ýf / Ýx2 = x1

		auto f = tlnc::x<0> * tlnc::x<1> + tlnc::sin(tlnc::x<0>);
		auto g = tlnc::derivative(f, tlnc::x<0>);
		auto h = tlnc::derivative(f, tlnc::x<1>);

		auto x = boost::numeric::ublas::vector<double>(2);
		x(0) = 1.0;
		x(1) = 2.0;

		auto y = boost::numeric::ublas::vector<interval_t>(2);
		y(0) = interval_t(1.0, 2.0);
		y(1) = interval_t(3.0, 4.0);

		std::cout << "f(x1, x2) = x1 * x2 + sin(x1)" << std::endl;
		std::cout << "g(x1, x2) = x2 + cos(x1)" << std::endl;
		std::cout << "h(x1, x2) = x1" << std::endl << std::endl;

		std::cout << "f(1.0, 2.0) = " << f(x) << std::endl;
		std::cout << "g(1.0, 2.0) = " << g(x) << std::endl;
		std::cout << "h(1.0, 2.0) = " << h(x) << std::endl;
		std::cout << "g([1.0, 2.0], [3.0, 4.0]) = " << g(y) << std::endl;
		std::cout << "type of f : " << name(f) << std::endl << std::endl;
		std::cout << "type of g : " << name(g) << std::endl << std::endl;
		std::cout << "type of h : " << name(h) << std::endl << std::endl;
	}

	{
		using tlnc::x;

		auto f = C_(2.0) * x<>;
		auto g = x<> * x<>;
		auto h = ((f, g), (f, g), (f, g));

		std::cout << name(h) << std::endl;
		std::cout << h(10.0) << std::endl;

		auto r = tlnc::call(h, 10.0, tlnc::memo());

		std::cout << r.first << std::endl;
		std::cout << name(r.second) << std::endl << std::endl;

		auto r2 = tlnc::call(h, interval_t(9.0, 11.0), tlnc::memo());

		std::cout << r2.first << std::endl;
		std::cout << name(r2.second) << std::endl << std::endl;
	}

	{
		using tlnc::x;
		using tlnc::sin;
		using tlnc::cos;

		auto f = sin(x<>);
		auto g = cos(x<>);
		auto h = ((f, g), (f, g), (f, g));

		std::cout << name(tlnc::prod(h, (C_(2.0), C_(3.0)))) << std::endl << std::endl;
	}

	{
		using namespace bcl::literals;

		using tlnc::x;

		auto f = 2.0_dc * x<0>;
		auto g = 3.0_dc * x<1>;
		auto h = (f, g);

		std::cout << name(tlnc::jacobian<2>(f, g)) << std::endl << std::endl;
		std::cout << name(tlnc::jacobian<2>(h)) << std::endl << std::endl;

		std::cout << name(tlnc::jacobian<2>(h) + tlnc::jacobian<2>(h)) << std::endl << std::endl;
	}

	{
		using namespace bcl::literals;
		using namespace cti::literals;

		using tlnc::x;

		auto f = 1.0_dc * x<0>;
		auto g = 2.0_di * x<1>;
		auto h = 3.0_dc * x<2>;

		std::cout << name(tlnc::jacobian<1, 2>(f, g, h)) << std::endl << std::endl;
		std::cout << name(tlnc::jacobian((f, g, h), (x<1>, x<2>))) << std::endl << std::endl;
	}

	{
		using namespace bcl::literals;
		using namespace cti::literals;
		using namespace cti::operators;

		using tlnc::x;
		using tlnc::c;

		auto f = (1.0_dc, 2.0_dc) * x<0>;
		auto g = c((1.0_dc, 2.0_dc)) * x<0>;

		std::cout << name(f) << std::endl << std::endl;
		std::cout << name(g) << std::endl << std::endl;
	}
}

