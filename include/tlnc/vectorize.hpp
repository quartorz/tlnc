#pragma once

#include <boost/numeric/ublas/vector.hpp>
#include <boost/iterator/zip_iterator.hpp>

#include <type_traits>
#include <utility>

#include <tlnc/traits.hpp>

namespace tlnc{
	namespace detail{
		namespace ub = ::boost::numeric::ublas;

		template <typename Func>
		class vectorize_wrapper{
			Func f;

			template <typename, typename Arg>
			struct result_type_impl{
				using type = typename ::std::result_of<Func(Arg)>::type;
			};

			template <typename Arg>
			struct result_type_impl<::std::false_type, Arg>{
			};

			template <typename Arg>
			using result_type = typename result_type_impl<typename ::tlnc::is_callable<Func, Arg>::type, Arg>::type;

		public:
			vectorize_wrapper(Func f) : f(f)
			{
			}

			template <typename T, typename A>
			ub::vector<result_type<T>> operator()(const ub::vector<T, A> &arg) const
			{
				ub::vector<result_type<T>> result(arg.size());

				auto i = ::boost::make_zip_iterator(::boost::make_tuple(arg.begin(), result.begin()));
				auto end = ::boost::make_zip_iterator(::boost::make_tuple(arg.end(), result.end()));

				for(; i != end; ++i){
					i->template get<1>() = f(i->template get<0>());
				}

				return result;
			}
 
			template <typename T, typename L, typename A>
			ub::matrix<result_type<T>> operator()(const ub::matrix<T, L, A> &arg) const
			{
				using size_type = typename ub::matrix<T, L, A>::size_type;

				ub::matrix<result_type<T>> result(arg.size1(), arg.size2());

				for(size_type i = 0; i < arg.size1(); ++i){
					for(size_type j = 0; j < arg.size2(); ++j){
						result(i, j) = f(arg(i, j));
					}
				}

				return result;
			}

			template <typename T>
			typename ::std::enable_if<
				!::tlnc::is_ublas_vector<T>::value && !::tlnc::is_ublas_matrix<T>::value,
				result_type<T>
			>::type
			operator()(const T &arg) const
			{
				return f(arg);
			}
		};
	}

	template <typename Func>
	detail::vectorize_wrapper<Func> vectorize(Func f)
	{
		return detail::vectorize_wrapper<Func>(f);
	}
}
