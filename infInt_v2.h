#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <cstring>
#include <cstdint>

class infInt {
    std::vector<uint32_t> data;
    bool sign;
    bool NaN;
public:
    bool isNaN() const {
        return NaN;
    }
    bool isNeg() const {
        return !NaN && sign;
    }
    bool isZero() const {
        return !NaN && data.size() == 1 && data[0] == 0;
    }
    bool isOdd() const {
        return !NaN && !data.empty() && (data[0] & 1);
    }
    bool isEven() const {
        return !NaN && !data.empty() && !(data[0] & 1);
    }
    char CompareAbs(const infInt& a, const infInt&b) const {  // return abs(a) < abs(b) if abs(a) != abs(b)
        if(a.NaN || b.NaN) return false;
        if(a.data.size() != b.data.size()) return a.data.size() < b.data.size();
        int len = a.data.size();
        for(int i = len - 1; i >= 0; i--) {
            if(a.data[i] != b.data[i]) {
                return a.data[i] < b.data[i];
            }
        }
        return -1;  // returns -1 if abs(a) == abs(b)
    }
    infInt abs() const {
        if(NaN) return NONE;
        if(!sign) return *this;
        infInt ret = *this;
        ret.sign = 0;
        return ret;
    }
    infInt& trim() {
        if(NaN) return *this;
        while(data.size() > 1 && data.back() == 0) {
            data.pop_back();
        }
        if(this->isZero()) {
            sign = 0;
        }
        return *this;
    }
    infInt& negate() {
        if(NaN) return *this;
        if(!this->isZero()) sign ^= 1;
        return *this;
    }
    infInt operator-() const {
        if(NaN) return NONE;
        if(this->isZero()) return ZERO;
        infInt ret = *this;
        ret.sign ^= 1;
        return ret;
    }
    bool operator<(const infInt& other) const {
        if(NaN ^ other.NaN) return false;
        if(sign ^ other.sign) return sign;
        char cmp = CompareAbs(*this, other);
        if(cmp == -1) return false;
        return (cmp == 1) ^ sign;
    }
    bool operator>(const infInt& other) const {
        if(NaN ^ other.NaN) return false;
        if(sign ^ other.sign) return other.sign;
        char cmp = CompareAbs(*this, other);
        if(cmp == -1) return false;
        return (cmp == 0) ^ sign;
    }
    bool operator<=(const infInt& other) const {
        if(NaN ^ other.NaN) return false;
        if(sign ^ other.sign) return sign;
        char cmp = CompareAbs(*this, other);
        if(cmp == -1) return true;
        return (cmp == 1) ^ sign;
    }
    bool operator>=(const infInt& other) const {
        if(NaN ^ other.NaN) return false;
        if(sign ^ other.sign) return other.sign;
        char cmp = CompareAbs(*this, other);
        if(cmp == -1) return true;
        return (cmp == 0) ^ sign;
    }
    bool operator==(const infInt& other) const {
        if(NaN ^ other.NaN) return false;
        if(sign ^ other.sign) return false;
        return data == other.data;
    }
    bool operator!=(const infInt& other) const {
        if(NaN ^ other.NaN) return true;
        if(sign ^ other.sign) return true;
        return data != other.data;
    }
    infInt operator~() const {
        if(NaN) return NONE;
        return -(*this) - infInt(1);
    }
    infInt operator&(const infInt& other) const {
        if(NaN) return NONE;
        infInt result;
        if(!this->sign && !other.sign) {
            int len = std::min(this->data.size(), other.data.size());
            result.sign = false;
            result.data.resize(len);
            for(int i = 0; i < len; i++) {
                result.data[i] = this->data[i] & other.data[i];
            }
        } else if(this->sign && other.sign) {
            infInt A = -*this - 1;
            infInt B = -other - 1;
            return -(A | B) - 1;
        } else {
            const infInt* A = this->sign ? &other : this;
            const infInt* B = this->sign ? this : &other;
            int Alen = A->data.size();
            int Blen = B->data.size();
            int len = std::max(Alen, Blen);
            result.sign = false;
            result.data.resize(len);
            uint64_t carry = 1;
            for(int i = 0; i < len; i++) {
                uint32_t b = (i < Blen ? B->data[i] : 0U);
                uint64_t inv_b = (~b & 0xFFFFFFFFULL) + carry;
                carry = inv_b >> 32;
                result.data[i] = (i < Alen ? A->data[i] : 0U) & (uint32_t)inv_b;
            }
        }
        while(result.data.size() > 1 && result.data.back() == 0) {
            result.data.pop_back();
        }
        if(result.isZero()) return ZERO;
        return result;
    }
    infInt operator|(const infInt& other) const {
        if(NaN) return NONE;
        infInt result;
        if(!this->sign && !other.sign) {
            const infInt* A = (this->data.size() < other.data.size() ? &other : this);
            const infInt* B = (this->data.size() < other.data.size() ? this : &other);
            int Alen = A->data.size();
            int Blen = B->data.size();
            result.sign = false;
            result.data.resize(Alen);
            for(int i = 0; i < Blen; i++) {
                result.data[i] = A->data[i] | B->data[i];
            }
            for(int i = Blen; i < Alen; i++) {
                result.data[i] = A->data[i];
            }
            while(result.data.size() > 1 && result.data.back() == 0) {
                result.data.pop_back();
            }
            if(result.isZero()) return ZERO;
            return result;
        } else {
            infInt A = -*this - 1;
            infInt B = -other - 1;
            return -(A & B) - 1;
        }
    }
    infInt operator^(const infInt& other) const {
        if(NaN) return NONE;
        infInt result;
        if(!this->sign && !other.sign) {
            const infInt* A = (this->data.size() < other.data.size() ? &other : this);
            const infInt* B = (this->data.size() < other.data.size() ? this : &other);
            int Alen = A->data.size();
            int Blen = B->data.size();
            result.sign = false;
            result.data.resize(Alen);
            for(int i = 0; i < Blen; i++) {
                result.data[i] = A->data[i] ^ B->data[i];
            }
            for(int i = Blen; i < Alen; i++) {
                result.data[i] = A->data[i];
            }
            while(result.data.size() > 1 && result.data.back() == 0) {
                result.data.pop_back();
            }
            if(result.isZero()) return ZERO;
            return result;
        } else {
            infInt A = *this | other;
            infInt B = *this & other;
            return A & ~B;
        }
    }
    infInt operator>>(const uint64_t& other) const {
        if(NaN) return *this;
        if(other == 0) return *this;
        if(other >= data.size() * 32) return ZERO;
        int skipped = other / 32;
        int shift = other % 32;
        infInt result;
        result.sign = sign;
        if(shift == 0) {
            result.data.assign(data.begin() + skipped, data.end());
            return result;
        }
        int len = data.size() - skipped;
        result.data.resize(len);
        result.data[0] = data[skipped] >> shift;
        for(int i = 1; i < len; i++) {
            result.data[i - 1] |= data[i + skipped] << (32 - shift);
            result.data[i] = data[i + skipped] >> shift;
        }
        if(result.data.size() > 1 && result.data.back() == 0) {
            result.data.pop_back();
        }
        if(result.isZero()) return ZERO;
        return result;
    }
    infInt operator<<(const uint64_t& other) const {
        if(NaN) return *this;
        if(other == 0) return *this;
        if(this->isZero()) return ZERO;
        int blocks = other / 32;
        int shift = other % 32;
        infInt result;
        result.sign = sign;
        if(shift == 0) {
            result.data.resize(blocks, 0U);
            result.data.insert(result.data.end(), data.begin(), data.end());
            return result;
        }
        int len = data.size() + blocks + 1;
        result.data.resize(len, 0U);
        for(int i = blocks; i < len - 1; i++) {
            result.data[i] |= data[i - blocks] << shift;
            result.data[i + 1] = data[i - blocks] >> (32 - shift);
        }
        if(result.data.size() > 1 && result.data.back() == 0) {
            result.data.pop_back();
        }
        if(result.isZero()) return ZERO;
        return result;
    }
    infInt Add(const infInt& a, const infInt& b) const {
        infInt result;
        result.sign = a.sign;
        const infInt* A = nullptr;
        const infInt* B = nullptr;
        if(a.data.size() < b.data.size()) {
            A = &b;
            B = &a;
        } else {
            A = &a;
            B = &b;
        }
        int Alen = A->data.size();
        int Blen = B->data.size();
        result.data.resize(Alen);
        uint32_t carry = 0;
        for(int i = 0; i < Blen; i++) {
            uint32_t sum;
            bool over1 = __builtin_add_overflow(A->data[i], B->data[i], &sum);
            bool over2 = __builtin_add_overflow(sum, carry, &sum);
            carry = over1 || over2;
            result.data[i] = sum;
        }
        for(int i = Blen; i < Alen; i++) {
            uint32_t sum;
            carry = __builtin_add_overflow(A->data[i], carry, &sum);
            result.data[i] = sum;
        }
        if(carry) {
            result.data.push_back(carry);
        }
        if(result.isZero()) return ZERO;
        return result;
    }
    infInt Sub(const infInt& a, const infInt& b) const {
        char cmp = CompareAbs(a, b);
        if(cmp == -1) return ZERO;
        infInt result;
        result.sign = a.sign;
        if(a.isZero()) {
            result.data = b.data;
            result.sign = 1;
            return result;
        }
        if(b.isZero()) {
            result.data = a.data;
            return result;
        }
        const infInt* A = nullptr;
        const infInt* B = nullptr;
        if(cmp) {
            A = &b;
            B = &a;
            result.sign ^= 1;
        } else {
            A = &a;
            B = &b;
        }
        int Alen = A->data.size();
        int Blen = B->data.size();
        result.data.resize(Alen);
        uint32_t borrow = 0;
        for(int i = 0; i < Blen; i++) {
            uint32_t diff;
            bool under1 = __builtin_sub_overflow(A->data[i], B->data[i], &diff);
            bool under2 = __builtin_sub_overflow(diff, borrow, &diff);
            borrow = under1 || under2;
            result.data[i] = diff;
        }
        for(int i = Blen; i < Alen; i++) {
            uint32_t diff;
            borrow = __builtin_sub_overflow(A->data[i], borrow, &diff);
            result.data[i] = diff;
        }
        for(int i = Alen - 1; i >= 0; i--) {
            if(result.data[i] != 0) {
                result.data.erase(result.data.begin() + i + 1, result.data.end());
                break;
            }
        }
        return result;
    }
    infInt operator+(const infInt& other) const {
        if(NaN || other.NaN) return NONE;
        if(sign ^ other.sign) return Sub(*this, other);
        return Add(*this, other);
    }
    infInt operator-(const infInt& other) const {
        if(NaN || other.NaN) return NONE;
        if(sign ^ other.sign) return Add(*this, other);
        return Sub(*this, other);
    }
    infInt operator*(const infInt& other) const {
        if(NaN || other.NaN) return NONE;
        if(this->isZero() || other.isZero()) return ZERO;
        const infInt* A = this;
        const infInt* B = &other;
        int Alen = A->data.size();
        int Blen = B->data.size();
        infInt result;
        result.data.resize(Alen + Blen);
        result.sign = A->sign ^ B->sign;
        for(int i = 0; i < Alen; i++) {
            uint64_t carry = 0;
            for(int j = 0; j < Blen; j++) {
                uint64_t prod = (uint64_t)A->data[i] * B->data[j] + result.data[i + j] + carry;
                result.data[i + j] = (uint32_t)prod;
                carry = prod >> 32;
            }
            if(carry) result.data[i + Blen] = carry;
        }
        if(result.data.size() > 1 && result.data.back() == 0) {
            result.data.pop_back();
        }
        return result;
    }
    infInt Divide(const infInt& divisor, infInt& remainder) const {
        if(divisor.isZero() || NaN || divisor.NaN) {
            remainder = NONE;
            return NONE;
        }
        if(this->isZero()) {
            remainder = ZERO;
            return ZERO;
        }
        if(CompareAbs(*this, divisor) == 1) {
            remainder = *this;
            remainder.sign = sign;
            return ZERO;
        }
        infInt result(0U);
        result.sign = sign ^ divisor.sign;
        if(divisor.data.size() == 1) {
            int len = data.size();
            result.data.resize(len);
            uint64_t div = divisor.data[0];
            uint64_t rem = 0;
            for(int i = len - 1; i >= 0; i--) {
                uint64_t cur = (rem << 32) | data[i];
                result.data[i] = (uint32_t)(cur / div);
                rem = cur % div;
            }
            if(result.data.size() > 1 && result.data.back() == 0) {
                result.data.pop_back();
            }
            remainder.data = std::vector<uint32_t>{(uint32_t)rem};
            remainder.sign = sign;
            return result;
        }
        int n = divisor.data.size();
        int m = this->data.size() - n;

        int shift = __builtin_clz(divisor.data.back());
        infInt A = *this << shift;
        infInt B = divisor << shift;
        if(A.data.size() == m + n) A.data.push_back(0U);
        result.data.resize(m + 1);

        uint64_t base = 1ULL << 32;
        for(int i = m; i >= 0; i--) {
            uint64_t num = ((uint64_t)A.data[i + n] << 32) | A.data[i + n - 1];
            uint64_t div = B.data[n - 1];
            uint64_t quo = num / div;
            uint64_t rem = num % div;

            while(quo >= base || quo * B.data[n - 2] > ((rem << 32) | A.data[i + n - 2])) {
                quo--;
                rem += div;
                if(rem >= base) break;
            }

            uint64_t borrow = 0;
            for(int j = 0; j < n; j++) {
                uint64_t prod = quo * B.data[j] + borrow;
                borrow = prod >> 32;
                bool under = __builtin_sub_overflow(A.data[i + j], (uint32_t)prod, &A.data[i + j]);
                if(under) borrow++;
            }
            bool under = __builtin_sub_overflow(A.data[i + n], (uint32_t)borrow, &A.data[i + n]);
            if(under) {
                quo--;
                uint32_t carry = 0;
                for(int j = 0; j < n; j++) {
                    uint32_t sum;
                    bool over1 = __builtin_add_overflow(A.data[i + j], B.data[j], &sum);
                    bool over2 = __builtin_add_overflow(sum, carry, &A.data[i + j]);
                    carry = over1 || over2;
                }
                A.data[i + n] += (uint32_t)carry;
            }
            result.data[i] = quo;
        }
        while(result.data.size() > 1 && result.data.back() == 0) {
            result.data.pop_back();
        }
        while(A.data.size() > 1 && A.data.back() == 0) {
            A.data.pop_back();
        }
        remainder = A >> shift;
        remainder.sign = sign;
        if(remainder.isZero()) remainder = ZERO;
        return result;
    }
    infInt operator/(const infInt& other) const {
        infInt remainder;
        return this->Divide(other, remainder);
    }
    infInt operator%(const infInt& other) const {
        infInt remainder;
        this->Divide(other, remainder);
        return remainder;
    }
    infInt& operator+=(const infInt& other) {
        *this = *this + other;
        return *this;
    }
    infInt& operator-=(const infInt& other) {
        *this = *this - other;
        return *this;
    }
    infInt& operator*=(const infInt& other) {
        *this = *this * other;
        return *this;
    }
    infInt& operator/=(const infInt& other) {
        *this = *this / other;
        return *this;
    }
    infInt& operator%=(const infInt& other) {
        *this = *this % other;
        return *this;
    }
    infInt& operator&=(const uint64_t& other) {
        *this = *this & other;
        return *this;
    }
    infInt& operator|=(const uint64_t& other) {
        *this = *this | other;
        return *this;
    }
    infInt& operator^=(const uint64_t& other) {
        *this = *this ^ other;
        return *this;
    }
    infInt& operator>>=(const uint64_t& other) {
        *this = *this >> other;
        return *this;
    }
    infInt& operator<<=(const uint64_t& other) {
        *this = *this << other;
        return *this;
    }
    std::string to_string() const {
        if(this->isNaN()) return "NaN";
        if(this->isZero()) return "0";
        std::string result = "";
        if(this->isNeg()) result += '-';
        infInt temp = *this;
        temp.sign = 0;
        std::vector<uint32_t> ret;
        while(temp > ZERO) {
            infInt rem;
            temp = temp.Divide(infInt(1000000000U), rem);
            ret.push_back(rem.data[0]);
        }
        std::reverse(ret.begin(), ret.end());
        bool first = true;
        for(uint32_t x : ret) {
            std::string temp = std::to_string(x);
            if(!first) {
                result += std::string(9 - temp.length(), '0');
            }
            result += temp;
            first = false;
        }
        return result;
    }
    friend std::ostream& operator<<(std::ostream& os, const infInt& num) {
        if(num.isNaN()) {
            os << "NaN";
            return os;
        }
        if(num.isZero()) {
            os << '0';
            return os;
        }
        if(num.isNeg()) {
            os << '-';
        }
        infInt temp = num;
        temp.sign = 0;
        std::vector<uint32_t> ret;
        while(temp > ZERO) {
            infInt rem;
            temp = temp.Divide(infInt(1000000000U), rem);
            ret.push_back(rem.data[0]);
        }
        std::reverse(ret.begin(), ret.end());
        bool first = true;
        for(uint32_t x : ret) {
            std::string temp = std::to_string(x);
            if(!first) {
                os << std::string(9 - temp.length(), '0');
            }
            os << temp;
            first = false;
        }
        return os;
    }
public:
    infInt() : data(std::vector<uint32_t>{0}), sign(false), NaN(false) {}
    infInt(int32_t x) : sign(x < 0), NaN(false) {
        uint32_t abs_x = x < 0 ? (uint32_t)(-(x + 1)) + 1 : (uint32_t)(x);
        data = std::vector<uint32_t>{abs_x};
        trim();
    }
    infInt(uint32_t x) : data(std::vector<uint32_t>{x}), sign(false), NaN(false) { trim(); }
    infInt(int64_t x) : sign(x < 0), NaN(false) {
        uint64_t abs_x = x < 0 ? (uint64_t)(-(x + 1)) + 1 : (uint64_t)(x);
        data = std::vector<uint32_t>{(uint32_t)abs_x, (uint32_t)(abs_x >> 32)};
        trim();
    }
    infInt(uint64_t x) : data(std::vector<uint32_t>{(uint32_t)x, (uint32_t)(x >> 32)}), sign(false), NaN(false) { trim(); }
    infInt(__int128 x) : sign(x < 0), NaN(false) {
        unsigned __int128 abs_x = x < 0 ? (unsigned __int128)(-(x + 1)) + 1 : (unsigned __int128)(x);
        data = std::vector<uint32_t>{(uint32_t)abs_x, (uint32_t)(abs_x >> 32), (uint32_t)(abs_x >> 64), (uint32_t)(abs_x >> 96)};
        trim();
    }
    infInt(unsigned __int128 x) : data(std::vector<uint32_t>{(uint32_t)x, (uint32_t)(x >> 32), (uint32_t)(x >> 64), (uint32_t)(x >> 96)}), sign(false), NaN(false) { trim(); }
    infInt(std::string x) {
        if(x.empty()) {
            *this = NONE;
            return;
        }
        bool negative = (x[0] == '-');
        int startpos = (negative || x[0] == '+');
        int len = x.length();
        if(startpos == len) {
            *this = NONE;
            return;
        }
        infInt res(0U);
        for(int i = startpos; i < len; i++) {
            if(x[i] < '0' || x[i] > '9') {
                *this = NONE;
                return;
            }
            res = res * 10 + (uint32_t)(x[i] - '0');
        }
        if(negative) res.negate();
        res.trim();
        *this = res;
    }
    infInt(char x, char y) : data(std::vector<uint32_t>()), sign(false), NaN(true) {}
    explicit operator int32_t() const {
        if(NaN || data.empty()) return 0;
        uint32_t ret = data[0];
        if(sign) ret = -ret;
        int32_t signed_ret;
        std::memcpy(&signed_ret, &ret, sizeof(signed_ret));
        return signed_ret;
    }
    explicit operator uint32_t() const {
        if(NaN || data.empty()) return 0;
        return (sign ? -data[0] : data[0]);
    }
    explicit operator int64_t() const {
        if(NaN || data.empty()) return 0;
        uint64_t ret = data[0];
        if(data.size() > 1) ret |= (uint64_t)data[1] << 32;
        if(sign) ret = -ret;
        int64_t signed_ret;
        std::memcpy(&signed_ret, &ret, sizeof(signed_ret));
        return signed_ret;
    }
    explicit operator uint64_t() const {
        if(NaN || data.empty()) return 0;
        uint64_t ret = data[0];
        if(data.size() > 1) ret |= (uint64_t)data[1] << 32;
        return (sign ? -ret : ret);
    }
    explicit operator __int128() const {
        if(NaN || data.empty()) return 0;
        unsigned __int128 ret = data[0];
        if(data.size() > 1) ret |= (unsigned __int128)data[1] << 32;
        if(data.size() > 2) ret |= (unsigned __int128)data[2] << 64;
        if(data.size() > 3) ret |= (unsigned __int128)data[3] << 96;
        if(sign) ret = -ret;
        __int128 signed_ret;
        std::memcpy(&signed_ret, &ret, sizeof(signed_ret));
        return signed_ret;
    }
    explicit operator unsigned __int128() const {
        if(NaN || data.empty()) return 0;
        unsigned __int128 ret = data[0];
        if(data.size() > 1) ret |= (unsigned __int128)data[1] << 32;
        if(data.size() > 2) ret |= (unsigned __int128)data[2] << 64;
        if(data.size() > 3) ret |= (unsigned __int128)data[3] << 96;
        return (sign ? -ret : ret);
    }
    const static infInt NONE;
    const static infInt ZERO;
};

const infInt infInt::NONE = infInt(' ', ' ');
const infInt infInt::ZERO = infInt(0U);