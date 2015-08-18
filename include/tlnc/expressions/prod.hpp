#pragma once

#include <bcl/tmp.hpp>

#include <tlnc/expressions/vector.hpp>
#include <tlnc/expressions/matrix.hpp>
#include <tlnc/expressions/operators.hpp>
#include <tlnc/expressions/element_prod.hpp>

namespace tlnc{
	template <typename ... Vectors, typename ... Exprs>
	constexpr auto prod(
		expressions::matrix<Vectors...>,
		expressions::vector<Exprs...>
	)
	{
		static_assert((... && (sizeof...(Exprs) == Vectors::row_size)));

		using vector_type = expressions::vector<Exprs...>;

		return expressions::vector<::bcl::replace_t<
			decltype(element_prod(Vectors{}, vector_type{})),
			expressions::op_add
		>...>{};
	}
}

