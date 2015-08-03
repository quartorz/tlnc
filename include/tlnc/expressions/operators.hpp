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

namespace tlnc{
	namespace expressions{
		template <typename ... Exprs>
		struct op_add;

		template <typename ... Exprs>
		struct op_mul;

		template <typename ... Exprs>
		struct op_comma;

		namespace detail{
			BCL_HAS_FUNCTION(reduction)
			BCL_HAS_FUNCTION(expand)

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
				return op_add<decltype(Exprs{}.template derivative<X>())...>{};
			}

			constexpr auto reduction() const
			{
				return op_add<decltype(detail::reduction<Exprs>())...>{};
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
				using expr = ::bcl::tuple_element_t<I, tuple>;
				using others = ::bcl::tuple_remove_t<::bcl::value_tuple<::std::size_t, I>, tuple>;

			public:
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
				static constexpr ::std::size_t i = I;
				static constexpr ::std::size_t j = N;
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
				using type = ::bcl::tuple<>;
			};
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
				return result{};
			}

		private:
			template <typename ... Ts>
			constexpr auto reduction_impl(::std::true_type)
			{
				return TLNC_C(0.0);
			}

			template <typename ... Ts>
			constexpr auto reduction_impl(::std::false_type)
			{
				return op_mul<Ts...>{};
			}

		public:
			constexpr auto reduction()
			{
				using exprs = ::bcl::tuple<decltype(detail::reduction<Exprs>())...>;
				return reduction_impl<decltype(detail::reduction<Exprs>())...>(
					::std::integral_constant<
						bool,
						sizeof...(Exprs) == 0 || ::bcl::has_value_v<::bcl::tuple_find_t<decltype(TLNC_C(0.0)), exprs>>
					>{});
			}

			constexpr auto expand()
			{
				using index_of_add = typename detail::op_mul_expand_aux<::bcl::tuple<Exprs...>, 0, sizeof...(Exprs)>::type;
				using perm_add = ::bcl::tuple_transform_t<::bcl::tuple_cartesian_prod_variadic_t, index_of_add>;
				return perm_add{};
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

