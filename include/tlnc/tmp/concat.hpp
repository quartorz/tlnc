#pragma once

#include <tuple>

namespace tlnc{
	namespace tmp{
		template <typename L, typename R>
		struct concat;

		template <template <typename...> class Tuple, typename... L, typename... R>
		struct concat<Tuple<L...>, Tuple<R...>>{
			using type = Tuple<L..., R...>;
		};
	}
}
