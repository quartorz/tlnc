#pragma once

#include <type_traits>

namespace tlnc{
	namespace tmp{
		template <int... I>
		struct index_tuple{
			using type = index_tuple<I...>;
			static type make()
			{
				return {};
			}
		};

		template <typename L, typename R>
		struct concat_index_impl;

		template <int... I, int... J>
		struct concat_index_impl<index_tuple<I...>, index_tuple<J...>>{
			using type = index_tuple<I..., J...>;
		};

		template <typename L, typename R>
		using concat_index = typename concat_index_impl<L, R>::type;


		template <int Begin, int N>
		struct make_index_range_impl
			: concat_index<
				typename make_index_range_impl<Begin, N / 2 + N % 2>::type,
				typename make_index_range_impl<Begin + N / 2 + N % 2, N / 2>::type
			>
		{
		};

		template <int Begin>
		struct make_index_range_impl<Begin, 0> : index_tuple<>{
		};

		template <int Begin>
		struct make_index_range_impl<Begin, 1> : index_tuple<Begin>{
		};

		template <int Begin, int End>
		using make_index_range = typename make_index_range_impl<Begin, End - Begin>::type;

		template <int End>
		using make_index = typename make_index_range_impl<0, End>::type;
	}
}
