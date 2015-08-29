#pragma once

#include <type_traits>

#include <bcl/tuple.hpp>

#include <tlnc/expressions/arguments.hpp>
#include <tlnc/expressions/vector.hpp>

namespace tlnc{
	template <
		typename Func,
		typename X = ::tlnc::expressions::arg,
		::std::enable_if<is_argument<X>{}>* = nullptr
	>
	constexpr auto derivative(Func &&function, X = x<>)
	{
		return function.template derivative<X>().reduction();
	}

	template <typename Func, typename ... Xs>
	constexpr auto derivative(Func &&function, expressions::vector<Xs...>)
	{
		return function.template derivative<::bcl::tuple<::std::decay_t<Xs>...>>();
	}
}

