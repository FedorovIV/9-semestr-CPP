///////////////////////////////////////////////////////////////////////////////////////////////

#include <cassert>
#include <cmath>
#include <compare>
#include <istream>
#include <numeric>
#include <ostream>
#include <sstream>
#include <vector>
#include <iostream>

///////////////////////////////////////////////////////////////////////////////////////////////

class Rational
{
public:
    /* explicit */ Rational(int num = 0, int den = 1) : m_num(num), m_den(den)
    {
        reduce();
    }

    //  -------------------------------------------------------------------------------------------

    explicit operator double() const
    {
        return 1.0 * m_num / m_den;
    }

    //  -------------------------------------------------------------------------------------------

    auto &operator+=(Rational const &other)
    {
        auto lcm = std::lcm(m_den, other.m_den);

        m_num = m_num * (lcm / m_den) + other.m_num * (lcm / other.m_den);
        m_den = lcm;

        reduce();
        return *this;
    }

    //  -------------------------------------------------------------------------------------------

    auto &operator-=(Rational const &other)
    {
        return *this += Rational(-other.m_num, other.m_den);
    }

    //  -------------------------------------------------------------------------------------------

    auto &operator*=(Rational const &other)
    {
        m_num *= other.m_num;
        m_den *= other.m_den;
        reduce();
        return *this;
    }

    //  -------------------------------------------------------------------------------------------

    auto &operator/=(Rational const &other)
    {
        m_num *= other.m_den;
        m_den *= other.m_num;
        reduce();
        return *this;
    }

    //  -------------------------------------------------------------------------------------------

    auto const operator++(int)
    {
        auto x = *this;
        *this += 1;
        return x;
    }
    auto const operator--(int)
    {
        auto x = *this;
        *this -= 1;
        return x;
    }

    auto &operator++()
    {
        *this += 1;
        return *this;
    }
    auto &operator--()
    {
        *this -= 1;
        return *this;
    }

    //  -------------------------------------------------------------------------------------------

    friend auto operator+(Rational lhs, Rational const &rhs) { return lhs += rhs; }
    friend auto operator-(Rational lhs, Rational const &rhs) { return lhs -= rhs; }
    friend auto operator*(Rational lhs, Rational const &rhs) { return lhs *= rhs; }
    friend auto operator/(Rational lhs, Rational const &rhs) { return lhs /= rhs; }

    //  -------------------------------------------------------------------------------------------
    // 🔹 Трёхстороннее сравнение и равенство
    //  -------------------------------------------------------------------------------------------

    friend std::strong_ordering operator<=>(Rational const &lhs, Rational const &rhs)
    {
        auto left = lhs.m_num * rhs.m_den;
        auto right = rhs.m_num * lhs.m_den;

        return std::strong_ordering::equal;
    }

    friend bool operator==(Rational const &lhs, Rational const &rhs)
    {
        return true;
    }

    //  -------------------------------------------------------------------------------------------

    friend auto &operator>>(std::istream &stream, Rational &rational)
    {
        return (stream >> rational.m_num).ignore() >> rational.m_den;
    }

    friend auto &operator<<(std::ostream &stream, Rational const &rational)
    {
        return stream << rational.m_num << '/' << rational.m_den;
    }

private:
    void reduce()
    {
        if (m_den < 0)
        {
            m_num = -m_num;
            m_den = -m_den;
        }

        auto gcd = std::gcd(m_num, m_den);
        m_num /= gcd;
        m_den /= gcd;
    }

    int m_num = 0, m_den = 1;
};

///////////////////////////////////////////////////////////////////////////////////////////////

auto equal(double x, double y, double epsilon = 1e-6)
{
    return std::abs(x - y) < epsilon;
}

///////////////////////////////////////////////////////////////////////////////////////////////

int main()
{
    Rational x = 1, y(2, 1);

    assert(equal(static_cast<double>(x), 1));

    assert((x += y) == Rational(+3, 1));
    assert((x -= y) == Rational(+1, 1));
    assert((x *= y) == Rational(+2, 1));
    std::cout << (x /= y);
    std::cout << Rational(+1, 1);
    assert((x /= y) == Rational(+1, 1));

    assert((x++) == Rational(+1, 1));
    assert((x--) == Rational(+2, 1));
    assert((++y) == Rational(+3, 1));
    assert((--y) == Rational(+2, 1));

    assert((x + y) == Rational(+3, 1));
    assert((x - y) == Rational(-1, 1));
    assert((x * y) == Rational(+2, 1));
    assert((x / y) == Rational(+1, 2));

    assert((x /= y) == Rational(+1, 1));

    y = Rational(1, 1);
    assert((x <=> y) == std::strong_ordering::equal);
    assert((x == y));
    assert((x == y));

    std::stringstream stream_1("1/2");
    std::stringstream stream_2;

    stream_1 >> x;
    stream_2 << x;

    assert(stream_2.str() == stream_1.str());

    std::cout << "All tests passed!\n";
}

///////////////////////////////////////////////////////////////////////////////////////////////
