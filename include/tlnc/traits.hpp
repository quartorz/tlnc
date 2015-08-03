#pragma once

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include <type_traits>

#include <bcl/double.hpp>

namespace tlnc{
	namespace ub = ::boost::numeric::ublas;

	template <typename T>
	struct is_ublas_vector : ::std::false_type{
	};

	template <typename T, typename A>
	struct is_ublas_vector<ub::vector<T, A>> : ::std::true_type{
	};

	template <typename T>
	struct is_ublas_matrix : ::std::false_type{
	};

	template <typename T, typename L, typename A>
	struct is_ublas_matrix<ub::matrix<T, L, A>> : ::std::true_type{
	};

	// true if T is a class defined in namespace ::tlnc::expressions
	template <typename T>
	struct is_expression : ::std::false_type{
	};

	template <typename T>
	constexpr auto is_expression_v = is_expression<T>::value;

	namespace detail{
		template <typename T>
		struct value_type_impl{
			using type = T;
		};

		template <typename T, typename A>
		struct value_type_impl<ub::vector<T, A>>{
			using type = T;
		};

		template <typename T, typename L, typename A>
		struct value_type_impl<ub::matrix<T, L, A>>{
			using type = T;
		};
	}

	template <typename T>
	using value_type = detail::value_type_impl<::std::decay_t<T>>;

	template <typename T>
	using value_type_t = typename value_type<T>::type;

	namespace detail{
		template <typename T, typename ... U>
		auto call_test(int) -> decltype(::std::declval<T>()(::std::declval<U>()...), void(0), ::std::true_type{});

		template <typename T, typename ... U>
		::std::false_type call_test(long);
	}

	template <typename T, typename ... U>
	struct is_callable : decltype(detail::call_test<T, U...>(0)){
	};

	// true if T represents a constexpr value,
	// and T and the value is mutually convertible,
	// and the value can do arithmetics.
	template <typename T>
	struct is_value : ::bcl::is_encoded_double<T>{
	};

	// true if T is tlnc::expressoins::constant<T>
	template <typename T>
	struct is_constant : ::std::false_type{
	};
}

