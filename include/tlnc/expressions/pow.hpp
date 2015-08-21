#pragma once

#include <type_traits>

#include <tlnc/traits.hpp>
#include <tlnc/generic.hpp>
#include <tlnc/expressions/constant.hpp>
#include <tlnc/expressions/detail/make_memo.hpp>
#include <tlnc/expressions/detail/memo_find.hpp>

namespace tlnc{
	namespace expressions{
		template <typename Expr>
		struct log;

		template <typename Base, typename Exponent>
		struct pow{
			using base_type = Base;
			using exponent_type = Exponent;

			template <typename Arg>
			constexpr auto operator()(Arg &&x) const
			{
				return ::tlnc::generic::pow{}(Base{}(x), Exponent{}(x));
			}

			template <typename X>
			constexpr auto derivative() const
			{
				return (
					(Exponent{} * log<Base>{}).template derivative<X>()
					* pow<Base, Exponent>{}
				).reduction();
			}

			constexpr auto reduction() const
			{
				return pow<decltype(Base{}.reduction()), decltype(Exponent{}.reduction())>{};
			}

			template <::std::size_t I, typename Arg, typename Memo>
			constexpr void update_memo(Arg &&arg, Memo &memo) const
			{
				constexpr ::std::size_t index1 = detail::memo_find_t<Base, Memo>::value;
				constexpr ::std::size_t index2 = detail::memo_find_t<Exponent, Memo>::value;
				::bcl::get<I>(memo).second = ::tlnc::generic::pow{}(
					::bcl::get<index1>(memo).second,
					::bcl::get<index2>(memo).second
				);
			}

			template <typename Memo, typename Arg>
			using make_memo = detail::make_memo<pow<Base, Exponent>, Memo, Arg>;

			template <typename Memo, typename Arg>
			using make_memo_t = typename make_memo<Memo, Arg>::type;
		};
	}

	namespace functions{
		struct pow{
			template <
				typename T, typename U,
				::std::enable_if_t<
					::tlnc::is_expression_v<::std::decay_t<T>>
					&& ::tlnc::is_expression_v<::std::decay_t<U>>
				>* = nullptr
			>
			constexpr auto operator()(T &&, U &&) const
			{
				return ::tlnc::expressions::pow<::std::decay_t<T>, ::std::decay_t<U>>{};
			}

			template <
				typename T, typename U,
				::std::enable_if_t<
					::tlnc::is_expression_v<::std::decay_t<T>>
					&& !tlnc::is_expression_v<::std::decay_t<U>>
					&& ::tlnc::is_value<::std::decay_t<U>>{}
				>* = nullptr
			>
			constexpr auto operator()(T &&, U &&) const
			{
				return ::tlnc::expressions::pow<
					::std::decay_t<T>, ::tlnc::expressions::constant<::std::decay_t<U>>
				>{};
			}

			template <
				typename T, typename U,
				::std::enable_if_t<
					!::tlnc::is_expression_v<::std::decay_t<T>>
					&& ::tlnc::is_expression_v<::std::decay_t<U>>
					&& ::tlnc::is_value<::std::decay_t<T>>{}
				>* = nullptr
			>
			constexpr auto operator()(T &&, U &&) const
			{
				return ::tlnc::expressions::pow<
					::tlnc::expressions::constant<::std::decay_t<T>>, ::std::decay_t<U>
				>{};
			}

			template <
				typename T, typename U,
				::std::enable_if_t<
					!::tlnc::is_expression_v<::std::decay_t<T>>
					&& !::tlnc::is_expression_v<::std::decay_t<U>>
				>* = nullptr
			>
			constexpr auto operator()(T &&x, U &&y) const
			{
				return ::tlnc::generic::pow{}(x, y);
			}
		};
	}

	template <typename Base, typename Exponent>
	struct is_expression<expressions::pow<Base, Exponent>> : ::std::true_type{
	};

	constexpr functions::pow pow{};
}

