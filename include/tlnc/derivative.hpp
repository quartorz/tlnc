#pragma once

namespace tlnc{
	template <typename T, typename X = ::tlnc::expressions::arg>
	constexpr auto derivative(const T &function, X = x())
	{
		return function.template derivative<X>();
	}
}

