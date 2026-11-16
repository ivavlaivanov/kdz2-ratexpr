#include "ratexpr/expression.hpp"
#include "ratexpr/fraction.hpp"
#include <memory>

namespace ratexpr {
namespace {
using uniqueExpression = std::unique_ptr<Expression>;
}  // namespace
std::ostream &operator<<(std::ostream &os, const Expression &e) {
    e.print(os);
    return os;
}

uniqueExpression simplify(const uniqueExpression &expr) {
    bool mayThrow = false;
    return simplify(expr, mayThrow);
}

uniqueExpression simplify(const uniqueExpression &expr, bool &mayThrow) {
    auto isConst = [](const uniqueExpression &e, const Fraction &v) {
        return e->kind() == ExprType::Const && static_cast<const Const &>(*e).value_ == v;
    };

    auto isNonZeroConst = [&isConst](const uniqueExpression &e) {
        return e->kind() == ExprType::Const && !isConst(e, Fraction(0));
    };

    if (expr->kind() == ExprType::Const) {
        mayThrow = false;
        return std::make_unique<Const>(static_cast<const Const &>(*expr).value_);
    }

    if (expr->kind() == ExprType::Negative) {
        const auto &inner = static_cast<const Negative &>(*expr).value_;
        if (inner->kind() == ExprType::Negative) {
            return simplify(static_cast<const Negative &>(*inner).value_, mayThrow);
        }
        return std::make_unique<Negative>(simplify(inner, mayThrow));
    }

    const BinaryExpression &b = static_cast<const BinaryExpression &>(*expr);
    bool lThrow = false, rThrow = false;
    uniqueExpression l = simplify(b.lhs, lThrow);
    uniqueExpression r = simplify(b.rhs, rThrow);
    const Fraction zero(0), one(1);

    switch (b.kind()) {
        case ExprType::Add:
            if (isConst(l, zero)) {
                mayThrow = rThrow;
                return r;
            }
            if (isConst(r, zero)) {
                mayThrow = lThrow;
                return l;
            }
            mayThrow = lThrow || rThrow;
            return std::make_unique<Add>(std::move(l), std::move(r));

        case ExprType::Sub:
            if (isConst(l, zero)) {
                mayThrow = rThrow;
                return std::make_unique<Negative>(std::move(r));
            }
            if (isConst(r, zero)) {
                mayThrow = lThrow;
                return l;
            }
            mayThrow = lThrow || rThrow;
            return std::make_unique<Sub>(std::move(l), std::move(r));

        case ExprType::Mul:

            if ((isConst(l, zero) && !rThrow) || (isConst(r, zero) && !lThrow)) {
                mayThrow = false;
                return std::make_unique<Const>(zero);
            }
            if (isConst(l, one)) {
                mayThrow = rThrow;
                return r;
            }
            if (isConst(r, one)) {
                mayThrow = lThrow;
                return l;
            }
            mayThrow = lThrow || rThrow;
            return std::make_unique<Mul>(std::move(l), std::move(r));

        case ExprType::Div:
            if (isConst(r, one)) {
                mayThrow = lThrow;
                return l;
            }

            if (isConst(l, zero) && isNonZeroConst(r)) {
                mayThrow = false;
                return std::make_unique<Const>(zero);
            }

            mayThrow = lThrow || rThrow || !isNonZeroConst(r);
            return std::make_unique<Div>(std::move(l), std::move(r));

        default:
            break;
    }
    return nullptr;
}

Const::Const(const Fraction &value) : value_(value) {};
Fraction Const::evaluate() const {
    return value_;
}
void Const::print(std::ostream &os) const {
    os << evaluate();
}
ExprType Const::kind() const {
    return ExprType::Const;
}
Negative::Negative(uniqueExpression operand) : value_(std::move(operand)) {};
Fraction Negative::evaluate() const {
    return -value_->evaluate();
};
void Negative::print(std::ostream &os) const {
    os << "(-" << *value_ << ")";
};
ExprType Negative::kind() const {
    return ExprType::Negative;
}
BinaryExpression::BinaryExpression(uniqueExpression lhs, uniqueExpression rhs)
    : lhs(std::move(lhs)), rhs(std::move(rhs)) {};

#define DECLARE_BINARYEXPR(FUNC) \
    FUNC(Add, +);                \
    FUNC(Sub, -);                \
    FUNC(Mul, *);                \
    FUNC(Div, /);
#define CONSTRUCTOR(tp, op) \
    tp::tp(uniqueExpression lhs, uniqueExpression rhs) : BinaryExpression(std::move(lhs), std::move(rhs)) {};
#define EVALUATE(tp, op)                           \
    Fraction tp::evaluate() const {                \
        return lhs->evaluate() op rhs->evaluate(); \
    }
#define PRINT(tp, op)                                          \
    void tp::print(std::ostream &os) const {                   \
        os << "(" << *lhs << " " << #op << " " << *rhs << ")"; \
    }
#define KIND(tp, op)            \
    ExprType tp::kind() const { \
        return ExprType::tp;    \
    }
DECLARE_BINARYEXPR(CONSTRUCTOR)
DECLARE_BINARYEXPR(EVALUATE)
DECLARE_BINARYEXPR(PRINT)
DECLARE_BINARYEXPR(KIND)

#undef KIND
#undef PRINT
#undef EVALUATE
#undef CONSTRUCTOR
#undef DECLARE_BINARYEXPR
}  // namespace ratexpr
