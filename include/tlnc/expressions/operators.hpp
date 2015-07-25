#pragma once

#include <type_traits>

#include <sprout/index_tuple.hpp>

#include <bcl/tuple.hpp>
#include <bcl/tuple_operation.hpp>
#include <bcl/value_tuple.hpp>
#include <bcl/double.hpp>

#include <tlnc/traits.hpp>
#include <tlnc/expressions/constant.hpp>
#include <tlnc/expressions/pow.hpp>
#include <tlnc/expressions/detail/make_memo.hpp>

namespace tlnc{
	namespace expressions{
		template <typename ... Exprs>
		struct op_add;

		template <typename ... Exprs>
		struct op_mul;
	}
}

namespace tlnc{
	namespace expressions{
		template <typename ... Exprs>
		struct op_add{
			template <typename Arg>
			constexpr auto operator()(Arg &&x) const
			{
				return (Exprs{}(x) + ...);
			}

			template <typename X>
			constexpr auto derivative() const
			{
				return op_add<decltype(Exprs{}.template derivative<X>())...>{};
			}

			constexpr auto reduction() const
			{
				return op_add<Exprs...>{};
			}

			template <typename Memo, typename Arg>
			using make_memo = detail::make_memo<op_add<Exprs...>, Memo, Arg>;

			template <typename Memo, typename Arg>
			using make_memo_t = typename make_memo<Memo, Arg>::type;
		};

		namespace detail{
			template <typename X, typename Tuple, ::std::size_t I, ::std::size_t N>
			struct op_mul_derivative_impl{
				using left = op_mul_derivative_impl<X, Tuple, I, N / 2 + N % 2>;
				using right = op_mul_derivative_impl<X, Tuple, I + N / 2 + N % 2, N / 2>;

				using type = ::bcl::tuple_concat_t<typename left::type, typename right::type>;
			};

			template <typename X, typename Tuple, ::std::size_t I>
			struct op_mul_derivative_impl<X, Tuple, I, 0>{
				using type = ::bcl::tuple<>;
			};

			template <typename X, typename ... Exprs, ::std::size_t I>
			struct op_mul_derivative_impl<X, ::bcl::tuple<Exprs...>, I, 1>{
				using tuple = ::bcl::tuple<Exprs...>;
				using expr = ::bcl::tuple_element_t<I, tuple>;
				using others = ::bcl::tuple_remove_t<::bcl::value_tuple<::std::size_t, I>, tuple>;
				using type = ::bcl::tuple<
					::bcl::tuple_transform_t<
						::tlnc::expressions::op_mul,
						::bcl::tuple_concat_t<
							::bcl::tuple<decltype(expr{}.template derivative<X>())>,
							others
						>
					>
				>;
			};
		}

		template <typename ... Exprs>
		struct op_mul{
			template <typename Arg>
			constexpr auto operator()(Arg &&x) const
			{
				return (Exprs{}(x) * ...);
			}

			template <typename X>
			constexpr auto derivative() const
			{
				using tuple = typename detail::op_mul_derivative_impl<
					X, ::bcl::tuple<Exprs...>, 0, sizeof...(Exprs)
				>::type;
				using result = ::bcl::tuple_transform_t<op_add, tuple>;
				return result{};
			}

			constexpr auto reduction() const
			{
				return op_mul<Exprs...>{};
			}

			template <typename Memo, typename Arg>
			using make_memo = detail::make_memo<op_mul<Exprs...>, Memo, Arg>;

			template <typename Memo, typename Arg>
			using make_memo_t = typename make_memo<Memo, Arg>::type;
		};
	}

	template <typename ... Exprs>
	struct is_expression<expressions::op_add<Exprs...>> : ::std::true_type{
	};

	template <typename ... Exprs>
	struct is_expression<expressions::op_mul<Exprs...>> : ::std::true_type{
	};
}

namespace tlnc{
	namespace expressions{
		template <typename ... ExprsL, typename ... ExprsR>
		constexpr auto operator+(op_add<ExprsL...>, op_add<ExprsR...>)
		{
			return op_add<ExprsL..., ExprsR...>{};
		}

		template <
			typename ... Exprs, typename U,
			::std::enable_if_t<
				::tlnc::is_expression_v<::std::decay_t<U>>
			>* = nullptr
		>
		constexpr auto operator+(op_add<Exprs...>, U &&)
		{
			return op_add<Exprs..., ::std::decay_t<U>>{};
		}

		template <
			typename ... Exprs, typename U,
			::std::enable_if_t<
				::tlnc::is_value<::std::decay_t<U>>{}
			>* = nullptr
		>
		constexpr auto operator+(op_add<Exprs...>, U &&)
		{
			return op_add<Exprs..., constant<::std::decay_t<U>>>{};
		}

		template <
			typename T, typename ... Exprs,
			::std::enable_if_t<
				::tlnc::is_expression_v<::std::decay_t<T>>
			>* = nullptr
		>
		constexpr auto operator+(T &&, op_add<Exprs...>)
		{
			return op_add<::std::decay_t<T>, Exprs...>{};
		}

		template <
			typename T, typename ... Exprs,
			::std::enable_if_t<
				::tlnc::is_value<::std::decay_t<T>>{}
			>* = nullptr
		>
		constexpr auto operator+(T &&, op_add<Exprs...>)
		{
			return op_add<constant<::std::decay_t<T>>, Exprs...>{};
		}

		template <
			typename T, typename U,
			::std::enable_if_t<
				::tlnc::is_expression_v<::std::decay_t<T>>
				&& ::tlnc::is_expression_v<::std::decay_t<U>>
			>* = nullptr
		>
		constexpr auto operator+(T &&, U &&)
		{
			return op_add<::std::decay_t<T>, ::std::decay_t<U>>{};
		}

		template <
			typename T, typename U,
			::std::enable_if_t<
				::tlnc::is_expression_v<::std::decay_t<T>>
				&& ::tlnc::is_value<::std::decay_t<U>>{}
			>* = nullptr
		>
		constexpr auto operator+(T &&, T &&)
		{
			return op_add<::std::decay_t<T>, constant<::std::decay_t<U>>>{};
		}

		template <
			typename T, typename U,
			::std::enable_if_t<
				::tlnc::is_value<::std::decay_t<T>>{}
				&& ::tlnc::is_expression_v<::std::decay_t<U>>
			>* = nullptr
		>
		constexpr auto operator+(T &&, U &&)
		{
			return op_add<constant<::std::decay_t<T>>, ::std::decay_t<U>>{};
		}

		template <typename T, typename U>
		constexpr auto operator-(T &&x, U &&y)
		{
			return x + BCL_DOUBLE_V(-1.0) * y;
		}

		template <typename ... ExprsL, typename ... ExprsR>
		constexpr auto operator*(op_mul<ExprsL...>, op_mul<ExprsR...>)
		{
			return op_mul<ExprsL..., ExprsR...>{};
		}

		template <
			typename ... Exprs, typename U,
			::std::enable_if_t<
				::tlnc::is_expression_v<::std::decay_t<U>>
			>* = nullptr
		>
		constexpr auto operator*(op_mul<Exprs...>, U &&)
		{
			return op_mul<Exprs..., ::std::decay_t<U>>{};
		}

		template <
			typename ... Exprs, typename U,
			::std::enable_if_t<
				::tlnc::is_value<::std::decay_t<U>>{}
			>* = nullptr
		>
		constexpr auto operator*(op_mul<Exprs...>, U &&)
		{
			return op_mul<Exprs..., constant<::std::decay_t<U>>>{};
		}

		template <
			typename T, typename ... Exprs,
			::std::enable_if_t<
				::tlnc::is_expression_v<::std::decay_t<T>>
			>* = nullptr
		>
		constexpr auto operator*(T &&, op_mul<Exprs...>)
		{
			return op_mul<::std::decay_t<T>, Exprs...>{};
		}

		template <
			typename T, typename ... Exprs,
			::std::enable_if_t<
				::tlnc::is_value<::std::decay_t<T>>{}
			>* = nullptr
		>
		constexpr auto operator*(T &&, op_mul<Exprs...>)
		{
			return op_mul<constant<::std::decay_t<T>>, Exprs...>{};
		}

		template <
			typename T, typename U,
			::std::enable_if_t<
				::tlnc::is_expression_v<::std::decay_t<T>>
				&& ::tlnc::is_expression_v<::std::decay_t<T>>
			>* = nullptr
		>
		constexpr auto operator*(T &&, U &&)
		{
			return op_mul<::std::decay_t<T>, ::std::decay_t<U>>{};
		}

		template <
			typename T, typename U,
			::std::enable_if_t<
				::tlnc::is_expression_v<::std::decay_t<T>>
				&& ::tlnc::is_value<::std::decay_t<U>>{}
			>* = nullptr
		>
		constexpr auto operator*(T &&, U &&)
		{
			return op_mul<::std::decay_t<T>, constant<::std::decay_t<U>>>{};
		}

		template <
			typename T, typename U,
			::std::enable_if_t<
				::tlnc::is_value<::std::decay_t<T>>{}
				&& ::tlnc::is_expression_v<::std::decay_t<U>>
			>* = nullptr
		>
		constexpr auto operator*(T &&, U &&)
		{
			return op_mul<constant<::std::decay_t<T>>, ::std::decay_t<U>>{};
		}

		template <typename T, typename U>
		constexpr auto operator/(T &&x, U &&)
		{
			return x * pow<U, decltype(TLNC_C(-1.0))>{};
		}

		template <typename T>
		constexpr auto operator-(T &&x)
		{
			return TLNC_C(-1.0) * x;
		}
	}
}

