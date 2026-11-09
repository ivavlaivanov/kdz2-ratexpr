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

Const::Const(const Fraction &value) : value_(value) {};
Fraction Const::evaluate() const {
    return value_;
}
void Const::print(std::ostream &os) const {
    os << evaluate();
}
Negative::Negative(std::unique_ptr<Expression> operand) : value_((uniqueExpression &&)operand) {};
Fraction Negative::evaluate() const {
    return -value_->evaluate();
};
void Negative::print(std::ostream &os) const {
    os << "(-" << *value_ << ")";
};
BinaryExpression::BinaryExpression(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs)
    : lhs((uniqueExpression &&)lhs), rhs((uniqueExpression &&)rhs) {};

#define DECLARE_BINARYEXPR(FUNC) \
    FUNC(Add, +);                \
    FUNC(Sub, -);                \
    FUNC(Mul, *);                \
    FUNC(Div, /);
#define CONSTRUCTOR(tp, op)                                                  \
    tp::tp(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs) \
        : BinaryExpression((uniqueExpression &&)lhs, (uniqueExpression &&)rhs) {};
#define EVALUATE(tp, op)                           \
    Fraction tp::evaluate() const {                \
        return lhs->evaluate() op rhs->evaluate(); \
    }
#define PRINT(tp, op)                                          \
    void tp::print(std::ostream &os) const {                   \
        os << "(" << *lhs << " " << #op << " " << *rhs << ")"; \
    }
DECLARE_BINARYEXPR(CONSTRUCTOR)
DECLARE_BINARYEXPR(EVALUATE)
DECLARE_BINARYEXPR(PRINT)

#undef PRINT
#undef EVALUATE
#undef CONSTRUCTOR
#undef DECLARE_BINARYEXPR
}  // namespace ratexpr
