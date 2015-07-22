#pragma once

#include <initializer_list>
#include <algorithm>

#include <boost/numeric/ublas/vector.hpp>

namespace tlnc{
	namespace detail{
		namespace ub = ::boost::numeric::ublas;

		template <typename Func>
		class simplified{
			Func f;

		public:
			simplified(Func f)
				: f(f)
			{
			}

			template <typename T>
			auto operator()(::std::initializer_list<T> list)
				-> decltype(f(ub::vector<T>()))
			{
				ub::vector<T> v(list.size());

				auto iter = v.begin();

				for(auto &x : list){
					*iter++ = x;
				}

				return f(v);
			}

			template <typename T>
			auto operator()(const T &x)
				-> decltype(f(x))
			{
				return f(x);
			}
		};
	}

	template <typename Func>
	detail::simplified<Func> simplify(Func f)
	{
		return {f};
	}

	template <typename T>
	T f(std::initializer_list<T>)
	{
		
	}
}
