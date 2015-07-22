#pragma once

#include <type_traits>
#include <utility>
#include <tuple>
#include <array>

#include <tlnc/traits.hpp>
#include <tlnc/tmp/index_tuple.hpp>
#include <tlnc/expressions/expr_tuple.hpp>

namespace tlnc{
	namespace expressions{
		namespace detail{
			template <typename... Ts, typename U, int... I>
			expr_tuple<Ts..., U> operator_comma_impl(const expr_tuple<Ts...> &x, const U &y, ::tlnc::tmp::index_tuple<I...>)
			{
				return {::std::get<I>(x.exprs)..., y};
			}

			template <typename... Ts, typename U, int... I>
			expr_tuple<Ts..., U> operator_comma_impl(expr_tuple<Ts...> &&x, U &&y, ::tlnc::tmp::index_tuple<I...>)
			{
				return {::std::move(::std::get<I>(x.exprs))..., ::std::move(y)};
			}
		}

		template <typename... Ts, typename... Us>
		auto operator,(const expr_tuple<Ts...> &x, const expr_tuple<Us...> &y)
			-> expr_tuple<expr_tuple<Ts...>, expr_tuple<Us...>>
		{
			return {x, y};
		}

		template <typename... Ts, typename... Us>
		auto operator,(expr_tuple<Ts...> &&x, expr_tuple<Us...> &&y)
			-> expr_tuple<expr_tuple<Ts...>, expr_tuple<Us...>>
		{
			return {::std::move(x), ::std::move(y)};
		}

		template <typename... Ts, typename U>
		auto operator,(const expr_tuple<Ts...> &x, const U &y)
			-> expr_tuple<Ts..., U>
		{
			return detail::operator_comma_impl(x, y, ::tlnc::tmp::make_index<sizeof...(Ts)>::make());
		}

		template <typename... Ts, typename U>
		auto operator,(expr_tuple<Ts...> &&x, U y)
			-> expr_tuple<Ts..., U>
		{
			return detail::operator_comma_impl(::std::move(x), ::std::move(y), ::tlnc::tmp::make_index<sizeof...(Ts)>::make());
		}

		template <typename T, typename U>
		auto operator,(const T &x, const U &y)
			-> expr_tuple<T, U>
		{
			return {x, y};
		}
	}
}