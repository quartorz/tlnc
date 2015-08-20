#pragma once

#include <cstdint>
#include <type_traits>

#include <bcl/tuple.hpp>

#include <tlnc/traits.hpp>
#include <tlnc/generic.hpp>
#include <tlnc/expressions/detail/make_memo.hpp>
#include <tlnc/expressions/detail/memo_find.hpp>

#define TLNC_SCALAR_FUNCTION(name, ...)\
	namespace tlnc{\
		namespace expressions{\
			template <typename Expr>\
			struct name{\
				template <typename T>\
				constexpr auto operator()(T &&x) const\
				{\
					return ::tlnc::generic::name{}(Expr{}(x));\
				}\
				template <typename X>\
				auto derivative() const\
				{\
					return __VA_ARGS__;\
				}\
				constexpr auto reduction() const\
				{\
					return name<decltype(Expr{}.reduction())>{};\
				}\
				template <::std::size_t I, typename Arg, typename Memo>\
				constexpr void update_memo(Arg &&arg, Memo &memo) const\
				{\
					constexpr ::std::size_t index = detail::memo_find_t<Expr, Memo>::value;\
					::bcl::get<I>(memo).second = ::tlnc::generic::name{}(::bcl::get<index>(memo).second);\
				}\
				template <typename Memo, typename Arg>\
				using make_memo = detail::make_memo<name<Expr>, Memo, Arg>;\
				template <typename Memo, typename Arg>\
				using make_memo_t = typename make_memo<Memo, Arg>::type;\
			};\
		}\
		namespace functions{\
			struct name{\
				template <\
					typename Expr,\
					::std::enable_if_t<\
						::tlnc::is_expression_v<::std::decay_t<Expr>>\
					>* = nullptr\
				>\
				constexpr auto\
				operator()(Expr &&) const\
				{\
					return ::tlnc::expressions::name<::std::decay_t<Expr>>{};\
				}\
				template <\
					typename T,\
					::std::enable_if_t<\
						!::tlnc::is_expression_v<::std::decay_t<T>>\
					>* = nullptr\
				>\
				constexpr auto operator()(T &&x) const\
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

