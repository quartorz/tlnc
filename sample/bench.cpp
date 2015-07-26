#include <tlnc/expressions.hpp>
#include <tlnc/call.hpp>

#include <boost/numeric/ublas/vector.hpp>

#include <chrono>

#if !defined(ITERATION)
# define ITERATION (1000)
#endif

template <typename F>
void bench(F f)
{
	auto begin = std::chrono::steady_clock::now();
	f();
	auto end = std::chrono::steady_clock::now();

	std::cout << std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count() << "micro sec" << std::endl << std::endl;
}

auto g = [](auto &x){
	using std::sin;
	return cos(sin(x(0))) + cos(sin(x(0))) + cos(sin(x(0))) + cos(cos(sin(x(0)))) + cos(cos(cos(sin(x(0))))) + cos(cos(cos(cos(sin(x(0)))))) + cos(cos(cos(cos(cos(sin(x(0))))))) + cos(cos(cos(cos(cos(cos(sin(x(0)))))))) + cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0)))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0)))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0)))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0)))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0)))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0)))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0)))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0)))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0)))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0)))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0)))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0)))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0)))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0)))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0)))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0)))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0)))))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0)))))))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0)))))))))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0)))))))))))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0)))))))))))))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))))))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))))))))))))))))))))))))))))))))))))))))))))
		+ cos(sin(x(0))) + cos(sin(x(0))) + cos(sin(x(0))) + cos(cos(sin(x(0)))) + cos(cos(cos(sin(x(0))))) + cos(cos(cos(cos(sin(x(0)))))) + cos(cos(cos(cos(cos(sin(x(0))))))) + cos(cos(cos(cos(cos(cos(sin(x(0)))))))) + cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0)))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0)))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0)))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0)))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0)))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0)))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0)))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0)))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0)))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0)))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0)))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0)))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0)))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0)))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0)))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0)))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0)))))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0)))))))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0)))))))))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0)))))))))))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0)))))))))))))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))))))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x(0))))))))))))))))))))))))))))))))))))))))))))))))))));
};

int main()
{
	using namespace tlnc;
	using tlnc::sin;
	using tlnc::cos;

	boost::numeric::ublas::vector<double> v(2);
	v(0) = 0.5;
	v(1) = 0.3;

	auto f = cos(sin(x<0>())) + cos(sin(x<0>())) + cos(sin(x<0>())) + cos(cos(sin(x<0>()))) + cos(cos(cos(sin(x<0>())))) + cos(cos(cos(cos(sin(x<0>()))))) + cos(cos(cos(cos(cos(sin(x<0>())))))) + cos(cos(cos(cos(cos(cos(sin(x<0>()))))))) + cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))) + cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>()))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>()))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>()))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>()))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>()))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>()))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>()))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>()))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>()))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>()))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>()))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>()))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>()))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>()))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>()))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>()))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>()))))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>()))))))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>()))))))))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>()))))))))))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>()))))))))))))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))))))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))))))))))))))))))))))))))))))))))))))))))))
		+ cos(sin(x<0>())) + cos(sin(x<0>())) + cos(sin(x<0>())) + cos(cos(sin(x<0>()))) + cos(cos(cos(sin(x<0>())))) + cos(cos(cos(cos(sin(x<0>()))))) + cos(cos(cos(cos(cos(sin(x<0>())))))) + cos(cos(cos(cos(cos(cos(sin(x<0>()))))))) + cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))) + cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>()))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>()))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>()))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>()))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>()))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>()))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>()))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>()))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>()))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>()))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>()))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>()))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>()))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>()))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>()))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>()))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>()))))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>()))))))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>()))))))))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>()))))))))))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>()))))))))))))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))))))))))))))))))))))))))))))))))))))))))) + cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(cos(sin(x<0>())))))))))))))))))))))))))))))))))))))))))))))))))));

	bench([&]{
		std::cout << "not memoized" << std::endl;
		std::cout << f(v) << std::endl;

		double sum = 0.0;

		for(auto i = 0; i < ITERATION; ++i){
			sum += f(v);
		}

		std::cout << sum << std::endl;
	});

	bench([&]{
		auto result = tlnc::call(f, v, tlnc::memo());

		std::cout << "memoized" << std::endl;
		std::cout << result.first << std::endl;

		double sum = 0.0;

		for(auto i = 0; i < ITERATION; ++i)
			sum += tlnc::call(f, v, result.second).first;

		std::cout << sum << std::endl;
	});

	bench([&]{
		auto result = g(v);

		std::cout << "normal function" << std::endl;
		std::cout << result << std::endl;

		double sum = 0.0;

		for(auto i = 0; i < ITERATION; ++i)
				sum += g(v);

		std::cout << sum << std::endl;
	});
}

