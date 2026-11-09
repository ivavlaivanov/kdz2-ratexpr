#include <iostream>
#include <stdexcept>

#include "ratexpr/fraction.hpp"
using ratexpr::Fraction;

int main() {
    const Fraction firstFrac(1, 2);
    const Fraction secondFrac(1, 3);

    std::cout << "Fraction(6, -8)  = " << Fraction(6, -8) << '\n';
    std::cout << "Fraction(10, 4)  = " << Fraction(10, 4) << '\n';
    std::cout << std::endl;

    std::cout << firstFrac << " + " << secondFrac << " = " << firstFrac + secondFrac << '\n';
    std::cout << firstFrac << " - " << secondFrac << " = " << firstFrac - secondFrac << '\n';
    std::cout << firstFrac << " * " << secondFrac << " = " << firstFrac * secondFrac << '\n';
    std::cout << firstFrac << " / " << secondFrac << " = " << firstFrac / secondFrac << '\n';
    std::cout << std::endl;

    std::cout << firstFrac << " + " << 2u << "   = " << firstFrac + 2u << '\n';
    std::cout << 2u << " + " << firstFrac << "   = " << 2u + firstFrac << '\n';
    std::cout << firstFrac << " - " << 2u << "   = " << firstFrac - 2u << '\n';
    std::cout << 2u << " - " << firstFrac << "   = " << 2u - firstFrac << '\n';
    std::cout << firstFrac << " * " << 2u << "   = " << firstFrac * 2u << '\n';
    std::cout << 2u << " * " << firstFrac << "   = " << 2u * firstFrac << '\n';
    std::cout << firstFrac << " / " << 2u << "   = " << firstFrac / 2u << '\n';
    std::cout << 2u << " / " << firstFrac << "   = " << 2u / firstFrac << '\n';
    std::cout << std::endl;

    Fraction acc;
    acc += firstFrac;
    acc += secondFrac;
    acc *= 6u;
    std::cout << "acc = 0; acc += 1/2; acc += 1/3; acc *= 6u -> " << acc << '\n';
    std::cout << std::endl;

    try {
        const Fraction bad(1, 0);
        std::cout << "Fraction(1, 0)   = " << bad << '\n';
    } catch (const std::invalid_argument &e) {
        std::cout << "Fraction(1, 0)   -> std::invalid_argument: " << e.what() << '\n';
    }

    try {
        const Fraction bad = firstFrac / 0u;
        std::cout << firstFrac << " / " << 0u << " = " << bad << '\n';
    } catch (const std::invalid_argument &e) {
        std::cout << firstFrac << " / " << 0u << "          -> std::invalid_argument: " << e.what() << '\n';
    }

    return 0;
}