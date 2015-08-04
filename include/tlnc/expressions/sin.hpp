#pragma once

#include <tlnc/expressions/detail/scalar_function.hpp>
#include <tlnc/expressions/detail/reduction.hpp>
#include <tlnc/expressions/cos.hpp>

namespace tlnc{
	namespace expressions{
		template <typename Expr>
		struct cos;
	}
}

TLNC_SCALAR_FUNCTION(sin, (cos<Expr>{} * detail::reduction<decltype(Expr{}.template derivative<X>())>()).reduction())

