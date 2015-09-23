#pragma once

#include <cstdint>
#include <type_traits>

#include <sprout/index_tuple.hpp>

#include <tlnc/expressions/arguments.hpp>
#include <tlnc/expressions/vector.hpp>
#include <tlnc/expressions/matrix.hpp>

namespace tlnc{
	namespace detail{
		template <::std::size_t I, typename ... Exprs, ::sprout::index_t ... Is>
		constexpr auto jacobian_impl2(
			::tlnc::expressions::vector<Exprs...> v,
			::sprout::index_tuple<Is...>
		)
		{
			using f_type = ::bcl::tuple_element_t<I, typename decltype(v)::tuple_type>;
			return ::tlnc::expressions::vector<
				decltype(f_type{}.template derivative<::tlnc::expressions::vector_arg<Is>>())...
			>{};
		}

		template <typename ... Exprs, ::sprout::index_t ... Is, ::sprout::index_t ... Js>
		constexpr auto jacobian_impl(
			::tlnc::expressions::vector<Exprs...> v,
			::sprout::index_tuple<Is...>,
			::sprout::index_tuple<Js...> t
		)
		{
			return expressions::matrix<
				decltype(jacobian_impl2<Is>(v, t))...
			>{};
		}
	}

	template <::std::size_t N, typename ... Exprs>
	constexpr auto jacobian(expressions::vector<Exprs...> v)
	{
		return detail::jacobian_impl(
			v,
			::sprout::make_index_tuple<sizeof...(Exprs)>::make(),
			::sprout::make_index_tuple<N>::make());
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
		return detail::jacobian_impl(
			v,
			::sprout::make_index_tuple<sizeof...(Exprs)>::make(),
			::sprout::index_range<I, I + N>::make());
	}

	template <
		::std::size_t I, ::std::size_t N, typename ... Exprs,
		::std::enable_if_t<(sizeof...(Exprs) > 1)>* = nullptr
	>
	constexpr auto jacobian(Exprs ...)
	{
		return jacobian<I, N>(expressions::vector<Exprs...>{});
	}

	template <typename ... Exprs, typename ... Xs>
	constexpr auto jacobian(expressions::vector<Exprs...> v, expressions::vector<Xs...>)
	{
		return detail::jacobian_impl(
			v,
			::sprout::make_index_tuple<sizeof...(Exprs)>::make(),
			::sprout::index_tuple<Xs::index...>::make());
	}

	template <typename ... Exprs, typename X>
	constexpr auto jacobian(expressions::vector<Exprs...> v, X)
	{
		return detail::jacobian_impl(
			v,
			::sprout::make_index_tuple<sizeof...(Exprs)>::make(),
			::sprout::index_tuple<X::index>::make());
	}
}

