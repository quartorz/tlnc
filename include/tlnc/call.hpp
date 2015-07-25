#pragma once

#include <utility>
#include <type_traits>

#include <sprout/index_tuple.hpp>

#include <bcl/tuple.hpp>
#include <bcl/has_xxx.hpp>

namespace tlnc{
	namespace detail{
		template <typename Arg, typename Memo, ::sprout::index_t ... Is>
		constexpr void memo_update(Arg &&arg, Memo &memo, ::sprout::index_tuple<Is...>)
		{
			((::bcl::get<Is>(memo).second = ::bcl::get<Is>(memo).first(arg)), ...);
		}

		template <typename MemoOld, typename MemoNew, ::sprout::index_t ... Is>
		constexpr void memo_copy_impl(MemoOld &&m1, MemoNew &m2, ::sprout::index_tuple<Is...>)
		{
			((::bcl::get<Is>(m2) = ::bcl::get<Is>(m1)), ...);
		}

		template <
			typename MemoNew, typename MemoOld,
			::std::enable_if_t<
				::std::is_same<MemoOld, MemoNew>{}
			>* = nullptr
		>
		constexpr auto memo_copy(MemoOld &&m1)
		{
			MemoNew m2 = m1;
			return m2;
		}

		template <
			typename MemoNew, typename MemoOld,
			::std::enable_if_t<
				!::std::is_same<MemoOld, MemoNew>{}
			>* = nullptr
		>
		constexpr auto memo_copy(MemoOld &&m1)
		{
			MemoNew m2;
			memo_copy_impl(m1, m2, ::sprout::index_range<0, ::bcl::tuple_size<MemoOld>::value>::make());
			return m2;
		}

		template <typename T, typename Memo, ::std::size_t I, ::std::size_t N, typename = void>
		struct memo_find{
			using left = typename memo_find<T, Memo, I, N / 2 + N % 2>::type;
			using type = ::std::conditional_t<
				::bcl::has_value_v<left>,
				left,
				typename memo_find<T, Memo, I + N / 2 + N % 2, N / 2>::type
			>;
		};

		template <typename T, typename Memo, ::std::size_t I>
		struct memo_find<T, Memo, I, 0, void>{
			using type = memo_find;
		};

		template <typename T, typename Memo, ::std::size_t I>
		struct memo_find<
			T, Memo, I, 1,
			::std::enable_if_t<
				!::std::is_same<T, ::std::tuple_element_t<0, ::bcl::tuple_element_t<I, Memo>>>{}
			>
		>
		{
			using type = memo_find;
		};

		template <typename T, typename Memo, ::std::size_t I>
		struct memo_find<
			T, Memo, I, 1,
			::std::enable_if_t<
				::std::is_same<T, ::std::tuple_element_t<0, ::bcl::tuple_element_t<I, Memo>>>{}
			>
		>
		{
			using type = memo_find;
			static constexpr auto value = I;
		};
	}

	template <typename Func, typename Arg>
	constexpr auto call(Func &&f, Arg &&arg)
	{
		return f(arg);
	}

	template <typename Func, typename Arg, typename Memo>
	constexpr auto call(Func &&f, Arg &&arg, Memo &&memo)
	{
		using func_t = ::std::decay_t<Func>;
		using memo_t = ::std::decay_t<Memo>;
		using memo_new_t = typename func_t::template make_memo_t<memo_t, Arg>;

		constexpr auto memo_old_size = ::bcl::tuple_size<memo_t>::value;
		constexpr auto memo_new_size = ::bcl::tuple_size<memo_new_t>::value;

		memo_new_t memo_new = detail::memo_copy<memo_new_t>(memo);

		detail::memo_update(
			arg,
			memo_new,
			::sprout::index_range<memo_old_size, memo_new_size>::make());

		constexpr auto index = detail::memo_find<func_t, memo_new_t, 0, memo_new_size>::type::value;

		return ::std::make_pair(
			::bcl::get<index>(memo_new).second,
			::std::move(memo_new));
	}

	constexpr ::bcl::tuple<> memo()
	{
		return {};
	}
}

