#pragma once

#include <type_traits>
#include <cstdint>

#include <tlnc/traits.hpp>

namespace tlnc{
	namespace expressions{
		template <::std::size_t I>
		struct vector_arg{
			template <typename T>
			typename T::value_type operator()(const T &x) const
			{
				return x(I);
			}

			template <typename X>
			typename ::std::enable_if<
				::std::is_same<X, vector_arg<I>>::value,
				one
			>::type
			derivative() const
			{
				return {};
			}

			template <typename X>
			typename ::std::enable_if<
				!::std::is_same<X, vector_arg<I>>::value,
				zero
			>::type
			derivative() const
			{
				return {};
			}
		};

		template <::std::size_t I, ::std::size_t J>
		struct matrix_arg{
			template <typename T>
			typename T::value_type operator()(const T &x) const
			{
				return x(I, J);
			}

			template <typename X>
			typename ::std::enable_if<
				::std::is_same<X, matrix_arg<I, J>>::value,
				one
			>::type
			derivative() const
			{
				return {};
			}

			template <typename X>
			typename ::std::enable_if<
				!::std::is_same<X, matrix_arg<I, J>>::value,
				zero
			>::type
			derivative() const
			{
				return {};
			}
		};

		struct arg{
			template <::std::size_t I>
			vector_arg<I> at() const
			{
				return {};
			}

			template <::std::size_t I, ::std::size_t J>
			matrix_arg<I, J> at() const
			{
				return {};
			}

			template <typename T>
			T operator()(const T &x) const
			{
				return x;
			}

			template <typename X>
			typename ::std::enable_if<
				::std::is_same<X, arg>::value,
				one
			>::type
			derivative() const
			{
				return {};
			}

			template <typename X>
			typename ::std::enable_if<
				!::std::is_same<X, arg>::value,
				zero
			>::type
			derivative() const
			{
				return {};
			}
		};
	}

	template <::std::size_t I>
	struct is_expression<expressions::vector_arg<I>> : ::std::true_type{
	};

	template <::std::size_t I, ::std::size_t J>
	struct is_expression<expressions::matrix_arg<I, J>> : ::std::true_type{
	};

	template <>
	struct is_expression<expressions::arg> : ::std::true_type{
	};

	template <typename T>
	struct is_argument : ::std::false_type{
	};

	template <::std::size_t I>
	struct is_argument<expressions::vector_arg<I>> : ::std::true_type{
	};

	template <::std::size_t I, ::std::size_t J>
	struct is_argument<expressions::matrix_arg<I, J>> : ::std::true_type{
	};

	template <>
	struct is_argument<expressions::arg> : ::std::true_type{
	};

	expressions::arg x()
	{
		return{};
	}

	template <int I>
	expressions::vector_arg<I> x()
	{
		return{};
	}

	template <int I, int J>
	expressions::matrix_arg<I, J> x()
	{
		return{};
	}
}
