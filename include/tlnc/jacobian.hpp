#pragma once

#include <cstdint>
#include <type_traits>

#include <sprout/index_tuple.hpp>

#include <tlnc/expressions/arguments.hpp>
#include <tlnc/expressions/vector.hpp>
#include <tlnc/expressions/matrix.hpp>

namespace tlnc{
	namespace detail{
		template <::std::size_t I, typename ... Exprs>
		constexpr auto jacobian_impl2(::tlnc::expressions::vector<Exprs...>)
		{
			return ::tlnc::expressions::vector<
				decltype(Exprs{}.template derivative<::tlnc::expressions::vector_arg<I>>())...
			>{};
		}

		template <typename ... Exprs, ::sprout::index_t ... Is>
		constexpr auto jacobian_impl(
			::tlnc::expressions::vector<Exprs...> v,
			::sprout::index_tuple<Is...>
		)
		{
			return expressions::matrix<decltype(jacobian_impl2<Is>(v))...>{};
		}
	}

	template <::std::size_t N, typename ... Exprs>
	constexpr auto jacobian(expressions::vector<Exprs...> v)
	{
		return detail::jacobian_impl(v, ::sprout::make_index_tuple<N>::make());
	}

	template <
		::std::size_t N, typename ... Exprs,
		::std::enable_if_t<(sizeof...(Exprs) > 1)>* = nullptr
	>
	constexpr auto jacobian(Exprs ...)
	{
		return jacobian<N>(expressions::vector<Exprs...>{});
	}

	template <::std::size_t I, ::std::size_t N, typename ... Exprs>
	constexpr auto jacobian(expressions::vector<Exprs...> v)
	{
		return detail::jacobian_impl(v, ::sprout::index_range<I, I + N>::make());
	}

	template <
		::std::size_t I, ::std::size_t N, typename ... Exprs,
		::std::enable_if_t<(sizeof...(Exprs) > 1)>* = nullptr
	>
	constexpr auto jacobian(Exprs ...)
	{
		return jacobian<I, N>(expressions::vector<Exprs...>{});
	}

	template <typename ... Exprs, typename ... Args>
	constexpr auto jacobian(expressions::vector<Exprs...> v, expressions::vector<Args...>)
	{
		return detail::jacobian_impl(v, ::sprout::index_tuple<Args::index...>::make());
	}
}

