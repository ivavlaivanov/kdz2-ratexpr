#include "ratexpr/fraction.hpp"
#include <stdexcept>
#include <cstdint>
#include <numeric>
#include <string>
#include <cstdlib>

namespace ratexpr {
Fraction::Fraction() : num(0), den(1) {};
Fraction::Fraction(const Fraction &o) : num(o.num), den(o.den) {
}
Fraction::Fraction(Fraction::unchecked &&, std::int64_t numerator, std::int64_t denominator)
    : num(numerator), den(denominator) {};
Fraction::Fraction(std::int64_t numerator, std::int64_t denominator) {
    if (!(denominator != 0)) {
        throw std::invalid_argument("denomiantor must be not equal zero");
    }
    den = denominator > 0 ? denominator : -denominator;
    num = denominator > 0 ? numerator : -numerator;
    normalize();
}
void Fraction::normalize() {
    std::int64_t gcd = std::gcd(den, std::abs(num));
    den /= gcd;
    num /= gcd;
}

Fraction Fraction::operator-() const {
    return Fraction(Fraction::unchecked{}, -num, den);
}
Fraction &Fraction::operator+=(const Fraction &o) {
    std::int64_t _den = std::lcm(o.den, den);
    num = num * (_den / den) + (_den / o.den) * o.num;
    den = _den;
    normalize();
    return *this;
}
Fraction &Fraction::operator*=(const Fraction &o) {
    std::int64_t l = std::gcd(std::abs(num), o.den), r = std::gcd(std::abs(o.num), den);
    num = (num / l) * (o.num / r);
    den = (den / r) * (o.den / l);

    return *this;
}
Fraction &Fraction::operator-=(const Fraction &o) {
    *this += -o;
    return *this;
}
Fraction &Fraction::operator/=(const Fraction &o) {
    if (o.num == 0) {
        throw std::invalid_argument("division by zero");
    }
    *this *= Fraction(Fraction::unchecked{}, (o.num > 0 ? o.den : -o.den), std::abs(o.num));
    return *this;
}

#define DECLARE_OPERATORS(_op) _op(+) _op(-) _op(*) _op(/)
#define mutable_with_uint32_t(s)                           \
    Fraction &Fraction::operator s## = (std::uint32_t k) { \
        *this s## = Fraction(Fraction::unchecked{}, k);    \
        return *this;                                      \
    }
DECLARE_OPERATORS(mutable_with_uint32_t)
#undef mutable_with_uint32_t
#define immutable_with_fraction(s)                              \
    Fraction operator s(const Fraction &a, const Fraction &b) { \
        return (Fraction(a) s## = b);                           \
    }
DECLARE_OPERATORS(immutable_with_fraction)
#undef immutable_with_fraction
#define immutable_withR_uint32_t(s)                           \
    Fraction operator s(const Fraction &a, std::uint32_t k) { \
        return (Fraction(a) s## = k);                         \
    }
DECLARE_OPERATORS(immutable_withR_uint32_t)
#undef immutable_withR_uint32_t
#define immutable_withL_uint32_t(s)                           \
    Fraction operator s(std::uint32_t k, const Fraction &a) { \
        return (Fraction(Fraction::unchecked{}, k) s## = a);  \
    }
DECLARE_OPERATORS(immutable_withL_uint32_t)
#undef immutable_withL_uint32_t
#undef DECLARE_OPERATORS

std::ostream &operator<<(std::ostream &os, const Fraction &f) {
    os << f.num << (f.den != 1 ? "/" + std::to_string(f.den) : "");
    return os;
}
}  // namespace ratexpr
