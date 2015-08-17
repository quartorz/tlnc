#pragma once

#include <boost/preprocessor/facilities/overload.hpp>

#include <utility>

#include <bcl/double.hpp>
#include <bcl/tuple.hpp>
#include <bcl/has_xxx.hpp>

#include <cti/interval.hpp>
#include <cti/rdouble.hpp>

#include <tlnc/traits.hpp>

namespace tlnc{
	namespace expressions{
		template <typename Value>
		struct constant{
			//static constexpr auto value = Value::value;

			using value_type = Value;

			static constexpr auto value()
			{
				return Value{};
			}

			constexpr operator Value() const
			{
				return Value{};
			}

			template <typename Arg>
			constexpr auto operator()(Arg &&) const
			{
				return ::tlnc::value_type_t<Arg>(Value{});
			}

			template <typename X>
			constexpr auto derivative() const
			{
				return constant<decltype(Value{} * BCL_DOUBLE_V(0.0))>{};
			}

			template <::std::size_t I, typename Arg, typename Memo>
			constexpr void update_memo(Arg &&arg, Memo &memo) const
			{
				::bcl::get<I>(memo).second = operator()(arg);
			}

		private:
			template <typename Arg>
			using result_type = ::std::result_of_t<constant<Value>(Arg)>;

			template <bool, typename Memo, typename Pair>
			struct make_memo_impl{
				using type = Memo;
			};

			template <typename Memo, typename Pair>
			struct make_memo_impl<false, Memo, Pair>{
				using type = ::bcl::tuple_concat_t<Memo, ::bcl::tuple<Pair>>;
			};

		public:
			template <typename Memo, typename Arg>
			using make_memo = make_memo_impl<
				::bcl::has_value_v<
					::bcl::tuple_find_t<
						::std::pair<constant<Value>, result_type<Arg>>,
						Memo
					>
				>,
				Memo,
				::std::pair<constant<Value>, result_type<Arg>>
			>;

			template <typename Memo, typename Arg>
			using make_memo_t = typename make_memo<Memo, Arg>::type;
		};
	}

	template <typename Value>
	struct is_constant<expressions::constant<Value>> : ::std::true_type{
	};

	template <typename Value>
	struct is_expression<expressions::constant<Value>> : ::std::true_type{
	};
}

// make double constant
// TLNC_C(x)
#define TLNC_C(x) ::tlnc::expressions::constant<BCL_DOUBLE_T(x)>{}

#define TLNC_I_I_1(x) ::tlnc::expressions::constant<::cti::interval<BCL_DOUBLE_T(x), BCL_DOUBLE_T(x)>>{}
#define TLNC_I_I_2(x, y) ::tlnc::expressions::constant<::cti::interval<BCL_DOUBLE_T(x), BCL_DOUBLE_T(y)>>{}

// make interval constant
// TLNC_I(x) or TLNC_I(inf, sup)
#if defined(_MSC_VER)
# define TLNC_I(...) BOOST_PP_CAT(BOOST_PP_OVERLOAD(TLNC_I_I_,__VA_ARGS__)(__VA_ARGS__),BOOST_PP_EMPTY())
#else
# define TLNC_I(...) BOOST_PP_OVERLOAD(TLNC_I_I_,__VA_ARGS__)(__VA_ARGS__)
#endif

// alias to TLNC_C
#if !defined(C_)
# define C_(x) TLNC_C(x)
#endif

// alias to TLNC_I
#if !defined(I_)
# if defined(_MSC_VER)
#  define I_(...) BOOST_PP_CAT(BOOST_PP_OVERLOAD(TLNC_I_I_,__VA_ARGS__)(__VA_ARGS),BOOST_PP_EMPTY())
# else
#  define I_(...) BOOST_PP_OVERLOAD(TLNC_I_I_,__VA_ARGS__)(__VA_ARGS__)
# endif
#endif

namespace tlnc{
	template <typename T>
	struct constant_traits<
		expressions::constant<T>,
		::std::enable_if_t<::bcl::is_encoded_double_v<T>>
	>{
		static constexpr bool is_constant = true;
		static constexpr bool is_zero = (T::value == 0.0);
		static constexpr bool is_one = (T::value == 1.0);

		using zero_type = decltype(TLNC_C(0.0));
		using one_type = decltype(TLNC_C(1.0));
	};

	template <typename T>
	struct constant_traits<
		expressions::constant<T>,
		::std::enable_if_t<::cti::is_interval_v<T>>
	>{
		static constexpr bool is_constant = true;
		static constexpr bool is_zero =
			(T{}.lower() == static_cast<typename T::value_type>(0.0))
			&& (T{}.upper() == static_cast<typename T::value_type>(0.0));
		static constexpr bool is_one =
			(T{}.lower() == static_cast<typename T::value_type>(1.0))
			&& (T{}.upper() == static_cast<typename T::value_type>(1.0));

		using zero_type = ::cti::interval<BCL_DOUBLE_T(0.0), BCL_DOUBLE_T(0.0)>;
		using one_type = ::cti::interval<BCL_DOUBLE_T(1.0), BCL_DOUBLE_T(1.0)>;
	};
}

