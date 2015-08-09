#pragma once

#include <type_traits>
#include <cstdint>

#include <boost/numeric/ublas/vector.hpp>

#include <sprout/index_tuple.hpp>

#include <bcl/tuple.hpp>
#include <bcl/value_tuple.hpp>
#include <bcl/double.hpp>
#include <bcl/has_xxx.hpp>

#include <tlnc/traits.hpp>
#include <tlnc/expressions/constant.hpp>
#include <tlnc/expressions/pow.hpp>
#include <tlnc/expressions/detail/make_memo.hpp>
#include <tlnc/expressions/detail/memo_find.hpp>
#include <tlnc/expressions/detail/reduction.hpp>

namespace tlnc{
	namespace expressions{
		template <typename ... Exprs>
		struct op_add;

		template <typename ... Exprs>
		struct op_mul;

		template <typename ... Exprs>
		struct op_comma;

		namespace detail{
			BCL_HAS_FUNCTION(expand)

			template <
				typename Func,
				::std::enable_if_t<
					has_expand_func<Func>{}
				>* = nullptr
			>
			constexpr auto expand()
			{
				return ::std::decay_t<Func>{}.expand();
			}

			template <
				typename Func,
				::std::enable_if_t<
					!has_expand_func<Func>{}
				>* = nullptr
			>
			constexpr auto expand()
			{
				return ::std::decay_t<Func>{};
			}
		}
	}
}

namespace tlnc{
	namespace expressions{
		// for op_add::reduction
		namespace detail{
			template <typename Tuple, ::std::size_t I, ::std::size_t N>
			class op_add_reduction_impl{
				using left = op_add_reduction_impl<Tuple, I, N / 2 + N % 2>;
				using right = op_add_reduction_impl<Tuple, I + N / 2 + N % 2, N / 2>;

			public:
				using type = ::bcl::tuple_concat_t<typename left::type, typename right::type>;
			};

			template <typename Tuple, ::std::size_t I>
			struct op_add_reduction_impl<Tuple, I, 0>{
				using type = ::bcl::tuple<>;
			};

			template <typename Tuple, ::std::size_t I>
			class op_add_reduction_impl<Tuple, I, 1>{
				using element = decltype(reduction<::bcl::tuple_element_t<I, Tuple>>());

			public:
				using type = ::std::conditional_t<
					::tlnc::is_zero<element>{},
					::bcl::tuple<>,
					::bcl::tuple<element>
				>;
			};

			template <typename T>
			struct op_add_from_tuple{
				using type = T;
			};

			template <>
			struct op_add_from_tuple<::bcl::tuple<>>{
				using type = decltype(TLNC_C(0.0));
			};

			template <typename T>
			struct op_add_from_tuple<::bcl::tuple<T>>{
				using type = T;
			};

			template <typename T, typename U, typename ... Ts>
			struct op_add_from_tuple<::bcl::tuple<T, U, Ts...>>{
				using type = ::tlnc::expressions::op_add<T, U, Ts...>;
			};
		}

		template <typename ... Exprs>
		struct op_add{
			template <typename Arg>
			constexpr auto operator()(Arg &&x) const
			{
				return (... + Exprs{}(x));
			}

			template <typename X>
			constexpr auto derivative() const
			{
				return op_add<decltype(Exprs{}.template derivative<X>())...>{}.reduction();
			}

			constexpr auto reduction() const
			{
				using tuple = typename detail::op_add_reduction_impl<
					::bcl::tuple<decltype(detail::reduction<Exprs>())...>,
					0, sizeof...(Exprs)
				>::type;
				return typename detail::op_add_from_tuple<tuple>::type{};
			}

			constexpr auto expand() const
			{
				return op_add<decltype(detail::expand<Exprs>())...>{};
			}

		private:
			template <::std::size_t I, typename Arg, typename Memo, ::sprout::index_t ... Is>
			constexpr void update_memo_impl(Arg &&arg, Memo &memo, ::sprout::index_tuple<Is...>) const
			{
				::bcl::get<I>(memo).second = (... + ::bcl::get<Is>(memo).second);
			}

		public:
			template <::std::size_t I, typename Arg, typename Memo>
			constexpr void update_memo(Arg &&arg, Memo &memo) const
			{
				update_memo_impl<I>(
					arg, memo,
					::sprout::index_tuple<detail::memo_find_t<Exprs, Memo>::value...>::make());
			}

			template <typename Memo, typename Arg>
			using make_memo = detail::make_memo<op_add<Exprs...>, Memo, Arg>;

			template <typename Memo, typename Arg>
			using make_memo_t = typename make_memo<Memo, Arg>::type;
		};

		template <typename ... Exprs1, typename ... Exprs2>
		struct op_add<op_add<Exprs1...>, Exprs2...> : op_add<Exprs1..., Exprs2...>{
		};

		// for op_mul::derivative
		namespace detail{
			template <typename X, typename Tuple, ::std::size_t I, ::std::size_t N>
			class op_mul_derivative_impl{
				using left = op_mul_derivative_impl<X, Tuple, I, N / 2 + N % 2>;
				using right = op_mul_derivative_impl<X, Tuple, I + N / 2 + N % 2, N / 2>;

			public:
				using type = ::bcl::tuple_concat_t<typename left::type, typename right::type>;
			};

			template <typename X, typename Tuple, ::std::size_t I>
			struct op_mul_derivative_impl<X, Tuple, I, 0>{
				using type = ::bcl::tuple<>;
			};

			template <typename X, typename ... Exprs, ::std::size_t I>
			class op_mul_derivative_impl<X, ::bcl::tuple<Exprs...>, I, 1>{
				using tuple = ::bcl::tuple<Exprs...>;
// 				using expr = ::bcl::tuple_element_t<I, tuple>;
// 				using others = ::bcl::tuple_remove_t<::bcl::value_tuple<::std::size_t, I>, tuple>;
				using derivative = decltype(::bcl::tuple_element_t<I, tuple>{}.template derivative<X>());
				using replaced = ::bcl::tuple_replace_t<I, derivative, tuple>;

			public:
				using type = ::bcl::tuple<
					::bcl::tuple_transform_t<
						::tlnc::expressions::op_mul,
						replaced
					>
				>;
			};
		}

		// for op_mul::reduction
		namespace detail{
			// op_mul_reduction_impl<Tuple, ...>::value == 0 : Tuple may not contain zero
			// op_mul_reduction_impl<Tuple, ...>::value == 1 : Tuple contains zero

			template <int, typename, typename>
			struct op_mul_reduction_aux{
				using type = decltype(TLNC_C(0.0));
			};

			template <typename L, typename R>
			struct op_mul_reduction_aux<0, L, R>{
				using type = ::bcl::tuple_concat_t<typename L::type, typename R::type>;
			};

			template <typename Tuple, ::std::size_t I, ::std::size_t N>
			class op_mul_reduction_impl{
				using left = op_mul_reduction_impl<Tuple, I, N / 2 + N % 2>;
				using right = op_mul_reduction_impl<Tuple, I + N / 2 + N % 2, N / 2>;

			public:
				static constexpr int value = (left::value == 1 || right::value == 1) ? 1 : 0;

				using type = typename op_mul_reduction_aux<value, left, right>::type;
			};

			template <typename Tuple, ::std::size_t I>
			struct op_mul_reduction_impl<Tuple, I, 0>{
				using type = decltype(TLNC_C(0.0));
				static constexpr int value = 0;
			};

			template <typename Tuple, ::std::size_t I>
			class op_mul_reduction_impl<Tuple, I, 1>{
				using element = decltype(reduction<::bcl::tuple_element_t<I, Tuple>>());

			public:
				static constexpr int value = ::tlnc::is_zero<element>{} ? 1 : 0;

				using type = ::std::conditional_t<
					value == 1,
					decltype(TLNC_C(0.0)),
					::std::conditional_t<
						::tlnc::is_one<element>{},
						::bcl::tuple<>,
						::bcl::tuple<element>
					>
				>;
			};

			template <typename T>
			struct op_mul_reduction_from_tuple{
				using type = T;
			};

			template <>
			struct op_mul_reduction_from_tuple<::bcl::tuple<>>{
				using type = decltype(TLNC_C(0.0));
			};

			template <typename T>
			struct op_mul_reduction_from_tuple<::bcl::tuple<T>>{
				using type = T;
			};

			template <typename T, typename U, typename ... Ts>
			struct op_mul_reduction_from_tuple<::bcl::tuple<T, U, Ts...>>{
				using type = ::tlnc::expressions::op_mul<T, U, Ts...>;
			};
		}

		// fpr op_mul::expand
		namespace detail{
			template <typename T>
			struct op_add_trait{
				static constexpr bool is_add = false;
			};

			template <typename ... Exprs>
			struct op_add_trait<::tlnc::expressions::op_add<Exprs...>>{
				static constexpr bool is_add = true;
				static constexpr ::std::size_t size = sizeof...(Exprs);
			};

			template <::std::size_t I, ::std::size_t N>
			struct op_mul_expand_pair{
			};

			template <::std::size_t I>
			struct op_mul_expand_single{
			};

			template <typename T, typename U>
			struct op_mul_expand_dispatch;

			template <::std::size_t I, ::std::size_t N, typename ... Exprs>
			class op_mul_expand_dispatch<op_mul_expand_pair<I, N>, op_mul<Exprs...>>{
				using tuple = ::bcl::tuple<Exprs...>;
				// expr_add is assumed to be op_add<Exprs...>
				using expr_add = ::bcl::tuple_element_t<I, tuple>;

			public:
				using type = ::bcl::tuple_element_t<N, ::bcl::tuple_from_t<expr_add>>;
			};

			template <::std::size_t I, typename ... Exprs>
			class op_mul_expand_dispatch<op_mul_expand_single<I>, op_mul<Exprs...>>{
				using tuple = ::bcl::tuple<Exprs...>;

			public:
				using type = ::bcl::tuple_element_t<I, tuple>;
			};

			template <::std::size_t I, typename Index>
			struct op_mul_expand_perm;

			template <::std::size_t I, ::sprout::index_t ... Is>
			struct op_mul_expand_perm<I, ::sprout::index_tuple<Is...>>{
				using type = ::bcl::tuple<::bcl::tuple<op_mul_expand_pair<I, Is>>...>;
			};

			template <
				typename Tuple, ::std::size_t I, ::std::size_t N,
				typename = void
			>
			class op_mul_expand_aux{
				using left = op_mul_expand_aux<Tuple, I, N / 2 + N % 2>;
				using right = op_mul_expand_aux<Tuple, I + N / 2 + N % 2, N / 2>;

			public:
				using type = ::bcl::tuple_concat_t<typename left::type, typename right::type>;
			};

			template <typename Tuple, ::std::size_t I>
			struct op_mul_expand_aux<
				Tuple, I, 1,
				::std::enable_if_t<
					op_add_trait<::bcl::tuple_element_t<I, Tuple>>::is_add
				>
			>{
				using type = ::bcl::tuple<
					typename op_mul_expand_perm<
						I,
						typename ::sprout::index_range<
							0,
							op_add_trait<::bcl::tuple_element_t<I, Tuple>>::size
						>::type
					>::type
				>;
			};

			template <typename Tuple, ::std::size_t I>
			struct op_mul_expand_aux<
				Tuple, I, 1,
				::std::enable_if_t<
					!op_add_trait<::bcl::tuple_element_t<I, Tuple>>::is_add
				>
			>{
				using type = ::bcl::tuple<::bcl::tuple<::bcl::tuple<op_mul_expand_single<I>>>>;
			};

			template <typename Tuple, typename Mul, ::std::size_t I, ::std::size_t N>
			class op_mul_expand_impl{
				using left = op_mul_expand_impl<Tuple, Mul, I, N / 2 + N % 2>;
				using right = op_mul_expand_impl<Tuple, Mul, I + N / 2 + N % 2, N / 2>;

			public:
				using type = ::bcl::tuple_concat_t<
					typename left::type, typename right::type
				>;
			};

			template <typename Tuple, typename Mul, ::std::size_t I>
			struct op_mul_expand_impl<Tuple, Mul, I, 0>{
				using type = ::bcl::tuple<>;
			};

			template <typename Tuple, typename Mul>
			struct op_mul_expand_impl_aux;

			template <typename ... Ts, typename Mul>
			struct op_mul_expand_impl_aux<::bcl::tuple<Ts...>, Mul>{
				using type = ::tlnc::expressions::op_mul<typename op_mul_expand_dispatch<Ts, Mul>::type...>;
			};

			template <typename Tuple, typename Mul, ::std::size_t I>
			class op_mul_expand_impl<Tuple, Mul, I, 1>{
				using tuple = ::bcl::tuple_element_t<I, Tuple>;

			public:
				using type = ::bcl::tuple<typename op_mul_expand_impl_aux<tuple, Mul>::type>;
			};
		}

		// for op_mul::
		namespace detail{
		}

		template <typename ... Exprs>
		struct op_mul{
			template <typename Arg>
			constexpr auto operator()(Arg &&x) const
			{
				return (... * Exprs{}(x));
			}

			template <typename X>
			constexpr auto derivative() const
			{
				using tuple = typename detail::op_mul_derivative_impl<
					X, ::bcl::tuple<Exprs...>, 0, sizeof...(Exprs)
				>::type;
				using result = ::bcl::tuple_transform_t<op_add, tuple>;
				return detail::reduction<result>();
			}

			constexpr auto reduction()
			{
				return typename detail::op_mul_reduction_from_tuple<
					typename detail::op_mul_reduction_impl<
						::bcl::tuple<decltype(detail::reduction<Exprs>())...>, 0, sizeof...(Exprs)
					>::type
				>::type{};
			}

			constexpr auto expand()
			{
				using index_of_add = typename detail::op_mul_expand_aux<::bcl::tuple<Exprs...>, 0, sizeof...(Exprs)>::type;
				using perm_add = ::bcl::tuple_transform_t<::bcl::tuple_cartesian_prod_variadic_t, index_of_add>;
				using result = typename detail::op_mul_expand_impl<perm_add, op_mul<Exprs...>, 0, ::bcl::tuple_size<perm_add>::value>::type;
				return ::bcl::tuple_transform_t<op_add, result>{};
			}

		private:
			template <::std::size_t I, typename Arg, typename Memo, ::sprout::index_t ... Is>
			constexpr void update_memo_impl(Arg &&arg, Memo &memo, ::sprout::index_tuple<Is...>) const
			{
				::bcl::get<I>(memo).second = (... * ::bcl::get<Is>(memo).second);
			}

		public:
			template <::std::size_t I, typename Arg, typename Memo>
			constexpr void update_memo(Arg &&arg, Memo &memo) const
			{
				update_memo_impl<I>(
					::std::forward<Arg>(arg), memo,
					::sprout::index_tuple<detail::memo_find_t<Exprs, Memo>::value...>::make());
			}

			template <typename Memo, typename Arg>
			using make_memo = detail::make_memo<op_mul<Exprs...>, Memo, Arg>;

			template <typename Memo, typename Arg>
			using make_memo_t = typename make_memo<Memo, Arg>::type;
		};

		template <typename ... Exprs1, typename ... Exprs2>
		struct op_mul<op_mul<Exprs1...>, Exprs2...> : op_mul<Exprs1..., Exprs2...>{
		};

		template <typename ... Exprs>
		struct op_comma{
			template <typename Arg>
			constexpr auto operator()(Arg &&x) const
			{
				using common_t = ::std::common_type_t<decltype(Exprs{}(x))...>;

				::boost::numeric::ublas::vector<common_t> result(sizeof...(Exprs));

				::std::size_t i = 0;

				for(auto &&v : {static_cast<common_t>(Exprs{}(x))...}){
					result[i] = ::std::move(v);
				}

				return result;
			}

			template <typename X>
			constexpr auto derivative() const
			{
				return op_comma<decltype(Exprs{}.derivative())...>{};
			}

			constexpr auto reduction() const
			{
				return op_comma<decltype(detail::reduction<Exprs>())...>{};
			}

		private:
			template <
				::std::size_t I, typename Arg, typename Memo,
				::sprout::index_t ... Is, ::sprout::index_t ... Js
			>
			constexpr void update_memo_impl(
				Arg &&arg, Memo &memo,
				::sprout::index_tuple<Is...>, ::sprout::index_tuple<Js...>
			) const
			{
				using common_t = ::std::common_type_t<decltype(Exprs{}(arg))...>;
				::boost::numeric::ublas::vector<common_t> v(sizeof...(Exprs));
				((v(Js) = ::bcl::get<Is>(memo).second), ...);
				::bcl::get<I>(memo).second = ::std::move(v);
			}

		public:
			template <::std::size_t I, typename Arg, typename Memo>
			constexpr void update_memo(Arg &&arg, Memo &memo) const
			{
				update_memo_impl<I>(
					::std::forward<Arg>(arg), memo,
					::sprout::index_tuple<::bcl::tuple_find_t<Exprs, Memo>::value...>::make(),
					::sprout::index_range<0, sizeof...(Exprs)>::make());
			}

			template <typename Memo, typename Arg>
			using make_memo = detail::make_memo<op_comma<Exprs...>, Memo, Arg>;

			template <typename Memo, typename Arg>
			using make_memo_t = typename make_memo<Memo, Arg>::type;
		};

		template <typename ... Commas>
		struct op_nested_comma{
			static constexpr auto column_size = ::bcl::tuple_size<
				::bcl::tuple_from_t<::bcl::tuple_element_t<0, ::bcl::tuple<Commas...>>>
			>::value;

			static_assert((... && (column_size == ::bcl::tuple_size<::bcl::tuple_from_t<Commas>>::value)));

			template <typename Arg>
			using result_value_type = ::std::common_type_t<
				typename decltype(Commas{}(::std::declval<Arg>()))::value_type...
			>;

			template <typename X>
			constexpr auto derivative() const
			{
				return op_nested_comma<decltype(Commas{}.template derivative<X>())...>{};
			}

		private:
			template <::sprout::index_t I, typename T, typename Arg, ::sprout::index_t ... Is>
			constexpr void operator_call_impl2(T &result, Arg &&arg, ::sprout::index_tuple<Is...>) const
			{
				using func_tuple = ::bcl::tuple_from_t<::bcl::tuple_element_t<I, ::bcl::tuple<Commas...>>>;
				func_tuple funcs;
				(..., (result(I, Is) = ::bcl::get<Is>(funcs)(arg)));
			}

			template <typename T, typename Arg, ::sprout::index_t ... Is>
			constexpr void operator_call_impl1(T &result, Arg &&arg, ::sprout::index_tuple<Is...>) const
			{
				(..., operator_call_impl2<Is>(result, arg, ::sprout::make_index_tuple<column_size>::make()));
			}

		public:
			template <typename Arg>
			constexpr auto operator()(Arg &&arg) const
			{
				::boost::numeric::ublas::matrix<result_value_type<Arg>> result(sizeof...(Commas), column_size);

				operator_call_impl1(
					result, ::std::forward<Arg>(arg),
					::sprout::make_index_tuple<sizeof...(Commas)>::make());

				return result;
			}


		};
	}

	template <typename ... Exprs>
	struct is_expression<expressions::op_add<Exprs...>> : ::std::true_type{
	};

	template <typename ... Exprs>
	struct is_expression<expressions::op_mul<Exprs...>> : ::std::true_type{
	};

	template <typename ... Exprs>
	struct is_expression<expressions::op_comma<Exprs...>> : ::std::true_type{
	};
}

namespace tlnc{
	namespace expressions{
		template <typename ... ExprsL, typename ... ExprsR>
		constexpr auto operator+(op_add<ExprsL...>, op_add<ExprsR...>)
		{
			return op_add<ExprsL..., op_add<ExprsR...>>{};
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
			return op_add<::std::decay_t<T>, op_add<Exprs...>>{};
		}

		template <
			typename T, typename ... Exprs,
			::std::enable_if_t<
				::tlnc::is_value<::std::decay_t<T>>{}
			>* = nullptr
		>
		constexpr auto operator+(T &&, op_add<Exprs...>)
		{
			return op_add<constant<::std::decay_t<T>>, op_add<Exprs...>>{};
		}

		template <
			typename T, typename U,
			::std::enable_if_t<
				::tlnc::is_expression_v<::std::decay_t<T>>
				&& ::tlnc::is_expression_v<::std::decay_t<U>>
				&& (!::tlnc::is_constant<::std::decay_t<T>>{} || !::tlnc::is_constant<::std::decay_t<U>>{})
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
				&& ::tlnc::is_expression_v<::std::decay_t<U>>
				&& (::tlnc::is_constant<::std::decay_t<T>>{} && ::tlnc::is_constant<::std::decay_t<U>>{})
			>* = nullptr
		>
		constexpr auto operator+(T &&, U &&)
		{
			using type = decltype(::std::decay_t<T>::value() + ::std::decay_t<U>::value());
			return constant<::std::decay_t<type>>{};
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
			return op_mul<ExprsL..., op_mul<ExprsR...>>{};
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
			return op_mul<::std::decay_t<T>, op_mul<Exprs...>>{};
		}

		template <
			typename T, typename ... Exprs,
			::std::enable_if_t<
				::tlnc::is_value<::std::decay_t<T>>{}
			>* = nullptr
		>
		constexpr auto operator*(T &&, op_mul<Exprs...>)
		{
			return op_mul<constant<::std::decay_t<T>>, op_mul<Exprs...>>{};
		}

		template <
			typename T, typename U,
			::std::enable_if_t<
				::tlnc::is_expression_v<::std::decay_t<T>>
				&& ::tlnc::is_expression_v<::std::decay_t<T>>
				&& (!::tlnc::is_constant<::std::decay_t<T>>{} || !::tlnc::is_constant<::std::decay_t<U>>{})
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
				&& ::tlnc::is_expression_v<::std::decay_t<T>>
				&& (::tlnc::is_constant<::std::decay_t<T>>{} && ::tlnc::is_constant<::std::decay_t<U>>{})
			>* = nullptr
		>
		constexpr auto operator*(T &&, U &&)
		{
			using type = decltype(::std::decay_t<T>::value() * ::std::decay_t<U>::value());
			return constant<::std::decay_t<type>>{};
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

		template <
			typename ... Exprs,
			typename U,
			::std::enable_if_t<
				::tlnc::is_expression_v<::std::decay_t<U>>
			>* = nullptr
		>
		constexpr auto operator,(op_comma<Exprs...>, U &&)
		{
			return op_comma<Exprs..., ::std::decay_t<U>>{};
		}

		template <
			typename ... Exprs,
			typename U,
			::std::enable_if_t<
				::tlnc::is_value<::std::decay_t<U>>{}
			>* = nullptr
		>
		constexpr auto operator,(op_comma<Exprs...>, U &&)
		{
			return op_comma<Exprs..., constant<::std::decay_t<U>>>{};
		}

		template <
			typename T,
			typename U,
			::std::enable_if_t<
				::tlnc::is_expression_v<::std::decay_t<T>>
				&& ::tlnc::is_expression_v<::std::decay_t<U>>
			>* = nullptr
		>
		constexpr auto operator,(T &&, U &&)
		{
			return op_comma<::std::decay_t<T>, ::std::decay_t<U>>{};
		}

		template <
			typename T,
			typename U,
			::std::enable_if_t<
				::tlnc::is_expression_v<::std::decay_t<T>>
				&& ::tlnc::is_value<::std::decay_t<U>>{}
			>* = nullptr
		>
		constexpr auto operator,(T &&, U &&)
		{
			return op_comma<::std::decay_t<T>, constant<::std::decay_t<U>>>{};
		}

		template <
			typename T,
			typename U,
			::std::enable_if_t<
				::tlnc::is_value<::std::decay_t<T>>{}
				&& ::tlnc::is_expression_v<::std::decay_t<U>>
			>* = nullptr
		>
		constexpr auto operator,(T &&, U &&)
		{
			return op_comma<constant<::std::decay_t<T>>, ::std::decay_t<U>>{};
		}

		template <typename ... Exprs1, typename ... Exprs2>
		constexpr auto operator,(op_comma<Exprs1...>, op_comma<Exprs2...>)
		{
			return op_nested_comma<op_comma<Exprs1...>, op_comma<Exprs2...>>{};
		}

		template <typename ... Exprs1, typename ... Exprs2>
		constexpr auto operator,(op_nested_comma<Exprs1...>, op_comma<Exprs2...>)
		{
			return op_nested_comma<Exprs1..., op_comma<Exprs2...>>{};
		}
	}
}

