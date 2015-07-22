#pragma once

#include <type_traits>

#include <tlnc/traits.hpp>
#include <tlnc/generic.hpp>

#define TLNC_SCALAR_FUNCTION(name, ...)\
	namespace tlnc{\
		namespace expressions{\
			template <typename Expr>\
			struct name{\
				Expr expr;\
				constexpr name(Expr e) : expr(e){}\
				template <typename T>\
				constexpr auto operator()(const T & x) const\
				{\
					return ::tlnc::generic::name{}(x);\
				}\
				template <typename X>\
				auto derivative() const\
				{\
					return __VA_ARGS__;\
				}\
			};\
		}\
		namespace functions{\
			struct name{\
				template <typename Expr>\
				constexpr ::std::enable_if_t<\
					::tlnc::is_expression_v<Expr>,\
					::tlnc::expressions::sin<Expr>\
				>\
				operator()(Expr expr) const\
				{\
					return {expr};\
				}\
				template <\
					typename T,\
					typename = ::std::enable_if_t<!::tlnc::is_expression_v<T>>\
				>\
				constexpr auto operator()(const T & x) const\
				{\
					return ::tlnc::generic::name{}(x);\
				}\
			};\
		}\
		template <typename Expr>\
		struct is_expression<expressions::name<Expr>> : ::std::true_type{\
		};\
		constexpr functions::name name{};\
	}

namespace tlnc{
	namespace expressions{
		template <typename Expr>
		struct cos;
	}
}

// namespace tlnc{
// 	namespace expressions{
// 		template <typename Expr>
// 		struct sin{
// 			Expr expr;
// 			constexpr sin(Expr e) : expr(e){}
// 			constexpr auto f()
// 			{
// 				return cos<Expr>();
// 			}
// 		};
// 	}
//
// 	namespace functions{
// 		struct sin{
// 			template <typename Expr>
// 			constexpr ::std::enable_if_t<
// 				::tlnc::is_expression_v<Expr>,
// 				::tlnc::expressions::sin<Expr>
// 			>
// 			operator()(Expr expr) const
// 			{
// 				return {expr};
// 			}
// 			template <
// 				typename T,
// 				typename = ::std::enable_if_t<!::tlnc::is_expression_v<T>>
// 			>
// 			constexpr auto operator()(const T & x) const
// 			{
// 				return ::tlnc::generic::sin(x);
// 			}
// 		};
// 	}
//
// 	template <typename Expr>
// 	struct is_expression<expressions::sin<Expr>> : ::std::true_type{
// 	};
//
// 	constexpr functions::sin sin{};
// }

TLNC_SCALAR_FUNCTION(sin, (cos<Expr>(expr) * expr.template derivative<X>()).reduction())
TLNC_SCALAR_FUNCTION(cos, ((-sin<Expr>(expr)).reduction() * expr.template derivative<X>()).reduction())
TLNC_SCALAR_FUNCTION(tan, 1)

