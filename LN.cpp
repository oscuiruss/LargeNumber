#include <string>
#include "LN.h"

static const uint32_t BASE = 1e9;
static const int BLOCK_SIZE = 9;


LN::LN(int a) : LN(static_cast<long long>(a)) {}

LN::LN(long long a) {
    isNaN_ = false;
    digits_ = new(std::nothrow) uint32_t[3];
    if (!digits_) {
        throw "Error: memory was not allocated";
    }
    if (a < 0) {
        a *= -1;
        sign_ = -1;
    }
    digits_[0] = abs(a % BASE);
    digits_[1] = abs((a /= BASE) % BASE);
    digits_[2] = abs(a / BASE);
    len_ = 3;
    delete_zero();
}

LN::LN(std::string_view str) : LN(str.data()) {}

LN::LN(const char *str) {
    size_t size = strlen(str);
    int pos = 0;
    sign_ = 1;
    if (str[0] == '-' || str[0] == '+') {
        if (str[0] == '-') {
            sign_ = -1;
        }
        pos++;
    }

    len_ = (size - pos - 1) / BLOCK_SIZE + 1;
    digits_ = new(std::nothrow) uint32_t[len_];
    if (!digits_) {
        throw "Error: memory was not allocated";
    }
    memset(digits_, 0, len_ * sizeof(uint32_t));
    int index = 0;
    for (int i = size; i > pos; i -= BLOCK_SIZE) {
        std::string temp;
        if (i >= BLOCK_SIZE + pos) {
            for (int j = i - BLOCK_SIZE; j < i; j++) {
                temp += str[j];
            }
        } else {
            for (int j = pos; j < i; j++) {
                temp += str[j];
            }
        }
        digits_[index] = stoi(temp);
        index++;
    }
    isNaN_ = false;
    delete_zero();
}

LN::operator long long() const {
    if (*this > INT64_MAX || *this < INT64_MIN ) {
        throw "The value is bigger than long long";
    }
    if (isNaN_) {
        throw "NaN cast to long long";
    }
    long long res = 0;
    for (int i = len_ - 1; i >= 0; --i) {
        res *= BASE;
        res += digits_[i];
    }
    res *= sign_;
    return res;
}

LN::operator bool() const {
    if (len_ == 1 && digits_[0] == 0) return false;
    return true;
}

std::string LN::string_() const {
    if (isNaN_) {
        return "NaN";
    }
    std::string str;
    if (sign_ == -1) {
        str += "-";
    }
    for (int i = len_ - 1; i >= 0; i--) {
        std::string cur = std::to_string(digits_[i]);
        if (cur.size() < BLOCK_SIZE & i != len_ - 1) {
            size_t l = cur.size();
            for (int j = 0; j < BLOCK_SIZE - l; j++) {
                str += '0';
            }
        }
        str += cur;
    }
    return str;
}

LN::LN(LN &&x) noexcept {
    isNaN_ = x.isNaN_;
    if (isNaN_) {
        return;
    }
    sign_ = x.sign_;
    len_ = x.len_;
    digits_ = x.digits_;
    x.digits_ = nullptr;
}

LN::LN(const LN &x) {
    isNaN_ = x.isNaN_;
    if (isNaN_) {
        return;
    }
    sign_ = x.sign_;
    len_ = x.len_;
    digits_ = new(std::nothrow)uint32_t[len_];
    if (!digits_) {
        throw "Error: memory was not allocated";
    }
    memcpy(digits_, x.digits_, len_ * sizeof(uint32_t));
}

void LN::delete_zero() {
    size_t i = len_;
    if (i == 0) {
        delete[] digits_;
        digits_ = new(std::nothrow) uint32_t[1];
        if (!digits_) {
            throw "Error: memory was not allocated";
        }
        sign_ = 1;
        len_ = 1;
        digits_[0] = 0;
        return;
    }
    while (i > 1 && digits_[i - 1] == 0) {
        i--;
    }
    if (i == 1 && digits_[0] == 0) {
        sign_ = 1;
    }
    if (i == len_) {
        return;
    }
    auto *res = new(std::nothrow) uint32_t[i];
    if (!res) {
        throw "Error: memory was not allocated";
    }
    memcpy(res, digits_, i * sizeof(uint32_t));
    len_ = i;
    delete[] digits_;
    digits_ = res;
}

int LN::compareDigits(const LN &x) const {
    if (len_ < x.len_) {
        return -1;
    }
    if (len_ > x.len_) {
        return 1;
    }
    for (int i = x.len_ - 1; i >= 0; i--) {
        if (digits_[i] > x.digits_[i]) {
            return 1;
        }
        if (digits_[i] < x.digits_[i]) {
            return -1;
        }
    }
    return 0;
}

LN LN::operator==(const LN &x) const {
    if (isNaN_ || x.isNaN_) return 0;
    if (sign_ == x.sign_ && compareDigits(x) == 0) {
        return 1;
    }
    return 0;
}

LN LN::operator!=(const LN &x) const {
    if (isNaN_ || x.isNaN_) return 1;
    if (sign_ != x.sign_ || compareDigits(x) != 0) {
        return 1;
    }
    return 0;
}

LN LN::operator<(const LN &x) const {
    if (isNaN_ || x.isNaN_) return 0;
    if (sign_ == x.sign_) {
        int compare = compareDigits(x);
        if (compare == 0) return 0;
        if (compare < 0) {
            if (sign_ == -1) return 0;
            return 1;
        }
        if (sign_ == -1) return 1;
        return 0;
    }
    if (sign_ == -1) return 1;
    return 0;
}

LN LN::operator>(const LN &x) const {
    if (isNaN_ || x.isNaN_) return 0;
    if (sign_ == x.sign_) {
        int compare = compareDigits(x);
        if (compare == 0) return 0;
        if (compare < 0) {
            if (sign_ == -1) return 1;
            return 0;
        }
        if (sign_ == -1) return 0;
        return 1;
    }
    if (sign_ == -1) return 0;
    return 1;
}

LN LN::operator<=(const LN &x) const {
    if (isNaN_ || x.isNaN_) return 0;
    if (sign_ == x.sign_) {
        int compare = compareDigits(x);
        if (compare == 0) return 1;
        if (compare < 0) {
            if (sign_ == -1) return 0;
            return 1;
        }
        if (sign_ == -1) return 1;
        return 0;
    }
    if (sign_ == -1) return 1;
    return 0;
}

LN LN::operator>=(const LN &x) const {
    if (isNaN_ || x.isNaN_) return 0;
    if (sign_ == x.sign_) {
        int compare = compareDigits(x);
        if (compare == 0) return 1;
        if (compare < 0) {
            if (sign_ == -1) return 1;
            return 0;
        }
        if (sign_ == -1) return 0;
        return 1;
    }
    if (sign_ == -1) return 0;
    return 1;
}

LN &LN::operator=(const LN &x) {
    if (this == &x) {
        return *this;
    }
    isNaN_ = x.isNaN_;
    sign_ = x.sign_;
    len_ = x.len_;
    delete[] digits_;
    digits_ = new(std::nothrow) uint32_t[len_];
    if (!digits_) {
        throw "Error: memory was not allocated";
    }
    memcpy(digits_, x.digits_, len_ * sizeof(uint32_t));
    return *this;
}

LN &LN::operator=(LN &&x) noexcept {
    if (this == &x) {
        return *this;
    }
    isNaN_ = x.isNaN_;
    sign_ = x.sign_;
    len_ = x.len_;
    delete[] digits_;
    digits_ = x.digits_;
    x.digits_ = nullptr;
    return *this;
}

LN &LN::operator+=(const LN &x) {
    if (isNaN_ || x.isNaN_) {
        LN a = LN(0);
        a.isNaN_ = true;
        *this = a;
        return *this;
    }
    if (sign_ == x.sign_) {
        add(x);
    } else {
        sub(x);
    }
    return *this;
}

void LN::add(const LN &x) {
    size_t new_size = std::max(len_, x.len_);
    uint32_t carry = 0;
    for (int i = 0; i < new_size; i++) {
        uint32_t cur = carry + (i < x.len_ ? x.digits_[i] : 0) + (i < len_ ? digits_[i] : 0);
        carry = cur / BASE;
        digits_[i] = cur % BASE;
    }
    if (carry != 0) {
        auto res = new(std::nothrow) uint32_t[new_size + 1];
        if (!res) {
            throw "Error: memory was not allocated";
        }
        for (int i = 0; i < len_; i++) {
            res[i] = digits_[i];
        }
        digits_ = res;
        digits_[new_size] = carry;
        len_ = new_size + 1;
    }
    delete_zero();
}

void LN::sub(const LN &x) {
    int new_size = std::max(len_, x.len_);
    int compare = compareDigits(x);
    sign_ *= compare;
    if (compare == 0) {
        *this = LN(0);
    } else {
        int32_t carry = 0;
        for (int i = 0; i < new_size; i++) {
            int32_t first = (i < len_ ? digits_[i] : 0);
            int32_t second = (i < x.len_ ? x.digits_[i] : 0);
            int cur = (first - second) * compare - carry;
            if (cur < 0) {
                digits_[i] = BASE + cur;
                carry = 1;
            } else {
                digits_[i] = cur;
            }
        }
    }
    delete_zero();
}

LN LN::operator+(const LN &x) const {
    LN temp = *this;
    temp += x;
    return temp;
}

LN &LN::operator-=(const LN &x) {
    if (isNaN_ || x.isNaN_) {
        LN a;
        a.isNaN_ = true;
        *this = a;
        return *this;
    }
    if (sign_ == x.sign_) {
        sub(x);
    } else {
        add(x);
    }
    return *this;
}

LN LN::operator-(const LN &x) const {
    return LN(*this) -= x;
}

LN &LN::operator*=(const LN &x) {
    if (isNaN_ || x.isNaN_) {
        LN a = LN();
        a.isNaN_ = true;
        *this = a;
        return *this;
    }
    if (x == 0 || *this == 0) {
        return *this = LN(0);
    }

    size_t new_size = x.len_ + len_;
    auto *res = new(std::nothrow) uint64_t[new_size];
    if (!res) {
        throw "Error: memory was not allocated";
    }
    memset(res, 0, new_size * sizeof(uint64_t));
    for (size_t i = 0; i < len_; i++) {
        for (size_t j = 0; j < x.len_; j++) {
            long long p = (long long) digits_[i] * (long long) x.digits_[j];
            res[i + j] += p % BASE;
            res[i + j + 1] += (uint32_t) (p / BASE + res[i + j] / BASE);
            res[i + j] %= BASE;
        }
    }

    digits_ = new(std::nothrow)uint32_t[new_size];
    if (!digits_) {
        throw "Error: memory was not allocated";
    }
    for (int i = 0; i < new_size; i++) {
        digits_[i] = res[i];
    }
    len_ = new_size;
    delete[]res;
    sign_ *= x.sign_;
    delete_zero();
    return *this;
}

LN LN::operator*(const LN &x) const {
    return LN(*this) *= x;
}

LN &LN::operator/=(const LN &x) {
    if (isNaN_ || x.isNaN_ || x.len_ == 1 && x.digits_[0] == 0) {
        LN a = LN();
        a.isNaN_ = true;
        *this = a;
        return *this;
    }
    if (compareDigits(x) == -1) {
        return (*this = LN(0));
    }
    LN cur, res;
    res.digits_ = new(std::nothrow) uint32_t[len_];
    if (!digits_) {
        throw "Error: memory was not allocated";
    }
    memset(res.digits_, 0, len_ * sizeof(uint32_t));
    for (int i = len_ - 1; i >= 0; i--) {
        cur.shift_for_one_sign(digits_[i]);
        int64_t l = 0, r = BASE;
        while (r - l > 1) {
            int64_t m = (l + r) / 2;
            if (cur.compareDigits(x * (long long) m) >= 0) {
                l = m;
            } else r = m;
        }
        res.digits_[i] = l;
        cur = cur - x * LN((long long) l);
    }
    res.sign_ = sign_ * x.sign_;
    res.delete_zero();
    return *this = res;
}

LN LN::operator/(const LN &x) const {
    return LN(*this) /= x;
}

LN &LN::operator%=(const LN &x) {
    if (isNaN_ || x.isNaN_ || x.len_ == 1 && x.digits_[0] == 0) {
        LN a = LN();
        a.isNaN_ = true;
        *this = a;
        return *this;
    }
    if (compareDigits(x) == -1) {
        return *this;
    }
    *this = (*this - (*this / x) * x);
    return *this;
}

LN LN::operator%(const LN &x) const {
    return LN(*this) %= x;
}


void LN::shift_for_one_sign(uint32_t x) {
    auto *res = new(std::nothrow) uint32_t[++len_];
    if (!digits_) {
        throw "Error: memory was not allocated";
    }
    memcpy(res + 1, digits_, len_ * sizeof(uint32_t));
    res[0] = x;
    delete[] digits_;
    digits_ = res;
    delete_zero();
}

LN LN::operator-() const {
    LN temp = *this;
    if (len_ == 1 && digits_[0] == 0){
        temp.sign_ = 1;
    }
    else {
        temp.sign_ = temp.sign_ * -1;
    }
    return temp;
}

LN LN::operator~() const {
    if (*this < LN(0)) {
        LN a = LN();
        a.isNaN_ = true;
        return a;
    }
    if (isNaN_) {
        return *this;
    }
    if (len_ == 1 && digits_[0] == 0) {
        return 0;
    }
    LN TWO(2), t;
    LN l = *this, r = (*this + 1) / 2;
    while (r < l) {
        l = r;
        t = *this;
        t /= r;
        r = (r + t) / 2;
    }
    return l;
}

LN operator "" _ln(const char *str) {
    return LN(str);
}

LN::~LN() {
    delete[] digits_;
}
