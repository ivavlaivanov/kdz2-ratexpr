#pragma once
#include "ratexpr/fraction.hpp"
#include <memory>
#include <ostream>
#include <cstdint>

namespace ratexpr {
class Expression {
public:
    virtual ~Expression() = default;
    virtual Fraction evaluate() const = 0;
    virtual void print(std::ostream &os) const = 0;
};
std::ostream &operator<<(std::ostream &os, const Expression &e);
class Const : public Expression {
    const Fraction value_;

public:
    explicit Const(const Fraction &value);
    virtual Fraction evaluate() const override final;
    virtual void print(std::ostream &os) const override final;
};
class Negative : public Expression {
    const std::unique_ptr<Expression> value_;

public:
    explicit Negative(std::unique_ptr<Expression> operand);
    virtual Fraction evaluate() const override final;
    virtual void print(std::ostream &os) const override final;
};
class BinaryExpression : public Expression {
protected:
    const std::unique_ptr<Expression> lhs, rhs;

public:
    BinaryExpression(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs);
    virtual Fraction evaluate() const = 0;
    virtual void print(std::ostream &os) const = 0;
};
#define DECLARE_CLASS(CLS) \
    CLS(Add);              \
    CLS(Sub);              \
    CLS(Mul);              \
    CLS(Div);
#define BINARY_EXPR(T)                                                       \
    class T : public BinaryExpression {                                      \
    public:                                                                  \
        T(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs); \
        Fraction evaluate() const override final;                            \
        void print(std::ostream &os) const override final;                   \
    }

DECLARE_CLASS(BINARY_EXPR)
#undef BINARY_EXPR
#undef DECLARE_CLASS

// class Add : public BinaryExpression {
// public:
//     Add(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs);
//     Fraction evaluate() const override;
//     void print(std::ostream &os) const override;
// };  // и так же Sub, Mul, Div
// class Sub : public BinaryExpression {
// public:
//     Sub(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs);
//     Fraction evaluate() const override;
//     void print(std::ostream &os) const override;
// };
// class Mul : public BinaryExpression {
// public:
//     Mul(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs);
//     Fraction evaluate() const override;
//     void print(std::ostream &os) const override;
// };
// class Div : public BinaryExpression {
// public:
//     Div(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs);
//     Fraction evaluate() const override;
//     void print(std::ostream &os) const override;
// };
}  // namespace ratexpr