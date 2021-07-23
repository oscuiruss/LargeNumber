#include <cstdint>
#include <iostream>

#ifndef LN_H
#define LN_H

class LN {
public:
    LN(long long = 0);

    LN(const LN &);

    LN(LN &&) noexcept;

    LN(std::string_view);

    LN(const char *);

    LN(int i);

    ~LN();

    LN &operator=(const LN &);

    LN &operator=(LN &&) noexcept;

    LN operator+(const LN &) const;

    LN &operator+=(const LN &);

    LN operator-(const LN &) const;

    LN &operator-=(const LN &);

    LN operator*(const LN &) const;

    LN &operator*=(const LN &);

    LN operator/(const LN &) const;

    LN &operator/=(const LN &);

    LN operator%(const LN &) const;

    LN &operator%=(const LN &);

    LN operator~() const;

    LN operator-() const;

    LN operator==(const LN &) const;

    LN operator!=(const LN &) const;

    LN operator<(const LN &) const;

    LN operator<=(const LN &) const;

    LN operator>(const LN &) const;

    LN operator>=(const LN &) const;

    explicit operator long long() const;

    explicit operator bool() const;

    std::string  string_() const;

private:
    uint32_t *digits_ = nullptr;
    bool isNaN_ = false;
    int sign_ = 1;
    size_t len_ = 0;

    void shift_for_one_sign(uint32_t x);

    void delete_zero();

    int compareDigits(const LN &x) const;

    void add(const LN &);

    void sub(const LN &);
};

LN operator "" _ln(const char * str);

#endif




