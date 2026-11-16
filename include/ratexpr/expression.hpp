#pragma once
#include "ratexpr/fraction.hpp"
#include <memory>
#include <ostream>
#include <cstdint>

namespace ratexpr {
enum class ExprType { Const = 1, Negative, Add, Sub, Mul, Div };
class Expression {
    friend std::unique_ptr<Expression> simplify(const std::unique_ptr<Expression> &expr);
    friend std::unique_ptr<Expression> simplify(const std::unique_ptr<Expression> &expr, bool &mayThrow);

public:
    virtual ~Expression() = default;
    virtual Fraction evaluate() const = 0;
    virtual void print(std::ostream &os) const = 0;
    virtual ExprType kind() const = 0;
};
std::ostream &operator<<(std::ostream &os, const Expression &e);
std::unique_ptr<Expression> simplify(const std::unique_ptr<Expression> &expr);
std::unique_ptr<Expression> simplify(const std::unique_ptr<Expression> &expr, bool &mayThrow);
class Const : public Expression {
    const Fraction value_;
    friend std::unique_ptr<Expression> simplify(const std::unique_ptr<Expression> &expr);
    friend std::unique_ptr<Expression> simplify(const std::unique_ptr<Expression> &expr, bool &mayThrow);

public:
    explicit Const(const Fraction &value);
    Fraction evaluate() const override final;
    void print(std::ostream &os) const override final;
    ExprType kind() const override final;
};
class Negative : public Expression {
    const std::unique_ptr<Expression> value_;
    friend std::unique_ptr<Expression> simplify(const std::unique_ptr<Expression> &expr);
    friend std::unique_ptr<Expression> simplify(const std::unique_ptr<Expression> &expr, bool &mayThrow);

public:
    explicit Negative(std::unique_ptr<Expression> operand);
    Fraction evaluate() const override final;
    void print(std::ostream &os) const override final;
    ExprType kind() const override final;
};
class BinaryExpression : public Expression {
protected:
    const std::unique_ptr<Expression> lhs, rhs;
    friend std::unique_ptr<Expression> simplify(const std::unique_ptr<Expression> &expr);
    friend std::unique_ptr<Expression> simplify(const std::unique_ptr<Expression> &expr, bool &mayThrow);

public:
    BinaryExpression(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs);
    Fraction evaluate() const override = 0;
    void print(std::ostream &os) const override = 0;
    ExprType kind() const override = 0;
};
#define DECLARE_CLASS(CLS) \
    CLS(Add);              \
    CLS(Sub);              \
    CLS(Mul);              \
    CLS(Div);
#define BINARY_EXPR(T)                                                       \
    class T : public BinaryExpression {                                      \
                                                                             \
    public:                                                                  \
        T(std::unique_ptr<Expression> lhs, std::unique_ptr<Expression> rhs); \
        Fraction evaluate() const override final;                            \
        void print(std::ostream &os) const override final;                   \
        ExprType kind() const override final;                                \
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