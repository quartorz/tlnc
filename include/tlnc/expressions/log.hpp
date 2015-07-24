#pragma once

#include <tlnc/generic.hpp>
#include <tlnc/expressions/constant.hpp>

namespace tlnc{
	namespace expressions{
		template <typename Base, typename Exponent>
		struct pow;

		template <typename Expr>
		struct log{
			template <typename Arg>
			constexpr auto operator()(Arg &&x) const
			{
				return ::tlnc::generic::log{}(Expr{}(x));
			}

			template <typename X>
			constexpr auto derivative() const
			{
				return (pow<Expr, decltype(TLNC_C(-1.0))>{} * Expr{}.template derivative<X>()).reduction();
			}
		};
	}

	namespace functions{
		struct log{
			template <
				typename T,
				::std::enable_if_t<
					::tlnc::is_expression_v<T>
				>* = nullptr
			>
			constexpr auto operator()(T &&) const
			{
				return ::tlnc::expressions::log<T>{};
			}

			template <
				typename T,
				::std::enable_if_t<
					!::tlnc::is_expression_v<T>
				>* = nullptr
			>
			constexpr auto operator()(T &&x) const
			{
				return ::tlnc::generic::log(x);
			}
		};
	}

	template <typename Expr>
	struct is_expression<expressions::log<Expr>> : ::std::true_type{
	};

	constexpr functions::log log{};
}

