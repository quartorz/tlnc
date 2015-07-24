#pragma once

#include <tlnc/expressions/detail/scalar_function.hpp>
#include <tlnc/expressions/constant.hpp>
#include <tlnc/expressions/pow.hpp>
#include <tlnc/expressions/cos.hpp>

TLNC_SCALAR_FUNCTION(tan, (Expr{}.template derivative<X>() / pow<cos<Expr>, decltype(TLNC_C(2.0))>{}).reduction())

