#pragma once

#include <utility>
#include <type_traits>

#include <boost/numeric/ublas/vector.hpp>

#include <sprout/index_tuple.hpp>

#include <bcl/tuple.hpp>

#include <tlnc/traits.hpp>
#include <tlnc/expressions/detail/make_memo.hpp>
#include <tlnc/expressions/detail/memo_find.hpp>

namespace tlnc{
	namespace expressions{
		template <typename ... Exprs>
		struct vector{
			static constexpr auto row_size = sizeof...(Exprs);

			using tuple_type = ::bcl::tuple<Exprs...>;

			template <typename Arg>
			constexpr auto operator()(Arg &&x) const
			{
				using common_t = ::std::common_type_t<decltype(Exprs{}(x))...>;

				::boost::numeric::ublas::bounded_vector<common_t, row_size> result(row_size);

				::std::size_t i = 0;

				for(auto &&v : {static_cast<common_t>(Exprs{}(x))...}){
					result[i] = ::std::move(v);
				}

				return result;
			}

			template <typename X>
			constexpr auto derivative() const
			{
				return vector<decltype(Exprs{}.derivative())...>{};
			}

			constexpr auto reduction() const
			{
				return vector<decltype(Exprs{}.reduction())...>{};
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
				::boost::numeric::ublas::bounded_vector<common_t, row_size> v(row_size);
				(..., (v(Js) = ::bcl::get<Is>(memo).second));
				::bcl::get<I>(memo).second = ::std::move(v);
			}

		public:
			template <::std::size_t I, typename Arg, typename Memo>
			constexpr void update_memo(Arg &&arg, Memo &memo) const
			{
				update_memo_impl<I>(
					::std::forward<Arg>(arg), memo,
					::sprout::index_tuple<detail::memo_find_t<Exprs, Memo>::value...>::make(),
					::sprout::make_index_tuple<sizeof...(Exprs)>::make());
			}

			template <typename Memo, typename Arg>
			using make_memo = detail::make_memo<vector<Exprs...>, Memo, Arg>;

			template <typename Memo, typename Arg>
			using make_memo_t = typename make_memo<Memo, Arg>::type;
		};
	}

	template <typename ... Exprs>
	struct is_expression<expressions::vector<Exprs...>> : ::std::true_type{
	};

	template <typename ... Exprs>
	struct is_vector<expressions::vector<Exprs...>> : ::std::true_type{
	};
}

