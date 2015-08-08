#pragma once

#include <tlnc/expressions/arguments.hpp>
#include <tlnc/expressions/detail/reduction.hpp>

namespace tlnc{
	template <typename T, typename X = ::tlnc::expressions::arg>
	constexpr auto derivative(const T &function, X = x<>)
	{
		return expressions::detail::reduction<decltype(function.template derivative<X>())>();
	}
}

