#pragma once

#include <cstdint>

#include <boost/numeric/ublas/matrix.hpp>

#include <sprout/index_tuple.hpp>

#include <bcl/tuple.hpp>

#include <tlnc/expressions/vector.hpp>
#include <tlnc/expressions/matrix.hpp>
#include <tlnc/expressions/detail/make_memo.hpp>
#include <tlnc/expressions/detail/memo_find.hpp>

namespace tlnc{
	namespace expressions{
		template <typename Expr1, typename Expr2>
		struct element_prod{
			template <typename Arg>
			constexpr auto operator()(Arg &&arg) const
			{
				return ::boost::numeric::ublas::element_prod(
					Expr1{}(arg), Expr2{}(arg)
				);
			}

			template <typename X>
			constexpr auto derivative() const
			{
				return element_prod<
					decltype(Expr1{}.template derivative<X>()),
					decltype(Expr2{}.template derivative<X>())
				>{}.reduction();
			}

			constexpr auto reduction() const
			{
				return element_prod<
					decltype(Expr1{}.reduction()),
					decltype(Expr2{}.reduction())
				>{};
			}

			template <::std::size_t I, typename Arg, typename Memo>
			constexpr void update_memo(Arg &&arg, Memo &memo) const
			{
				constexpr auto index1 = detail::memo_find_t<Expr1, Memo>::value;
				constexpr auto index2 = detail::memo_find_t<Expr2, Memo>::value;
				::bcl::get<I>(memo).second = ::boost::numeric::ublas::element_prod(
					::bcl::get<index1>(memo).second, ::bcl::get<index2>(memo).second
				);
			}

			template <typename Memo, typename Arg>
			using make_memo = detail::make_memo<element_prod<Expr1, Expr2>, Memo, Arg>;

			template <typename Memo, typename Arg>
			using make_memo_t = typename make_memo<Memo, Arg>::type;
		};
	}

	namespace detail{
		template <typename, typename>
		struct element_prod_impl;

		template <typename ... Exprs1, typename ... Exprs2>
		struct element_prod_impl<
			::tlnc::expressions::vector<Exprs1...>,
			::tlnc::expressions::vector<Exprs2...>
		>{
			using type = ::tlnc::expressions::vector<decltype(Exprs1{} * Exprs2{})...>;
		};

		template <typename ... Vectors1, typename ... Vectors2>
		struct element_prod_impl<
			::tlnc::expressions::matrix<Vectors1...>,
			::tlnc::expressions::matrix<Vectors2...>
		>{
			using type = ::tlnc::expressions::matrix<
				typename element_prod_impl<Vectors1, Vectors2>::type...
			>;
		};
	}

	template <typename ... Exprs1, typename ... Exprs2>
	constexpr auto element_prod(
		expressions::vector<Exprs1...>,
		expressions::vector<Exprs2...>
	)
	{
		using lhs_type = expressions::vector<Exprs1...>;
		using rhs_type = expressions::vector<Exprs2...>;

		static_assert(lhs_type::row_size == rhs_type::row_size);

		return typename detail::element_prod_impl<lhs_type, rhs_type>::type{};
	}

	template <typename ... Vectors1, typename ... Vectors2>
	constexpr auto element_prod(
		expressions::matrix<Vectors1...>,
		expressions::matrix<Vectors2...>
	)
	{
		using lhs_type = expressions::matrix<Vectors1...>;
		using rhs_type = expressions::matrix<Vectors2...>;

		static_assert(lhs_type::row_size == rhs_type::row_size);
		static_assert(lhs_type::column_size == rhs_type::column_size);

		return typename detail::element_prod_impl<lhs_type, rhs_type>::type{};
	}

	template <typename Expr1, typename Expr2>
	constexpr auto element_prod(Expr1 &&, Expr2 &&)
	{
		return expressions::element_prod<::std::decay_t<Expr1>, ::std::decay_t<Expr2>>{};
	}

	template <typename ... Exprs, typename Expr>
	constexpr auto element_prod(expressions::vector<Exprs...>, Expr &&)
	{
		return expressions::element_prod<expressions::vector<Exprs...>, ::std::decay_t<Expr>>{};
	}

	template <typename ... Exprs, typename Expr>
	constexpr auto element_prod(Expr &&, expressions::vector<Exprs...>)
	{
		return expressions::element_prod<::std::decay_t<Expr>, expressions::vector<Exprs...>>{};
	}

	template <typename ... Exprs, typename Expr>
	constexpr auto element_prod(expressions::matrix<Exprs...>, Expr &&)
	{
		return expressions::element_prod<expressions::matrix<Exprs...>, ::std::decay_t<Expr>>{};
	}

	template <typename ... Exprs, typename Expr>
	constexpr auto element_prod(Expr &&, expressions::matrix<Exprs...>)
	{
		return expressions::element_prod<::std::decay_t<Expr>, expressions::matrix<Exprs...>>{};
	}
}

