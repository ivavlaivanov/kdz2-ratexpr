#pragma once
#include <ostream>
#include <cstdint>
namespace ratexpr {
class Fraction {
    int64_t num;
    int64_t den;
    void normalize();
    struct unchecked {};
    Fraction(unchecked &&, std::int64_t numerator, std::int64_t denominator = 1);

public:
    Fraction();
    Fraction(std::int64_t numerator, std::int64_t denominator = 1);

    Fraction &operator+=(const Fraction &o);
    Fraction &operator-=(const Fraction &o);
    Fraction &operator*=(const Fraction &o);
    Fraction &operator/=(const Fraction &o);
    Fraction operator-() const;

    Fraction &operator+=(std::uint32_t k);
    Fraction &operator-=(std::uint32_t k);
    Fraction &operator*=(std::uint32_t k);
    Fraction &operator/=(std::uint32_t k);

    friend bool operator==(const Fraction &a, const Fraction &b);

    friend Fraction operator+(const Fraction &a, const Fraction &b);
    friend Fraction operator+(const Fraction &a, std::uint32_t k);
    friend Fraction operator+(std::uint32_t k, const Fraction &b);

    friend Fraction operator-(const Fraction &a, const Fraction &b);
    friend Fraction operator-(const Fraction &a, std::uint32_t k);
    friend Fraction operator-(std::uint32_t k, const Fraction &b);

    friend Fraction operator*(const Fraction &a, const Fraction &b);
    friend Fraction operator*(const Fraction &a, std::uint32_t k);
    friend Fraction operator*(std::uint32_t k, const Fraction &b);

    friend Fraction operator/(const Fraction &a, const Fraction &b);
    friend Fraction operator/(const Fraction &a, std::uint32_t k);
    friend Fraction operator/(std::uint32_t k, const Fraction &b);

    friend std::ostream &operator<<(std::ostream &os, const Fraction &f);
};
}  // namespace ratexpr
