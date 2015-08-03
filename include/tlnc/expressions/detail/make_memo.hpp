#pragma once

#include <cstdint>
#include <type_traits>
#include <utility>

#include <bcl/tuple.hpp>
#include <bcl/has_xxx.hpp>

namespace tlnc::expressions::detail{
	template <typename Func, typename Memo, typename Arg>
	struct make_memo{
	private:
		using result_type = ::std::result_of_t<Func(Arg)>;

		template <bool, typename Tuple, typename Pair>
		struct make_memo_impl;

		template <typename ... Types, typename Pair>
		struct make_memo_impl<true, ::bcl::tuple<Types...>, Pair>{
			using type = ::bcl::tuple<Types...>;
		};

		template <typename ... Types, typename Pair>
		struct make_memo_impl<false, ::bcl::tuple<Types...>, Pair>{
			using type = ::bcl::tuple<Types..., Pair>;
		};

	public:
		using type = typename make_memo_impl<
			::bcl::has_value_v<
				::bcl::tuple_find_t<
					::std::pair<Func, result_type>,
					Memo
				>
			>,
			Memo,
			::std::pair<Func, result_type>
		>::type;
	};

	namespace detail{
		template <
			typename Memo, typename Arg, typename Tuple,
			::std::size_t I, ::std::size_t N
		>
		struct memoize_make_memo_impl{
			using left = memoize_make_memo_impl<
				Memo, Arg, Tuple, I, N / 2 + N % 2>;
			using right = memoize_make_memo_impl<
				typename left::type, Arg, Tuple, I + N / 2 + N % 2, N / 2>;

			using type = typename right::type;
		};

		template <typename Memo, typename Arg, typename Tuple, ::std::size_t I>
		struct memoize_make_memo_impl<Memo, Arg, Tuple, I, 0>{
			using type = Memo;
		};

		template <typename Memo, typename Arg, typename Tuple, ::std::size_t I>
		struct memoize_make_memo_impl<Memo, Arg, Tuple, I, 1>
		{
			using element = ::bcl::tuple_element_t<I, Tuple>;
			using type = typename element::template make_memo_t<Memo, Arg>;
		};
	}

	template <
		template <typename ...> typename Func, typename ... Args,
		typename Memo, typename Arg
	>
	struct make_memo<Func<Args...>, Memo, Arg>{
	private:
		using func = Func<Args...>;
		using func_pair = ::std::pair<func, ::std::result_of_t<func(Arg)>>;

		struct make_memo_impl{
			using arg_memo = typename detail::memoize_make_memo_impl<
				Memo, Arg, ::bcl::tuple<Args...>, 0, sizeof...(Args)
			>::type;
			using type = ::bcl::tuple_concat_t<
				arg_memo,
				::bcl::tuple<func_pair>
			>;
		};

		struct dummy{
			using type = Memo;
		};

	public:
		using type = typename ::std::conditional_t<
			::bcl::has_value_v<::bcl::tuple_find_t<func_pair, Memo>>,
			dummy,
			make_memo_impl
		>::type;
	};
}

