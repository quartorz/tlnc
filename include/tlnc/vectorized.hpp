#pragma once

#include <cmath>
#include <type_traits>

#include <tlnc/vectorize.hpp>
#include <tlnc/generic.hpp>

namespace tlnc{
	namespace vectorized{

		const auto sin = ::tlnc::vectorize(::tlnc::generic::sin());
		const auto cos = ::tlnc::vectorize(::tlnc::generic::cos());
	}
}
