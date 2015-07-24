#pragma once

#include <tlnc/expressions/detail/scalar_function.hpp>
#include <tlnc/expressions/cos.hpp>

namespace tlnc{
	namespace expressions{
		template <typename Expr>
		struct cos;
	}
}

TLNC_SCALAR_FUNCTION(sin, (cos<Expr>{} * Expr{}.template derivative<X>()).reduction())

