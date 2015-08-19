#pragma once

#include <boost/numeric/ublas/matrix.hpp>

#include <bcl/tmp.hpp>

#include <tlnc/expressions/vector.hpp>
#include <tlnc/expressions/matrix.hpp>
#include <tlnc/expressions/operators.hpp>
#include <tlnc/expressions/element_prod.hpp>
#include <tlnc/expressions/detail/make_memo.hpp>

namespace tlnc{
	namespace expressions{
		template <typename Expr1, typename Expr2>
		struct prod{
			template <typename Arg>
			constexpr auto operator()(Arg &&arg)
			{
				auto lhs = Expr1{}(arg);
				auto rhs = Expr2{}(arg);

				return ::boost::numeric::ublas::prod(lhs, rhs);
			}
		};
	}

	template <typename ... Vectors, typename ... Exprs>
	constexpr auto prod(
		expressions::matrix<Vectors...>,
		expressions::vector<Exprs...>
	)
	{
		static_assert((... && (sizeof...(Exprs) == Vectors::row_size)));

		using vector_type = expressions::vector<Exprs...>;

		return expressions::vector<::bcl::replace_t<
			decltype(element_prod(Vectors{}, vector_type{})),
			expressions::op_add
		>...>{};
	}

	template <typename Expr, typename ... Exprs>
	constexpr auto prod(Expr &&, expressions::vector<Exprs...>)
	{
		return expressions::prod<::std::decay_t<Expr>, expressions::vector<Exprs...>>{};
	}

	template <typename ... Exprs, typename Expr>
	constexpr auto prod(expressions::vector<Exprs...>, Expr &&)
	{
		return expressions::prod<expressions::vector<Exprs...>, ::std::decay_t<Expr>>{};
	}

	template <typename Expr, typename ... Exprs>
	constexpr auto prod(Expr &&, expressions::matrix<Exprs...>)
	{
		return expressions::prod<::std::decay_t<Expr>, expressions::matrix<Exprs...>>{};
	}

	template <typename ... Exprs, typename Expr>
	constexpr auto prod(expressions::matrix<Exprs...>, Expr &&)
	{
		return expressions::prod<expressions::matrix<Exprs...>, ::std::decay_t<Expr>>{};
	}

	template <typename Expr1, typename Expr2>
	constexpr auto prod(Expr1 &&, Expr2 &&)
	{
		return expressions::prod<::std::decay_t<Expr1>, ::std::decay_t<Expr2>>{};
	}
}

