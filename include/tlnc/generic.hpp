#pragma once

#include <cmath>
#include <type_traits>

#include <sprout/math/sin.hpp>
#include <sprout/math/cos.hpp>
#include <sprout/math/tan.hpp>
#include <sprout/math/log.hpp>
#include <sprout/math/pow.hpp>

#include <bcl/double.hpp>

#include <tlnc/traits.hpp>

namespace tlnc{
	namespace generic{
		struct sin{
			template <
				typename T,
				::std::enable_if_t<
					!::tlnc::is_ublas_matrix<T>{} && !::tlnc::is_ublas_vector<T>{}
					&& !::bcl::is_encoded_double_v<T>
				>* = nullptr
			>
			constexpr auto operator()(T &&x) const
			{
				using ::std::sin;
				return sin(x);
			}

			template <
				typename T,
				::std::enable_if_t<::bcl::is_encoded_double_v<T>>* = nullptr
			>
			constexpr auto operator()(T &&) const
			{
				constexpr auto result = ::bcl::encode(::sprout::sin(T::value));
				return BCL_DOUBLE(result){};
			}
		};

		struct cos{
			template <
				typename T,
				::std::enable_if_t<
					!::tlnc::is_ublas_matrix<T>{} && !::tlnc::is_ublas_vector<T>{}
					&& !::bcl::is_encoded_double_v<T>
				>* = nullptr
			>
			constexpr auto operator()(T &&x) const
			{
				using ::std::cos;
				return cos(x);
			}

			template <
				typename T,
				::std::enable_if_t<::bcl::is_encoded_double_v<T>>* = nullptr
			>
			constexpr auto operator()(T &&) const
			{
				constexpr auto result = ::sprout::cos(T::value);
				return BCL_DOUBLE(result){};
			}
		};

		struct tan{
			template <
				typename T,
				::std::enable_if_t<
					!::tlnc::is_ublas_matrix<T>{} && !::tlnc::is_ublas_vector<T>{}
					&& !::bcl::is_encoded_double_v<T>
				>* = nullptr
			>
			constexpr auto operator()(T &&x) const
			{
				using ::std::tan;
				return tan(x);
			}

			template <
				typename T,
				::std::enable_if_t<::bcl::is_encoded_double_v<T>>* = nullptr
			>
			constexpr auto operator()(T &&) const
			{
				return ::sprout::tan(T::value);
			}
		};

		struct log{
			template <
				typename T,
				::std::enable_if_t<
					!::tlnc::is_ublas_matrix<T>{} && !::tlnc::is_ublas_vector<T>{}
					&& !::bcl::is_encoded_double_v<T>
				>* = nullptr
			>
			constexpr auto operator()(T &&x) const
			{
				using ::std::log;
				return log(x);
			}

			template <
				typename T,
				::std::enable_if_t<::bcl::is_encoded_double_v<T>>* = nullptr
			>
			constexpr auto operator()(T &&) const
			{
				return ::sprout::log(T::value);
			}
		};

		struct pow{
			template <
				typename T, typename U,
				::std::enable_if_t<
					!::tlnc::is_ublas_matrix<T>{} && !::tlnc::is_ublas_vector<T>{}
					&& !::tlnc::is_ublas_matrix<U>{} && !::tlnc::is_ublas_vector<U>{}
					&& (!::bcl::is_encoded_double_v<T> || !::bcl::is_encoded_double_v<U>)
				>* = nullptr
			>
			constexpr auto operator()(T &&x, U &&y) const
			{
				using ::std::pow;
				return pow(x, y);
			}

			template <
				typename T, typename U,
				::std::enable_if_t<
					::bcl::is_encoded_double_v<T> && ::bcl::is_encoded_double_v<U>
				>* = nullptr
			>
			constexpr auto operator()(T &&, U &&) const
			{
				return ::sprout::pow(T::value, U::value);
			}
		};
	}
}

