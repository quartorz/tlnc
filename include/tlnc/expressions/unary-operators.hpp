#pragma once

#include <utility>

#include <tlnc/traits.hpp>

namespace tlnc{
	namespace expressions{
		template <typename Expr>
		struct op_minus{
			const Expr expr;

			op_minus(const Expr &e)
				: expr(e)
			{
			}

			op_minus(Expr &&e)
				: expr(::std::move(e))
			{
			}

			template <typename T>
			auto operator()(const T &x) const -> decltype(-expr(x))
			{
				return -expr(x);
			}

			template <typename X>
			auto derivative() const -> decltype(-expr.template derivative<X>())
			{
				return -expr.template derivative<X>();
			}

			const op_minus<Expr> &reduction() const
			{
				return *this;
			}
		};

		template <typename Expr>
		struct op_minus<op_minus<Expr>> : Expr{
			op_minus(Expr &&e)
				: Expr(::std::move(e))
			{
			}

			op_minus(const Expr &e)
				: Expr(e)
			{
			}

			const Expr &reduction() const
			{
				return *this;
			}
		};

		template <typename Expr>
		op_minus<Expr> operator-(const Expr &expr)
		{
			return {expr};
		}
	}

	template <typename Expr>
	struct is_expression<expressions::op_minus<Expr>> : ::std::true_type{
	};
}
