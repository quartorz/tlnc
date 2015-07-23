#pragma once

#include <boost/preprocessor/facilities/overload.hpp>

#include <bcl/double.hpp>

#include <cti/interval.hpp>

#include <tlnc/traits.hpp>

namespace tlnc{
	namespace expressions{
		template <typename Value>
		struct constant{
			//static constexpr auto value = Value::value;

			constexpr operator Value() const
			{
				return Value{};
			}

			template <typename Arg>
			constexpr auto operator()(const Arg &) const
			{
				return ::tlnc::value_type_t<Arg>(Value{});
			}

			template <typename X>
			constexpr auto derivative() const
			{
				return constant<decltype(Value{} * BCL_DOUBLE_V(0.0))>{};
			}
		};
	}
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
// t@github.com:quartorz/bcl.gitC_
#if !defined(C_)
# define C_(x) TLNC_C(x)
#endif

// alias to TLNC_I
// I_(x) or I_(inf, sup)
#if !defined(I_) && !defined(I_I_1) && !defined(I_I_2)
# define I_I_1(x) ::tlnc::expressions::constant<::cti::interval<BCL_DOUBLE_T(x), BCL_DOUBLE_T(x)>>{}
# define I_I_2(x, y) ::tlnc::expressions::constant<::cti::interval<BCL_DOUBLE_T(x), BCL_DOUBLE_T(y)>>{}
# if defined(_MSC_VER)
#  define I_(...) BOOST_PP_CAT(BOOST_PP_OVERLOAD(TLNC_I_I_,__VA_ARGS__)(__VA_ARGS),BOOST_PP_EMPTY())
# else
#  define I_(...) BOOST_PP_OVERLOAD(I_I_,__VA_ARGS__)(__VA_ARGS__)
# endif
#endif

