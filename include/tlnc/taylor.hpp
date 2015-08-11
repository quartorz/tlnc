#pragma once

#include <cstdint>
#include <type_traits>

#include <tlnc/expressions.hpp>

namespace tlnc{
	namespace detail{
		template <::std::size_t N>
		struct taylor_expansion_impl{
			template <typename Func, typename X>
			constexpr auto operator()(Func 
		};
	}

	template <::std::size_t N, typename Func, typename X>
	constexpr auto taylor_expansion(Func, X &&point)
	{
		return (f + )
	}
}

