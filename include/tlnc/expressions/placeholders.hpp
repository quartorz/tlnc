#pragma once

#include <cstdint>

#include <bcl/tuple.hpp>

#include <tlnc/expressions/constant.hpp>
#include <tlnc/expressions/detail/make_memo.hpp>

namespace tlnc{
	namespace expressions{
		template <::std::size_t I>
		struct placeholder{
			template <typename Arg>
			constexpr auto operator()(Arg &&arg) const
			{
				return ::bcl::get<I>(arg);
			}

			template <typename X>
			constexpr auto derivative() const
			{
				return TLNC_C(0.0);
			}

			template <::std::size_t Idx, typename Arg, typename Memo>
			constexpr void update_memo(Arg &&arg, Memo &memo)
			{
				::bcl::get<Idx>(memo).second = ::bcl::get<I>(arg);
			}

			template <typename Memo, typename Arg>
			using make_memo = typename detail::make_memo<placeholder<I>, Memo, Arg>;

			template <typename Memo, typename Arg>
			using make_memo_t = typename make_memo<Memo, Arg>::type;
		};
	}

	template <::std::size_t I>
	struct is_expression<expressions::placeholder<I>> : ::std::true_type{
	};

	constexpr expressions::placeholder<1> _1{};
	constexpr expressions::placeholder<2> _2{};
	constexpr expressions::placeholder<3> _3{};
	constexpr expressions::placeholder<4> _4{};
	constexpr expressions::placeholder<5> _5{};
	constexpr expressions::placeholder<6> _6{};
	constexpr expressions::placeholder<7> _7{};
	constexpr expressions::placeholder<8> _8{};
	constexpr expressions::placeholder<9> _9{};
	constexpr expressions::placeholder<10> _10{};

	template <::std::size_t I>
	constexpr expressions::placeholder<I> holder{};
}

