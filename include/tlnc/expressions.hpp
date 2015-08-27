#pragma once

#include <tlnc/expressions/sin.hpp>
#include <tlnc/expressions/cos.hpp>
#include <tlnc/expressions/tan.hpp>
#include <tlnc/expressions/pow.hpp>
#include <tlnc/expressions/log.hpp>
#include <tlnc/expressions/operators.hpp>
#include <tlnc/expressions/arguments.hpp>
#include <tlnc/expressions/placeholders.hpp>
#include <tlnc/expressions/vector.hpp>
#include <tlnc/expressions/matrix.hpp>
#include <tlnc/expressions/element_prod.hpp>
#include <tlnc/expressions/prod.hpp>
#include <tlnc/expressions/at.hpp>

#include <type_traits>

#include <bcl/tuple.hpp>

namespace tlnc{
	namespace detail{
		template <typename Func>
		struct function_wrapper{
			template <typename Arg>
			constexpr auto operator()(Arg &&arg) const
			{
				return Func{}(::bcl::tuple<const ::std::decay_t<Arg>&>(arg));
			}
		};
	}

	template <typename Func>
	constexpr auto wrap(Func &&)
	{
		return detail::function_wrapper<::std::decay_t<Func>>{};
	}
}

