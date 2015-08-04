#pragma once

#include <type_traits>

#include <bcl/has_xxx.hpp>

namespace tlnc::expressions::detail{
	BCL_HAS_FUNCTION(reduction)

	template <
		typename Func,
		::std::enable_if_t<
			has_reduction_func<Func>{}
		>* = nullptr
	>
	constexpr auto reduction()
	{
		return ::std::decay_t<Func>{}.reduction();
	}

	template <
		typename Func,
		::std::enable_if_t<
			!has_reduction_func<Func>{}
		>* = nullptr
	>
	constexpr auto reduction()
	{
		return ::std::decay_t<Func>{};
	}

}

