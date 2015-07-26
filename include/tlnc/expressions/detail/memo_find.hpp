#pragma once

#include <cstdint>
#include <type_traits>

#include <bcl/has_xxx.hpp>
#include <bcl/tuple.hpp>

namespace tlnc::expressions::detail{
	namespace detail{
		template <typename T, typename Memo, ::std::size_t I, ::std::size_t N, typename = void>
		struct memo_find_impl{
			using left = typename memo_find_impl<T, Memo, I, N / 2 + N % 2>::type;
			using type = ::std::conditional_t<
				::bcl::has_value_v<left>,
				left,
				typename memo_find_impl<T, Memo, I + N / 2 + N % 2, N / 2>::type
			>;
		};

		template <typename T, typename Memo, ::std::size_t I>
		struct memo_find_impl<T, Memo, I, 0, void>{
			using type = memo_find_impl;
		};

		template <typename T, typename Memo, ::std::size_t I>
		struct memo_find_impl<
			T, Memo, I, 1,
			::std::enable_if_t<
				!::std::is_same<T, ::std::tuple_element_t<0, ::bcl::tuple_element_t<I, Memo>>>{}
			>
		>
		{
			using type = memo_find_impl;
		};

		template <typename T, typename Memo, ::std::size_t I>
		struct memo_find_impl<
			T, Memo, I, 1,
			::std::enable_if_t<
				::std::is_same<T, ::std::tuple_element_t<0, ::bcl::tuple_element_t<I, Memo>>>{}
			>
		>
		{
			using type = memo_find_impl;
			static constexpr auto value = I;
		};
	}

	template <typename T, typename Memo>
	using memo_find = detail::memo_find_impl<
		T, Memo, 0, ::bcl::tuple_size<Memo>::value
	>;

	template <typename T, typename Memo>
	using memo_find_t = typename memo_find<T, Memo>::type;
}

