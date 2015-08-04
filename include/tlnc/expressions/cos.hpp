#pragma once

#include <tlnc/expressions/detail/scalar_function.hpp>
#include <tlnc/expressions/detail/reduction.hpp>
#include <tlnc/expressions/sin.hpp>

namespace tlnc{
	namespace expressions{
		template <typename Expr>
		struct sin;
	}
}

TLNC_SCALAR_FUNCTION(cos, ((-sin<Expr>{}).reduction() * detail::reduction<decltype(Expr{}.template derivative<X>())>()).reduction())

