#include <iostream>
#include <memory>

#include "ratexpr/expression.hpp"
#include "ratexpr/fraction.hpp"
using ratexpr::Add;
using ratexpr::Const;
using ratexpr::Div;
using ratexpr::Expression;
using ratexpr::Fraction;
using ratexpr::Mul;
using ratexpr::Negative;
using ratexpr::Sub;

int main() {
    const std::unique_ptr<Expression> product = std::make_unique<Mul>(
        std::make_unique<Add>(std::make_unique<Const>(Fraction(1, 2)), std::make_unique<Const>(Fraction(1, 3))),
        std::make_unique<Sub>(std::make_unique<Const>(Fraction(3)), std::make_unique<Const>(Fraction(1, 4))));
    std::cout << *product << " = " << product->evaluate() << '\n';

    const std::unique_ptr<Expression> negation = std::make_unique<Negative>(
        std::make_unique<Div>(std::make_unique<Const>(Fraction(2)), std::make_unique<Const>(Fraction(3))));
    std::cout << *negation << " = " << negation->evaluate() << '\n';

    return 0;
}