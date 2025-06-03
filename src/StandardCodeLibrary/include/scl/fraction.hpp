#pragma once

#include <numeric>
#include <ostream>

template<class T>
struct Fraction {
    T numerator;
    T denominator;
    Fraction(T numerator_, T denominator_) : numerator(numerator_), denominator(denominator_) {
        if (denominator < 0) {
            denominator = -denominator;
            numerator = -numerator;
        }
    }
    Fraction() : Fraction(0, 1) {}
    explicit Fraction(T numerator_) : Fraction(numerator_, 1) {}
    explicit operator double() const {
        return 1. * numerator / denominator;
    }
    Fraction &operator+=(const Fraction &rhs) {
        numerator = numerator * rhs.denominator + rhs.numerator * denominator;
        denominator *= rhs.denominator;
        return *this;
    }
    Fraction &operator-=(const Fraction &rhs) {
        numerator = numerator * rhs.denominator - rhs.numerator * denominator;
        denominator *= rhs.denominator;
        return *this;
    }
    Fraction &operator*=(const Fraction &rhs) {
        numerator *= rhs.numerator;
        denominator *= rhs.denominator;
        return *this;
    }
    Fraction &operator/=(const Fraction &rhs) {
        numerator *= rhs.denominator;
        denominator *= rhs.numerator;
        if (denominator < 0) {
            numerator = -numerator;
            denominator = -denominator;
        }
        return *this;
    }
    friend Fraction operator+(Fraction lhs, const Fraction &rhs) {
        return lhs += rhs;
    }
    friend Fraction operator-(Fraction lhs, const Fraction &rhs) {
        return lhs -= rhs;
    }
    friend Fraction operator*(Fraction lhs, const Fraction &rhs) {
        return lhs *= rhs;
    }
    friend Fraction operator/(Fraction lhs, const Fraction &rhs) {
        return lhs /= rhs;
    }
    friend Fraction operator-(const Fraction &a) {
        return Fraction(-a.numerator, a.denominator);
    }
    friend bool operator==(const Fraction &lhs, const Fraction &rhs) {
        return lhs.numerator * rhs.denominator == rhs.numerator * lhs.denominator;
    }
    friend bool operator!=(const Fraction &lhs, const Fraction &rhs) {
        return lhs.numerator * rhs.denominator != rhs.numerator * lhs.denominator;
    }
    friend bool operator<(const Fraction &lhs, const Fraction &rhs) {
        return lhs.numerator * rhs.denominator < rhs.numerator * lhs.denominator;
    }
    friend bool operator>(const Fraction &lhs, const Fraction &rhs) {
        return lhs.numerator * rhs.denominator > rhs.numerator * lhs.denominator;
    }
    friend bool operator<=(const Fraction &lhs, const Fraction &rhs) {
        return lhs.numerator * rhs.denominator <= rhs.numerator * lhs.denominator;
    }
    friend bool operator>=(const Fraction &lhs, const Fraction &rhs) {
        return lhs.numerator * rhs.denominator >= rhs.numerator * lhs.denominator;
    }
    friend std::ostream &operator<<(std::ostream &os, Fraction x) {
        T g = std::gcd(x.numerator, x.denominator);
        if (x.denominator == g) {
            return os << x.numerator / g;
        } else {
            return os << x.numerator / g << "/" << x.denominator / g;
        }
    }
};
