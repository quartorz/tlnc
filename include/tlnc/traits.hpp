#pragma once

#include <boost/numeric/ublas/vector.hpp>
#include <boost/numeric/ublas/matrix.hpp>

#include <type_traits>

#include <bcl/double.hpp>

#include <cti/interval.hpp>

namespace tlnc{
	namespace ub = ::boost::numeric::ublas;

	namespace detail{
		template <typename E>
		::std::true_type is_ublas_vector_impl(::boost::numeric::ublas::vector_expression<E> &&);
		::std::false_type is_ublas_vector_impl(...);
	}

	template <typename T>
	struct is_ublas_vector : decltype(detail::is_ublas_vector_impl(::std::declval<T>())){
	};

	namespace detail{
		template <typename E>
		::std::true_type is_ublas_matrix_impl(::boost::numeric::ublas::matrix_expression<E> &&);
		::std::false_type is_ublas_matrix_impl(...);
	}

	template <typename T>
	struct is_ublas_matrix : decltype(detail::is_ublas_matrix_impl(::std::declval<T>())){
	};

	// true if T is a class defined in namespace ::tlnc::expressions
	template <typename T>
	struct is_expression : ::std::false_type{
	};

	template <typename T>
	constexpr auto is_expression_v = is_expression<T>::value;

	namespace detail{
		template <bool, typename T>
		struct value_type_impl{
			using type = T;
		};

// 		template <typename T, typename A>
// 		struct value_type_impl<ub::vector<T, A>>{
// 			using type = T;
// 		};
//
// 		template <typename T, typename L, typename A>
// 		struct value_type_impl<ub::matrix<T, L, A>>{
// 			using type = T;
// 		};

		template <typename T>
		struct value_type_impl<true, T>{
			using type = typename T::value_type;
		};
	}

	template <typename T>
	using value_type = detail::value_type_impl<
		is_ublas_vector<::std::decay_t<T>>{} || is_ublas_matrix<::std::decay_t<T>>{},
		::std::decay_t<T>
	>;

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

	template <typename Inf, typename Sup>
	struct is_value<::cti::interval<Inf, Sup>> : ::std::true_type{
	};

	// true if T is tlnc::expressoins::constant<T>
	template <typename T>
	struct is_constant : ::std::false_type{
	};

	template <typename T>
	struct is_argument : ::std::false_type{
	};

	template <typename T, typename = void>
	struct constant_traits{
		static constexpr bool is_constant = false;
		static constexpr bool is_zero = false;
		static constexpr bool is_one = false;
	};

	template <typename T>
	struct is_vector : ::std::false_type{
	};

	template <typename T>
	struct is_matrix : ::std::false_type{
	};
}

