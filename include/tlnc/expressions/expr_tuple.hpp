#pragma once

#include <bcl/tuple.hpp>

namespace tlnc{
	namespace expressions{
		template <typename ... Exprs>
		struct expr_tuple : ::bcl::tuple<Exprs...>{

		};
	}
}

