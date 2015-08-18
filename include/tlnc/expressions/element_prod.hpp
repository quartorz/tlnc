#pragma once

#include <sprout/index_tuple.hpp>

#include <tlnc/expressions/vector.hpp>
#include <tlnc/expressions/matrix.hpp>

namespace tlnc{
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
}

