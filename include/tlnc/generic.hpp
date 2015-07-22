#pragma once

#include <cmath>
#include <type_traits>

#include <tlnc/traits.hpp>

namespace tlnc{
	namespace generic{
		struct sin{
			template <
				typename T,
				typename = ::std::enable_if_t<
					!::tlnc::is_ublas_matrix<T>() && !::tlnc::is_ublas_vector<T>()>
			>
			constexpr auto operator()(const T &x) const
			{
				using ::std::sin;
				return sin(x);
			}
		};

		struct cos{
			template <
				typename T,
				typename = ::std::enable_if_t<
					!::tlnc::is_ublas_matrix<T>() && !::tlnc::is_ublas_vector<T>()>
			>
			constexpr auto operator()(const T &x) const
			{
				using ::std::cos;
				return cos(x);
			}
		};

		struct tan{
			template <
				typename T,
				typename = ::std::enable_if_t<
					!::tlnc::is_ublas_matrix<T>() && !::tlnc::is_ublas_vector<T>()>
			>
			constexpr auto operator()(const T &x) const
			{
				using ::std::tan;
				return tan(x);
			}
		};
	}
}

