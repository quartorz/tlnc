#pragma once

#include <type_traits>

#include <tlnc/traits.hpp>
#include <tlnc/generic.hpp>

namespace tlnc{
	namespace expressions{
		template <typename Expr>
		struct log;

		template <typename Base, typename Exponent>
		struct pow{
			template <typename Arg>
			constexpr auto operator()(Arg &&x) const
			{
				return ::tlnc::generic::pow{}(Base{}(x), Exponent{}(x));
			}

			template <typename X>
			constexpr auto derivative() const
			{
				return (
					(Exponent{} * log<Base>{}).template derivative<X>()
					* pow<Base, Exponent>{}
				).reduction();
			}
		};
	}

	namespace functions{
		struct pow{
			template <
				typename T, typename U,
				::std::enable_if_t<
					::tlnc::is_expression_v<::std::decay_t<T>>
					&& ::tlnc::is_expression_v<::std::decay_t<U>>
				>* = nullptr
			>
			constexpr auto operator()(T &&, U &&) const
			{
				return ::tlnc::expressions::pow<::std::decay_t<T>, ::std::decay_t<U>>{};
			}

			template <
				typename T, typename U,
				::std::enable_if_t<
					!::tlnc::is_expression_v<::std::decay_t<T>>
					&& !::tlnc::is_expression_v<::std::decay_t<U>>
				>* = nullptr
			>
			constexpr auto operator()(T &&x, U &&y) const
			{
				return ::tlnc::generic::pow{}(x, y);
			}
		};
	}

	template <typename Base, typename Exponent>
	struct is_expression<expressions::pow<Base, Exponent>> : ::std::true_type{
	};

	constexpr functions::pow pow{};
}

