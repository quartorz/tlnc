#pragma once

#include <utility>
#include <type_traits>

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include <sprout/index_tuple.hpp>

#include <bcl/tuple.hpp>

#include <tlnc/traits.hpp>
#include <tlnc/expressions/vector.hpp>
#include <tlnc/expressions/detail/make_memo.hpp>

namespace tlnc{
	namespace expressions{
		template <typename ... Vectors>
		struct matrix{
			static constexpr auto column_size = ::bcl::tuple_size<
				::bcl::tuple_from_t<::bcl::tuple_element_t<0, ::bcl::tuple<Vectors...>>>
			>::value;
			static constexpr auto row_size = sizeof...(Vectors);

			static_assert((... && (column_size == ::bcl::tuple_size<::bcl::tuple_from_t<Vectors>>::value)));

			template <typename Arg>
			using result_value_type = ::std::common_type_t<
				typename decltype(Vectors{}(::std::declval<Arg>()))::value_type...
			>;

			using tuple_type = ::bcl::tuple<Vectors...>;

			template <::std::size_t I>
			using row_vector = ::bcl::tuple_element_t<I, tuple_type>;

			template <::std::size_t I>
			using column_vector = vector<::bcl::tuple_element_t<I, typename Vectors::tuple_type>...>;

			template <typename X>
			constexpr auto derivative() const
			{
				return matrix<decltype(Vectors{}.template derivative<X>())...>{};
			}

		private:
			template <::sprout::index_t I, typename T, typename Arg, ::sprout::index_t ... Is>
			constexpr void operator_call_impl2(T &result, Arg &&arg, ::sprout::index_tuple<Is...>) const
			{
				using func_tuple = ::bcl::tuple_from_t<::bcl::tuple_element_t<I, ::bcl::tuple<Vectors...>>>;
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
				::boost::numeric::ublas::c_matrix<result_value_type<Arg>, row_size, column_size> result;

				operator_call_impl1(
					result, ::std::forward<Arg>(arg),
					::sprout::make_index_tuple<sizeof...(Vectors)>::make());

				return result;
			}

			constexpr auto reduction() const
			{
				return matrix<decltype(Vectors{}.reduction())...>{};
			}

		private:
			template <
				::sprout::index_t I,
				typename T, typename Memo, ::sprout::index_t ... Js
			>
			constexpr void update_memo_impl2(T &result, Memo &memo, ::sprout::index_tuple<Js...>) const
			{
				using vector = ::bcl::tuple_element_t<I, ::bcl::tuple<Vectors...>>;
				const auto &v = ::bcl::get<detail::memo_find_t<vector, Memo>::value>(memo).second;
				(..., (result(I, Js) = static_cast<typename T::value_type>(v(Js))));
			}

			template <::std::size_t I, typename Arg, typename Memo, ::sprout::index_t ... Is>
			constexpr void update_memo_impl1(Arg &&arg, Memo &memo, ::sprout::index_tuple<Is...>) const
			{
				::boost::numeric::ublas::c_matrix<result_value_type<Arg>, row_size, column_size> result;
				(..., update_memo_impl2<Is>(result, memo, ::sprout::make_index_tuple<column_size>::make()));
				::bcl::get<I>(memo).second = ::std::move(result);
			}

		public:
			template <::std::size_t I, typename Arg, typename Memo>
			constexpr void update_memo(Arg &&arg, Memo &memo) const
			{
				update_memo_impl1<I>(arg, memo, ::sprout::make_index_tuple<sizeof...(Vectors)>::make());
			}

			template <typename Memo, typename Arg>
			using make_memo = detail::make_memo<matrix<Vectors...>, Memo, Arg>;

			template <typename Memo, typename Arg>
			using make_memo_t = typename make_memo<Memo, Arg>::type;
		};
	}

	template <typename ... Vectors>
	struct is_expression<expressions::matrix<Vectors...>> : ::std::true_type{
	};

	template <typename ... Vectors>
	struct is_matrix<expressions::matrix<Vectors...>> : ::std::true_type{
	};

	template <typename ... Exprs>
	constexpr auto matrix(Exprs ...)
	{
		return expressions::matrix<expressions::vector<Exprs...>>{};
	}
}

