#pragma once

#include <cstdint>

#include <bcl/tuple.hpp>

#include <tlnc/generic.hpp>
#include <tlnc/expressions/constant.hpp>
#include <tlnc/expressions/detail/make_memo.hpp>
#include <tlnc/expressions/detail/memo_find.hpp>
#include <tlnc/expressions/pow.hpp>

namespace tlnc{
	namespace expressions{
		template <typename Base, typename Exponent>
		struct pow;

		template <typename Expr>
		struct log{
			template <typename Arg>
			constexpr auto operator()(Arg &&x) const
			{
				return ::tlnc::generic::log{}(Expr{}(x));
			}

			template <typename X>
			constexpr auto derivative() const
			{
				return (pow<Expr, decltype(TLNC_C(-1.0))>{} * Expr{}.template derivative<X>()).reduction();
			}

			template <::std::size_t I, typename Arg, typename Memo>
			constexpr void update_memo(Arg &&arg, Memo &memo) const
			{
				constexpr ::std::size_t index = detail::memo_find_t<Expr, Memo>::value;
				::bcl::get<I>(memo).second = ::tlnc::generic::log{}(::bcl::get<index>(memo).second);
			}

			template <typename Memo, typename Arg>
			using make_memo = detail::make_memo<log<Expr>, Memo, Arg>;

			template <typename Memo, typename Arg>
			using make_memo_t = typename make_memo<Memo, Arg>::type;
		};
	}

	namespace functions{
		struct log{
			template <
				typename T,
				::std::enable_if_t<
					::tlnc::is_expression_v<::std::decay_t<T>>
				>* = nullptr
			>
			constexpr auto operator()(T &&) const
			{
				return ::tlnc::expressions::log<::std::decay_t<T>>{};
			}

			template <
				typename T,
				::std::enable_if_t<
					!::tlnc::is_expression_v<::std::decay_t<T>>
				>* = nullptr
			>
			constexpr auto operator()(T &&x) const
			{
				return ::tlnc::generic::log(x);
			}
		};
	}

	template <typename Expr>
	struct is_expression<expressions::log<Expr>> : ::std::true_type{
	};

	constexpr functions::log log{};
}

