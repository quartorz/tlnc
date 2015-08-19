#pragma once

#include <cstdint>
#include <type_traits>

#include <tlnc/traits.hpp>
#include <tlnc/expressions/detail/reduction.hpp>
#include <tlnc/expressions/detail/make_memo.hpp>
#include <tlnc/expressions/detail/memo_find.hpp>

namespace tlnc{
	namespace expressions{
		template <typename Expr, ::std::size_t ... Is>
		struct at{
			template <typename Arg>
			constexpr auto operator()(Arg &&arg) const
			{
				return Expr{}(arg)(Is...);
			}

			template <typename X>
			constexpr auto derivative() const
			{
				return at<decltype(X{}.template derivative<X>()), Is...>{}.reduction();
			}

			constexpr auto reduction() const
			{
				return at<decltype(detail::reduction<Expr>()), Is...>{};
			}

			template <::std::size_t I, typename Arg, typename Memo>
			constexpr auto update_memo(Arg &&arg, Memo &memo) const
			{
				constexpr auto index = detail::memo_find<Expr, Memo>::value;
				::bcl::get<I>(memo).second = ::bcl::get<index>(memo).second(Is...);
			}

			template <typename Memo, typename Arg>
			using make_memo = detail::make_memo<
				at<Expr, Is...>, typename Expr::template make_memo_t<Memo, Arg>, Arg
			>;

			template <typename Memo, typename Arg>
			using make_memo_t = typename make_memo<Memo, Arg>::type;
		};
	}

	template <typename Expr, ::std::size_t ... Is>
	struct is_expression<expressions::at<Expr, Is...>> : ::std::true_type{
	};

	template <::std::size_t ... Is, typename Func>
	constexpr auto at(Func &&)
	{
		return expressions::at<::std::decay_t<Func>, Is...>{};
	}
}

