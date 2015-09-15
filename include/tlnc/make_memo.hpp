#pragma once

#include <type_traits>

#include <bcl/tuple.hpp>

namespace tlnc{
	template <typename Func, typename Arg>
	constexpr auto make_memo(Func &&f, Arg &&arg)
	{
		using func_t = ::std::decay_t<Func>;
		using memo_t = typename func_t::template make_memo_t<::bcl::tuple<>, Arg>;

		return memo_t{};
	}

	template <typename Func, typename Arg, typename Memo>
	constexpr auto make_memo(Func &&f, Arg &&arg, Memo &&memo)
	{
		using func_t = ::std::decay_t<Func>;
		using memo_t = typename func_t::template make_memo_t<::std::decay_t<Memo>, Arg>;

		return memo_t{};
	}
}

