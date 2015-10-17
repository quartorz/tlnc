// verification of Lorenz equation
// based on Taylor expansion method

#include <tlnc/expressions.hpp>
#include <tlnc/derivative.hpp>
#include <tlnc/jacobian.hpp>
#include <tlnc/call.hpp>

#include <bcl/literals.hpp>
#include <cti/interval.hpp>
#include <cti/literals.hpp>

#include <iostream>

#include <boost/numeric/ublas/io.hpp>

template <::std::size_t K>
struct make_phi_aux{
	template <typename Func, typename ... Xs>
	constexpr auto operator()(Func &&f, const tlnc::expressions::vector<Xs...> &xs) const
	{
		using namespace cti::literals;


		auto k = CTI_I(static_cast<double>(K)){};
		auto g = make_phi_aux<K - 1>{}(f, xs);

		return (1_di / k) * (/*tlnc::derivative(g, f) + */tlnc::prod(tlnc::jacobian(g, xs), f));
	}
};

template <>
struct make_phi_aux<1>{
	template <typename Func, typename ... Xs>
	constexpr auto operator()(Func &&f, tlnc::expressions::vector<Xs...>) const
	{
		return f;
	}
};

template <::std::size_t K>
struct make_phi{
	template <typename Func, typename ... Xs, typename H>
	constexpr auto operator()(Func &&f, const tlnc::expressions::vector<Xs...> &xs, H &&h) const
	{
		return make_phi<K - 1>{}(f, xs, h) + tlnc::pow(h, BCL_DOUBLE_V(K - 1)) * make_phi_aux<K>{}(f, xs);
	}
};

template <>
struct make_phi<1>{
	template <typename Func, typename ... Xs, typename H>
	constexpr auto operator()(Func &&f, const tlnc::expressions::vector<Xs...> &xs, H &&h) const
	{
		return f;
	}
};

int main()
{
	using namespace bcl::literals;
	using namespace cti::literals;

	auto x = tlnc::x<0>;
	auto y = tlnc::x<1>;
	auto z = tlnc::x<2>;

	auto h = tlnc::_1;
	auto t = tlnc::_2;

	// Lorenz equation
	auto f = (
		-6_dc * x + 6_dc * y,
		-x * z + 28_dc * x - y,
		x * y - (8_di / 3_di) * z
	);

	// p-th order Taylor expansion
	constexpr int p = 4;
	auto phi = make_phi<p - 1>{}(f, (x, y, z), h);
	auto fp = make_phi_aux<p>{}(f, (x, y, z));


	using interval_type = kv::interval<double>;
	using vector_type = boost::numeric::ublas::c_vector<interval_type, 3>;

	bcl::tuple<vector_type, interval_type, interval_type> arg;
	interval_type t1(0.0), t2;

	// initial value
	bcl::get<0>(arg)(0) = interval_type(1.0);
	bcl::get<0>(arg)(1) = interval_type(1.0);
	bcl::get<0>(arg)(2) = interval_type(1.0);

	vector_type a, u;

	std::cout.precision(16);
	std::cout.setf(std::cout.scientific);

	std::cout
		<< mid(t1)
		<< ',' << bcl::get<0>(arg)(0).lower() << ',' << bcl::get<0>(arg)(0).upper()
		<< ',' << bcl::get<0>(arg)(1).lower() << ',' << bcl::get<0>(arg)(1).upper()
		<< ',' << bcl::get<0>(arg)(2).lower() << ',' << bcl::get<0>(arg)(2).upper()
		<< std::endl;

	for(int i = 0; i < 32768; i++){
		t2 = 1.0 * (i + 1) / 32768;

		// [t_j, t_{j+1}]
		bcl::get<2>(arg) = interval_type::hull(t1, t2);

		auto &u_j = bcl::get<0>(arg);
		auto &h = bcl::get<1>(arg);
		auto &t = bcl::get<2>(arg);

		h = t2 - t1;
		t = kv::interval<double>::hull(t1, t2);

		u = u_j;

		for(;;){
			a = u_j + kv::interval<double>(0.0, h.upper()) * tlnc::call(f, u);

			if(subset(a(0), u(0)) && subset(a(1), u(1)) && subset(a(2), u(2))){
				u = a;
				break;
			}

			constexpr double epsilon = 0.6;

			// epsilon inflation
			u(0) = (1 + epsilon) * u(0) - epsilon * u(0);
			u(1) = (1 + epsilon) * u(1) - epsilon * u(1);
			u(2) = (1 + epsilon) * u(2) - epsilon * u(2);
		}

		auto r1 = tlnc::call(phi, arg, tlnc::memo());
		auto r2  = tlnc::call(fp, u, tlnc::memo());

		auto u_next = u_j + h * r1.first + pow(h, p) * r2.first;

		// u_next includes the exact solution at t = t2
		std::cout
			<< mid(t2)
			<< ',' << u_next(0).lower() << ',' << u_next(0).upper()
			<< ',' << u_next(1).lower() << ',' << u_next(1).upper()
			<< ',' << u_next(2).lower() << ',' << u_next(2).upper()
			<< std::endl;

		bcl::get<0>(arg) = u_next;
		t1 = t2;
	}

	return 0;
}


