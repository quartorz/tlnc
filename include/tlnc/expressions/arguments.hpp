#pragma once

#include <type_traits>
#include <cstdint>

#include <bcl/tuple.hpp>

#include <tlnc/traits.hpp>
#include <tlnc/expressions/constant.hpp>
#include <tlnc/expressions/detail/make_memo.hpp>

namespace tlnc{
	namespace expressions{
		template <::std::size_t I>
		struct vector_arg{
			template <typename T>
			constexpr auto operator()(T &&x) const
			{
				return x(I);
			}

			template <
				typename X,
				::std::enable_if_t<
					::std::is_same<X, vector_arg<I>>{}
				>* = nullptr
			>
			constexpr auto derivative() const
			{
				return TLNC_C(1.0);
			}

			template <
				typename X,
				::std::enable_if_t<
					!::std::is_same<X, vector_arg<I>>{}
				>* = nullptr
			>
			constexpr auto derivative() const
			{
				return TLNC_C(0.0);
			}

			template <::std::size_t Idx, typename Arg, typename Memo>
			constexpr void update_memo(Arg &&arg, Memo &memo)
			{
				::bcl::get<Idx>(memo).second = arg(I);
			}

			template <typename Memo, typename Arg>
			using make_memo = typename detail::make_memo<vector_arg<I>, Memo, Arg>;

			template <typename Memo, typename Arg>
			using make_memo_t = typename make_memo<Memo, Arg>::type;
		};

		template <::std::size_t I, ::std::size_t J>
		struct matrix_arg{
			template <typename T>
			constexpr auto operator()(T &&x) const
			{
				return x(I, J);
			}

			template <
				typename X,
				::std::enable_if_t<
					::std::is_same<X, matrix_arg<I, J>>{}
				>* = nullptr
			>
			constexpr auto derivative() const
			{
				return TLNC_C(1.0);
			}

			template <
				typename X,
				::std::enable_if_t<
					!::std::is_same<X, matrix_arg<I, J>>{}
				>* = nullptr
			>
			constexpr auto derivative() const
			{
				return TLNC_C(0.0);
			}

			template <::std::size_t Idx, typename Arg, typename Memo>
			constexpr void update_memo(Arg &&arg, Memo &memo)
			{
				::bcl::get<Idx>(memo).second = arg(I, J);
			}

			template <typename Memo, typename Arg>
			using make_memo = typename detail::make_memo<matrix_arg<I, J>, Memo, Arg>;

			template <typename Memo, typename Arg>
			using make_memo_t = typename make_memo<Memo, Arg>::type;
		};

		struct arg{
			template <typename T>
			constexpr auto operator()(T &&x) const
			{
				return x;
			}

			template <
				typename X,
				::std::enable_if_t<
					::std::is_same<X, arg>{}
				>* = nullptr
			>
			constexpr auto derivative() const
			{
				return TLNC_C(1.0);
			}

			template <
				typename X,
				::std::enable_if_t<
					!::std::is_same<X, arg>{}
				>* = nullptr
			>
			constexpr auto derivative() const
			{
				return TLNC_C(0.0);
			}

			template <::std::size_t I, typename Arg, typename Memo>
			constexpr void update_memo(Arg &&arg, Memo &memo)
			{
				::bcl::get<I>(memo).second = arg;
			}

			template <typename Memo, typename Arg>
			using make_memo = detail::make_memo<arg, Memo, Arg>;

			template <typename Memo, typename Arg>
			using make_memo_t = typename make_memo<Memo, Arg>::type;
		};
	}

	template <::std::size_t I>
	struct is_expression<expressions::vector_arg<I>> : ::std::true_type{
	};

	template <::std::size_t I, ::std::size_t J>
	struct is_expression<expressions::matrix_arg<I, J>> : ::std::true_type{
	};

	template <>
	struct is_expression<expressions::arg> : ::std::true_type{
	};

	template <typename T>
	struct is_argument : ::std::false_type{
	};

	template <::std::size_t I>
	struct is_argument<expressions::vector_arg<I>> : ::std::true_type{
	};

	template <::std::size_t I, ::std::size_t J>
	struct is_argument<expressions::matrix_arg<I, J>> : ::std::true_type{
	};

	template <>
	struct is_argument<expressions::arg> : ::std::true_type{
	};

	constexpr auto x()
	{
		return expressions::arg{};
	}

	template <int I>
	constexpr auto x()
	{
		return expressions::vector_arg<I>{};
	}

	template <int I, int J>
	constexpr auto x()
	{
		return expressions::matrix_arg<I, J>{};
	}
}
