#if defined(_MSC_VER)
#if !defined(_SCL_SECURE_NO_WARNINGS)
#define _SCL_SECURE_NO_WARNINGS
#endif
#if !defined(_CRT_SECURE_NO_WARNINGS)
#define _CRT_SECURE_NO_WARNINGS
#endif
#endif

#include <typeinfo>

#include <tlnc/expressions.hpp>
#include <tlnc/vectorize.hpp>
#include <tlnc/derivative.hpp>
#include <tlnc/jacobi.hpp>
#include <tlnc/simplify.hpp>

#include <kv/interval.hpp>
#include <kv/rdouble.hpp>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/io.hpp>

#if defined(_MSC_VER)
#include <conio.h>
#endif

int main()
{
	using interval_t = kv::interval<double>;

	{
		// f(x) = cos(sin(x))
		// g(x) = f'(x)
		// h(x) = g'(x)

		auto f = tlnc::cos(tlnc::sin(tlnc::x()));
		auto g = tlnc::derivative(f, tlnc::x());
		auto h = tlnc::derivative(g);
		auto fgh = (f, g, h);

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
		std::cout << "(h(1.0), h(2.0)) = " << tlnc::vectorize(h)(x) << std::endl;
		std::cout << "(f(1.0), g(1.0), h(1.0)) = " << fgh(1.0) << std::endl;
		std::cout << "(f'(1.0), g'(1.0), h'(1.0)) = " << tlnc::derivative(fgh)(1.0) << std::endl << ::std::endl;

		std::cout << "type of f : " << typeid(f).name() << std::endl << std::endl;
		std::cout << "type of g : " << typeid(g).name() << std::endl << std::endl;
		std::cout << "type of h : " << typeid(h).name() << std::endl << std::endl;
		std::cout << "type of h' : " << typeid(tlnc::derivative(h, tlnc::x())).name() << std::endl << std::endl;
		std::cout << "type of (f, g, h) : " << typeid(fgh).name() << std::endl << std::endl;
	}

	{
		// f(x1, x2) = x1 * x2 + sin(x1)
		// g(x1, x2) = Ýf / Ýx1 = x2 + cos(x1)
		// h(x1, x2) = Ýf / Ýx2 = x1

		auto f = tlnc::x<0>() * tlnc::x<1>() + tlnc::sin(tlnc::x<0>());
		auto g = tlnc::derivative(f, tlnc::x<0>());
		auto h = tlnc::derivative(f, tlnc::x<1>());
		auto fgh = (f, g, h);

		auto x = boost::numeric::ublas::vector<double>(2);
		x(0) = 1.0;
		x(1) = 2.0;

		auto y = boost::numeric::ublas::vector<interval_t>(2);
		y(0) = interval_t(1.0, 2.0);
		y(1) = interval_t(3.0, 4.0);

		std::cout << "f(x1, x2) = x1 * x2 + sin(x1)" << std::endl;
		std::cout << "g(x1, x2) = Ýf / Ýx1 = x2 + cos(x1)" << std::endl;
		std::cout << "h(x1, x2) = Ýf / Ýx2 = x1" << std::endl << std::endl;

		std::cout << "f(1.0, 2.0) = " << f(x) << std::endl;
		std::cout << "f(1.0, 2.0) = " << tlnc::simplify(f)({1.0, 2.0}) << std::endl;
		std::cout << "g(1.0, 2.0) = " << g(x) << std::endl;
		std::cout << "h(1.0, 2.0) = " << h(x) << std::endl;
		std::cout << "g([1.0, 2.0], [3.0, 4.0]) = " << g(y) << std::endl;
		std::cout << "(f(1.0, 2.0), g(1.0, 2.0), h(1.0, 2.0)) = " << fgh(x) << std::endl << std::endl;

		std::cout << "type of f : " << typeid(f).name() << std::endl << std::endl;
		std::cout << "type of g : " << typeid(g).name() << std::endl << std::endl;
		std::cout << "type of h : " << typeid(h).name() << std::endl << std::endl;
		std::cout << "type of (f, g, h) : " << typeid(fgh).name() << std::endl << std::endl;
		std::cout << "type of (Ýf/Ýx1, Ýg/Ýx1, Ýh/Ýx1) : " << typeid(tlnc::derivative(fgh, tlnc::x<0>())).name() << std::endl << std::endl;

		std::cout << (fgh, fgh)(x) << std::endl;
		std::cout << ((f, g, h), (f, g, h))(x) << std::endl;
		std::cout << (((f, g, h), (f, g, h)), ((f, g, h), (f, g, h)))(x) << std::endl;
		std::cout << (fgh, tlnc::derivative(fgh, tlnc::x<0>()))(x) << std::endl;
		std::cout << tlnc::derivative((((f, g, h), (f, g, h)), ((f, g, h), (f, g, h))), tlnc::x<0>())(x) << std::endl << std::endl;
	}

	{
		// x(r, ƒÆ) = r * cosƒÆ
		// y(r, ƒÆ) = r * sinƒÆ

		auto r = tlnc::x<0>();
		auto theta = tlnc::x<1>();

		auto x = r * tlnc::cos(theta);
		auto y = r * tlnc::sin(theta);

		auto v = boost::numeric::ublas::vector<interval_t>(2);
		v(0) = interval_t(2.0); // r
		v(1) = interval_t(1.0); // theta

		auto j = tlnc::jacobi_matrix<2>((x, y))(v);

		std::cout << "x(r, ƒÆ) = r * cosƒÆ" << std::endl;
		std::cout << "y(r, ƒÆ) = r * sinƒÆ" << std::endl << std::endl;

		std::cout << "x(2.0, 1.0) = " << tlnc::simplify(x)({2.0, 1.0}) << std::endl << std::endl;

		std::cout << "type of Ýx/ÝƒÆ : " << typeid(tlnc::derivative(x, theta)).name() << std::endl << std::endl;

		std::cout << j << std::endl;
		std::cout << j(0, 0) * j(1, 1) - j(0, 1) * j(1, 0) << std::endl << std::endl;
	}

#if defined(_MSC_VER)
	std::cout << "press any key to exit" << std::endl;
	_getch();
#endif
}
