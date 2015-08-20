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
#include <tlnc/expressions/vector.hpp>
#include <tlnc/expressions/matrix.hpp>
#include <tlnc/expressions/detail/make_memo.hpp>
#include <tlnc/expressions/detail/memo_find.hpp>

namespace tlnc{
	namespace expressions{
		template <typename ... Exprs>
		struct op_add;

		template <typename ... Exprs>
		struct op_mul;

		template <typename Base, typename Exponent>
		struct pow;

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
				using element = decltype(::bcl::tuple_element_t<I, Tuple>{}.reduction());

			public:
				using type = ::std::conditional_t<
					::tlnc::constant_traits<element>::is_zero,
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
					::bcl::tuple<decltype(Exprs{}.reduction())...>,
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
				using element = decltype(::bcl::tuple_element_t<I, Tuple>{}.reduction());

			public:
				static constexpr int value = ::tlnc::constant_traits<element>::is_zero ? 1 : 0;

				using type = ::std::conditional_t<
					value == 1,
					decltype(TLNC_C(0.0)),
					::std::conditional_t<
						::tlnc::constant_traits<element>::is_one,
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

			template <typename T>
			struct op_mul_reduction_is_pow{
				static constexpr bool value = false;
			};

			template <typename Base, typename Exponent>
			struct op_mul_reduction_is_pow<pow<Base, Exponent>>{
				static constexpr bool value = true;
			};

			template <typename T, typename Pows, typename Others, ::std::size_t I, ::std::size_t N>
			class op_mul_reduction_find_pow{
				using left = op_mul_reduction_find_pow<T, Pows, Others, I, N / 2 + N % 2>;
				using right = op_mul_reduction_find_pow<
					T,
					typename left::pows, typename left::others,
					I + N / 2 + N % 2, N / 2
				>;

			public:
				using pows = typename right::pows;
				using others = typename right::others;
			};

			template <typename T, typename Pows, typename Others, ::std::size_t I>
			struct op_mul_reduction_find_pow<T, Pows, Others, I, 0>{
				using pows = Pows;
				using others = Others;
			};

			template <typename T, typename Pows, typename Others, ::std::size_t I>
			class op_mul_reduction_find_pow<T, Pows, Others, I, 1>{
				using element = ::bcl::tuple_element_t<I, T>;
				static constexpr bool is_pow = op_mul_reduction_is_pow<element>::value;

			public:
				using pows = ::bcl::tuple_concat_t<
					Pows,
					::std::conditional_t<
						is_pow,
						::bcl::tuple<element>,
						::bcl::tuple<>
					>
				>;
				using others = ::bcl::tuple_concat_t<
					Others,
					::std::conditional_t<
						is_pow,
						::bcl::tuple<>,
						::bcl::tuple<element>
					>
				>;
			};

			template <typename Base, typename Tuple, ::std::size_t I, ::std::size_t N>
			class op_mul_reduction_extract_pow{
				using left = op_mul_reduction_extract_pow<Base, Tuple, I, N / 2 + N % 2>;
				using right = op_mul_reduction_extract_pow<Base, Tuple, I + N / 2 + N % 2, N / 2>;

			public:
				using type = ::bcl::tuple_concat_t<typename left::type, typename right::type>;
			};

			template <typename Base, typename Tuple, ::std::size_t I>
			struct op_mul_reduction_extract_pow<Base, Tuple, I, 0>{
				using type = ::bcl::value_tuple<::std::size_t>;
			};

			template <typename Base, typename Tuple, ::std::size_t I>
			class op_mul_reduction_extract_pow<Base, Tuple, I, 1>{
				using element = ::bcl::tuple_element_t<I, Tuple>;

			public:
				using type = ::std::conditional_t<
					::std::is_same<typename element::base_type, Base>{},
					::bcl::value_tuple<::std::size_t, I>,
					::bcl::value_tuple<::std::size_t>
				>;
			};

			template <typename T>
			struct op_mul_reduction_pow_3;

			template <typename Pow, typename ... Pows>
			struct op_mul_reduction_pow_3<::bcl::tuple<Pow, Pows...>>{
				using type = pow<
					typename Pow::base_type,
					decltype((typename Pow::exponent_type{} + ... + typename Pows::exponent_type{}).reduction())
				>;
			};

			template <typename T, ::std::size_t I, ::std::size_t N, typename = void>
			class op_mul_reduction_pow_2{
				using left = op_mul_reduction_pow_2<T, I, N / 2 + N % 2>;
				using right = op_mul_reduction_pow_2<typename left::next, I + N / 2 + N % 2, N / 2>;

			public:
				using next = typename right::next;
				using type = ::bcl::tuple_concat_t<typename left::type, typename right::type>;
			};

			template <typename T, ::std::size_t I>
			struct op_mul_reduction_pow_2<T, I, 0, void>{
				using next = T;
				using type = ::bcl::tuple<>;
			};

			template <typename T, ::std::size_t I>
			struct op_mul_reduction_pow_2<
				T, I, 1,
				::std::enable_if_t<(0 == ::bcl::tuple_size<T>::value)>
			>{
				using next = T;
				using type = ::bcl::tuple<>;
			};

			template <typename T, ::std::size_t I>
			class op_mul_reduction_pow_2<
				T, I, 1,
				::std::enable_if_t<(0 != ::bcl::tuple_size<T>::value)>
			>{
				using element = ::bcl::tuple_element_t<0, T>;
				using index = typename op_mul_reduction_extract_pow<
					typename element::base_type, T,
					0, ::bcl::tuple_size<T>::value
				>::type;
				using extracted = ::bcl::tuple_extract_t<index, T>;
				using removed = ::bcl::tuple_remove_t<index, T>;

			public:
				using next = removed;
				using type = ::bcl::tuple<typename op_mul_reduction_pow_3<extracted>::type>;
			};

			template <typename T>
			struct op_mul_reduction_pow_1{
				using type = T;
			};

			template <typename ... Exprs>
			class op_mul_reduction_pow_1<op_mul<Exprs...>>{
				using find_pow = op_mul_reduction_find_pow<
					::bcl::tuple<Exprs...>,
					::bcl::tuple<>,
					::bcl::tuple<>,
					0, sizeof...(Exprs)
				>;
				using pow_tuple = typename find_pow::pows;
				using others = typename find_pow::others;
				using pow_processed = op_mul_reduction_pow_2<
					pow_tuple, 0, ::bcl::tuple_size<pow_tuple>::value
				>;

			public:
				using type = ::bcl::tuple_transform_t<
					::tlnc::expressions::op_mul,
					::bcl::tuple_concat_t<
						typename pow_processed::type, others
					>
				>;
			};
		}

		// for op_mul::expand
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
				return result{}.reduction();
			}

			constexpr auto reduction()
			{
				using type1 = typename detail::op_mul_reduction_from_tuple<
					typename detail::op_mul_reduction_impl<
						::bcl::tuple<decltype(Exprs{}.reduction())...>, 0, sizeof...(Exprs)
					>::type
				>::type;

				using type2 = typename detail::op_mul_reduction_pow_1<type1>::type;

				return type2{};
			}

			constexpr auto expand()
			{
				using index_of_add = typename detail::op_mul_expand_aux<
					::bcl::tuple<Exprs...>, 0, sizeof...(Exprs)
				>::type;
				using perm_add = ::bcl::tuple_transform_t<::bcl::tuple_cartesian_prod_variadic_t, index_of_add>;
				using result = typename detail::op_mul_expand_impl<
					perm_add, op_mul<Exprs...>, 0, ::bcl::tuple_size<perm_add>::value
				>::type;
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
				&& !(::tlnc::is_vector<T>{} && ::tlnc::is_vector<U>{})
				&& !(::tlnc::is_matrix<T>{} && ::tlnc::is_matrix<U>{})
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
		constexpr auto operator+(T &&, U &&)
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
			return op_mul<ExprsL..., op_mul<ExprsR...>>{}.reduction();
		}

		template <
			typename ... Exprs, typename U,
			::std::enable_if_t<
				::tlnc::is_expression_v<::std::decay_t<U>>
			>* = nullptr
		>
		constexpr auto operator*(op_mul<Exprs...>, U &&)
		{
			return op_mul<Exprs..., ::std::decay_t<U>>{}.reduction();
		}

		template <
			typename ... Exprs, typename U,
			::std::enable_if_t<
				::tlnc::is_value<::std::decay_t<U>>{}
			>* = nullptr
		>
		constexpr auto operator*(op_mul<Exprs...>, U &&)
		{
			return op_mul<Exprs..., constant<::std::decay_t<U>>>{}.reduction();
		}

		template <
			typename T, typename ... Exprs,
			::std::enable_if_t<
				::tlnc::is_expression_v<::std::decay_t<T>>
			>* = nullptr
		>
		constexpr auto operator*(T &&, op_mul<Exprs...>)
		{
			return op_mul<::std::decay_t<T>, op_mul<Exprs...>>{}.reduction();
		}

		template <
			typename T, typename ... Exprs,
			::std::enable_if_t<
				::tlnc::is_value<::std::decay_t<T>>{}
			>* = nullptr
		>
		constexpr auto operator*(T &&, op_mul<Exprs...>)
		{
			return op_mul<constant<::std::decay_t<T>>, op_mul<Exprs...>>{}.reduction();
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
			return op_mul<::std::decay_t<T>, ::std::decay_t<U>>{}.reduction();
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
			return constant<::std::decay_t<type>>{}.reduction();
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
			return op_mul<::std::decay_t<T>, constant<::std::decay_t<U>>>{}.reduction();
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
			return op_mul<constant<::std::decay_t<T>>, ::std::decay_t<U>>{}.reduction();
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
		constexpr auto operator,(vector<Exprs...>, U &&)
		{
			return vector<Exprs..., ::std::decay_t<U>>{};
		}

		template <
			typename ... Exprs,
			typename U,
			::std::enable_if_t<
				::tlnc::is_value<::std::decay_t<U>>{}
			>* = nullptr
		>
		constexpr auto operator,(vector<Exprs...>, U &&)
		{
			return vector<Exprs..., constant<::std::decay_t<U>>>{};
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
			return vector<::std::decay_t<T>, ::std::decay_t<U>>{};
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
			return vector<::std::decay_t<T>, constant<::std::decay_t<U>>>{};
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
			return vector<constant<::std::decay_t<T>>, ::std::decay_t<U>>{};
		}

		template <typename ... Exprs1, typename ... Exprs2>
		constexpr auto operator,(vector<Exprs1...>, vector<Exprs2...>)
		{
			return matrix<vector<Exprs1...>, vector<Exprs2...>>{};
		}

		template <typename ... Exprs1, typename ... Exprs2>
		constexpr auto operator,(matrix<Exprs1...>, vector<Exprs2...>)
		{
			return matrix<Exprs1..., vector<Exprs2...>>{};
		}

		template <typename ... Exprs1, typename ... Exprs2>
		constexpr auto operator+(vector<Exprs1...>, vector<Exprs2...>)
		{
			static_assert(sizeof...(Exprs1) == sizeof...(Exprs2));
			return vector<decltype(Exprs1{} + Exprs2{})...>{};
		}

		template <typename ... Vectors1, typename ... Vectors2>
		constexpr auto operator+(matrix<Vectors1...>, matrix<Vectors2...>)
		{
			static_assert(sizeof...(Vectors1) == sizeof...(Vectors2));
			return matrix<decltype(Vectors1{} + Vectors2{})...>{};
		}
	}
}

