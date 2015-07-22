#pragma once

#include <type_traits>

#include <tlnc/expressions/scalar-functions.hpp>
#include <tlnc/expressions/constant.hpp>
#include <tlnc/expressions/binary-operators.hpp>
#include <tlnc/expressions/unary-operators.hpp>
#include <tlnc/expressions/arguments.hpp>

namespace tlnc{
/*	namespace expressions{
		template <typename T, typename X>
		struct derivative;

		template <typename T, typename U, typename X>
		struct derivative<op_add<T, U>, X>{
			using type = op_add<typename derivative<T, X>::type, typename derivative<U, X>::type>;

			const op_add<T, U> func;
			const X x;

			derivative(const op_add<T, U> &f, const X x)
				: func(f), x(x)
			{
			}
		};

		template <typename T, typename U, typename X>
		struct derivative<op_sub<T, U>, X>{
			using type = op_sub<typename derivative<T, X>::type, typename derivative<U, X>::type>;
		};

		template <typename T, typename U, typename X>
		struct derivative<op_mul<T, U>, X>{
			using type = op_add<
				op_mul<typename derivative<T, X>::type, U>,
				op_mul<typename derivative<U, X>::type, T>
			>;
		};

		template <typename T, typename U, typename X>
		struct derivative<op_div<T, U>, X>{
			using type = op_sub<
				typename derivative<T, X>::type,
				op_div<op_mul<typename derivative<U, X>::type, T>, op_mul<U, U>>
			>;
		};

		template <typename T, typename X>
		struct derivative<sin<T>, X>{
			using type = op_mul<cos<T>, typename derivative<T, X>::type>;
		};

		template <typename T, typename X>
		struct derivative<cos<T>, X>{
			using type = op_mul<op_minus<sin<T>>, typename derivative<T, X>::type>;
		};
	}*/

	template <typename T, typename X = ::tlnc::expressions::arg>
	auto derivative(const T &function, X = x()) -> decltype(function.derivative<X>())
	{
		return function.derivative<X>();
	}
}
