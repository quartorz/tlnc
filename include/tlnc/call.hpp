#pragma once

#include <utility>
#include <type_traits>

#include <sprout/index_tuple.hpp>

#include <bcl/tuple.hpp>
#include <bcl/has_xxx.hpp>

#include <tlnc/expressions/detail/memo_find.hpp>
#include <tlnc/make_memo.hpp>

namespace tlnc{
	namespace detail{
		template <typename Arg, typename Memo, ::sprout::index_t ... Is>
		constexpr void update_memo(Arg &&arg, Memo &memo, ::sprout::index_tuple<Is...>)
		{
			((::bcl::get<Is>(memo).first.template update_memo<Is>(arg, memo)), ...);
		}

		template <
			typename MemoOld, typename MemoNew, ::sprout::index_t ... Is,
			::std::enable_if_t<
				::std::is_rvalue_reference<MemoOld>{}
			>* = nullptr
		>
		constexpr void copy_memo_impl(MemoOld &&m1, MemoNew &m2, ::sprout::index_tuple<Is...>)
		{
			((::bcl::get<Is>(m2) = ::std::move(::bcl::get<Is>(m1))), ...);
		}

		template <
			typename MemoOld, typename MemoNew, ::sprout::index_t ... Is,
			::std::enable_if_t<
				!::std::is_rvalue_reference<MemoOld>{}
			>* = nullptr
		>
		constexpr void copy_memo_impl(MemoOld &&m1, MemoNew &m2, ::sprout::index_tuple<Is...>)
		{
			((::bcl::get<Is>(m2) = ::bcl::get<Is>(m1)), ...);
		}

		template <
			typename MemoNew, typename MemoOld,
			::std::enable_if_t<
				::std::is_same<::std::decay_t<MemoOld>, ::std::decay_t<MemoNew>>{}
			>* = nullptr
		>
		constexpr auto copy_memo(MemoOld &&m1)
		{
			return MemoNew(m1);
		}

		template <
			typename MemoNew, typename MemoOld,
			::std::enable_if_t<
				!::std::is_same<::std::decay_t<MemoOld>, ::std::decay_t<MemoNew>>{}
			>* = nullptr
		>
		constexpr auto copy_memo(MemoOld &&m1)
		{
			MemoNew m2;
			copy_memo_impl(m1, m2, ::sprout::index_range<0, ::bcl::tuple_size<MemoOld>::value>::make());
			return m2;
		}

		template <typename T>
		struct is_tuple : ::std::false_type{
		};

		template <typename ... Ts>
		struct is_tuple<::bcl::tuple<Ts...>> : ::std::true_type{
		};
	}

	// normal invocation
	template <
		typename Func, typename Arg,
		::std::enable_if_t<detail::is_tuple<::std::decay_t<Arg>>{}>* = nullptr
	>
	constexpr auto call(Func &&f, Arg &&arg)
	{
		return f(arg);
	}

	template <
		typename Func, typename Arg,
		::std::enable_if_t<!detail::is_tuple<::std::decay_t<Arg>>{}>* = nullptr
	>
	constexpr auto call(Func &&f, Arg &&arg)
	{
		return call(
			::std::forward<Func>(f),
			::bcl::tuple<::std::add_const_t<Arg>&>(::std::forward<Arg>(arg)));
	}

	// invocation with memoization and partial update of memo
	template <
		typename Func, typename Arg, typename Memo,
		::std::enable_if_t<detail::is_tuple<::std::decay_t<Arg>>{}>* = nullptr
	>
	constexpr auto call(Func &&f, Arg &&arg, Memo &&memo)
	{
		using func_t = ::std::decay_t<Func>;
		using memo_t = ::std::decay_t<Memo>;
		using memo_new_t = decltype(make_memo(f, arg, memo));

		constexpr auto memo_old_size = ::bcl::tuple_size<memo_t>::value;
		constexpr auto memo_new_size = ::bcl::tuple_size<memo_new_t>::value;

		memo_new_t memo_new = detail::copy_memo<memo_new_t>(memo);

		detail::update_memo(
			arg,
			memo_new,
			::sprout::index_range<memo_old_size, memo_new_size>::make());

		constexpr auto index = expressions::detail::memo_find<
			func_t, memo_new_t
		>::type::value;

		return ::std::make_pair(
			::bcl::get<index>(memo_new).second,
			::std::move(memo_new));
	}

	template <
		typename Func, typename Arg, typename Memo,
		::std::enable_if_t<!detail::is_tuple<::std::decay_t<Arg>>{}>* = nullptr
	>
	constexpr auto call(Func &&f, Arg &&arg, Memo &&memo)
	{
		return call(
			::std::forward<Func>(f),
			::bcl::tuple<::std::add_const_t<Arg>&>(::std::forward<Arg>(arg)),
			::std::forward<Memo>(memo));
	}

	constexpr ::bcl::tuple<> memo()
	{
		return {};
	}
}

