#pragma once

#include <type_traits>

#include <tlnc/traits.hpp>
//#include <tlnc/expressions/constant.hpp>
#include <tlnc/expressions/unary-operators.hpp>

#define NCT_BINARY_OPERATOR(op, name, ...)\
	namespace tlnc{\
		namespace expressions{\
			template <typename T, typename U>\
			struct op_ ## name{\
				const T lhs;\
				const U rhs;\
				op_ ## name(const T &l, const U &r)\
					: lhs(l), rhs(r)\
				{\
				}\
				op_ ## name(T &&l, U &&r)\
					: lhs(::std::move(l)), rhs(::std::move(r))\
				{\
				}\
				op_ ## name<T, U> reduction() const\
				{\
					return *this;\
				}\
				template <typename A>\
				auto operator()(const A &x) const -> decltype(lhs(x) op rhs(x))\
				{\
					return lhs(x) op rhs(x);\
				}\
				template <typename X>\
				auto derivative() const -> decltype(__VA_ARGS__)\
				{\
					return __VA_ARGS__;\
				}\
			};\
			template <typename T, typename U>\
			typename ::std::enable_if<\
				::tlnc::is_expression<T>::value && ::tlnc::is_expression<U>::value,\
				op_ ## name<T, U>\
			>::type\
			operator op(T x, U y)\
			{\
				return {::std::move(x), ::std::move(y)};\
			}\
		}\
		template <typename T, typename U>\
		struct is_expression<expressions::op_ ## name<T, U>> : ::std::true_type{\
		};\
	}

//#define NCT_BINARY_OPERATOR(op, name, deriv, ...) NCT_BINARY_OPERATOR_I(op, name, NCT_EXPAND ## deriv, __VA_ARGS__)

NCT_BINARY_OPERATOR(+, add, (lhs.template derivative<X>() + rhs.template derivative<X>()).reduction())
NCT_BINARY_OPERATOR(-, sub, (lhs.template derivative<X>() - rhs.template derivative<X>()).reduction())
NCT_BINARY_OPERATOR(*, mul, ((lhs.template derivative<X>() * rhs).reduction() + (rhs.template derivative<X>() * lhs).reduction()).reduction())
NCT_BINARY_OPERATOR(/, div, ((lhs.template derivative<X>() / rhs).reduction() - ((lhs * rhs.template derivative<X>()).reduction() / (rhs * rhs))).reduction())

#undef NCT_BINARY_OPERATOR
/*
namespace tlnc{
	namespace expressions{
		template <typename T>
		struct op_add<T, zero> : T{
			op_add(const T &lhs, zero)
				: T(lhs)
			{
			}
			T reduction() const
			{
				return *this;
			}
		};
		template <typename U>
		struct op_add<zero, U> : U{
			op_add(zero, const U &rhs)
				: U(rhs)
			{
			}
			U reduction() const
			{
				return *this;
			}
		};
		template <typename T>
		struct op_sub<T, zero> : T{
			op_sub(const T &lhs, zero)
				: T(lhs)
			{
			}
			T reduction() const
			{
				return *this;
			}
		};
		template <typename U>
		struct op_sub<zero, U> : U{
			op_sub(zero, const U &rhs)
				: U(rhs)
			{
			}
			U reduction() const
			{
				return *this;
			}
		};
		template <typename T>
		struct op_mul<T, one> : T{
			op_mul(const T &lhs, one)
				: T(lhs)
			{
			}
			T reduction() const
			{
				return *this;
			}
		};
		template <typename U>
		struct op_mul<one, U> : U{
			op_mul(one, const U &rhs)
				: U(rhs)
			{
			}
			U reduction() const
			{
				return *this;
			}
		};
		template <typename T>
		struct op_mul<T, zero> : zero{
			op_mul(const T &lhs, zero)
				: zero(0)
			{
			}
			zero reduction() const
			{
				return {};
			}
		};
		template <typename U>
		struct op_mul<zero, U> : zero{
			op_mul(zero, const U &rhs)
				: zero(0)
			{
			}
			zero reduction() const
			{
				return {};
			}
		};
		template <typename T, typename U>
		struct op_mul<op_minus<T>, op_minus<U>> : op_mul<T, U>{
			op_mul(const op_minus<T> &lhs, const op_minus<U> &rhs)
				: op_mul<T, U>(lhs.expr, rhs.expr)
			{
			}
			op_mul<T, U> reduction() const
			{
				return *this;
			}
		};
		template <typename T>
		struct op_div<T, one> : T{
			op_div(const T &lhs, one)
				: T(lhs)
			{
			}
			T reduction() const
			{
				return *this;
			}
		};
	}
}*/

