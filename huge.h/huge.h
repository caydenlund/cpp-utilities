/**
 * @file huge.h
 * @author Cayden Lund (cayden.lund@utah.edu)
 * @brief Header-only library for handling huge numbers.
 * @date 24 July 2022
 *
 * @copyright Copyright (c) 2022 by Cayden Lund.
 */

#pragma clang diagnostic push
#pragma ide diagnostic ignored "google-explicit-constructor"
#pragma ide diagnostic ignored "cert-dcl21-cpp"

#ifndef HUGE_H
#define HUGE_H

#include <stdexcept>
#include <string>

/**
 * @brief Represents a huge, whole, positive number.
 *
 */
class huge {
   public:
    /*
        //================\\
        ||  Constructors  ||
        \\================//
     */

    /**
     * @brief Zero-argument constructor.
     *
     */
    huge();

    /**
     * @brief Constructs a new `huge` object from a string.
     *
     * @param num The number to represent.
     */
    huge(const std::string& num);

    /**
     * @brief Constructs a new `huge` object from a `const char*`.
     *
     * @param num The number to represent.
     */
    huge(const char* num);

    /**
     * @brief Constructs a new `huge` object from a number.
     *
     * @tparam T The type of the number.
     * @param num The number to represent.
     */
    template <typename T>
    huge(T num);

    /**
     * @brief Copy constructor.
     *
     * @param other The `huge` object to copy.
     */
    huge(const huge& other);

    /**
     * @brief Move constructor.
     *
     * @param other The `huge` object to move.
     */
    huge(huge&& other) noexcept;

    /**
     * @brief Destructor.
     *
     */
    ~huge();

    /*
        //=============\\
        ||  Operators  ||
        \\=============//
     */

    /**
     * @brief Assignment operator.
     *
     * @param other The `huge` object to copy.
     * @return The `huge` object.
     */
    huge& operator=(const huge& other);

    /**
     * @brief Move assignment operator.
     *
     * @param other The `huge` object to move.
     * @return The `huge` object.
     */
    huge& operator=(huge&& other) noexcept;

    /**
     * @brief Addition operator.
     *
     * @param other The `huge` object to add.
     * @return The sum of the two `huge` objects.
     */
    huge operator+(const huge& other) const;

    /**
     * @brief Immediate addition operator.
     *
     * @param other The `huge` object to add.
     * @return This.
     */
    huge& operator+=(const huge& other);

    /**
     * @brief Subtraction operator.
     *
     * @param other The `huge` object to subtract.
     * @return The difference of the two `huge` objects.
     */
    huge operator-(const huge& other) const;

    /**
     * @brief Immediate subtraction operator.
     *
     * @param other The `huge` object to subtract.
     * @return This.
     */
    huge& operator-=(const huge& other);

    /**
     * @brief Multiplication operator.
     *
     * @param other The `huge` object by which to multiply.
     * @return The product of the two `huge` objects.
     */
    huge operator*(const huge& other) const;

    /**
     * @brief Immediate multiplication operator.
     *
     * @param other The `huge` object by which to multiply.
     * @return This.
     */
    huge& operator*=(const huge& other);

    /**
     * @brief Division operator.
     *
     * @param other The `huge` object by which to divide.
     * @return The quotient of the two `huge` objects.
     */
    huge operator/(const huge& other) const;

    /**
     * @brief Immediate division operator.
     *
     * @param other The `huge` object by which to divide.
     * @return This.
     */
    huge& operator/=(const huge& other);

    /**
     * @brief Modulus operator.
     *
     * @param other The `huge` object to mod.
     * @return The remainder of the two `huge` objects.
     */
    huge operator%(const huge& other) const;

    /**
     * @brief Immediate modulus operator.
     *
     * @param other The modular quotient.
     * @return This.
     */
    huge& operator%=(const huge& other);

    /**
     * @brief Prefix increment operator.
     *
     * @return The `huge` object.
     */
    huge& operator++();

    /**
     * @brief Postfix increment operator.
     *
     * @return The `huge` object.
     */
    huge operator++(int) &;

    /**
     * @brief Prefix decrement operator.
     *
     * @return The `huge` object.
     */
    huge& operator--();

    /**
     * @brief Postfix decrement operator.
     *
     * @return The `huge` object.
     */
    huge operator--(int) &;

    /**
     * @brief Equality operator.
     *
     * @param other The `huge` object to compare.
     * @return true if the two `huge` objects are equal.
     */
    bool operator==(const huge& other) const;

    /**
     * @brief Inequality operator.
     *
     * @param other The `huge` object to compare.
     * @return true if the two `huge` objects are not equal.
     */
    bool operator!=(const huge& other) const;

    /**
     * @brief Less than operator.
     *
     * @param other The `huge` object to compare.
     * @return true if the `huge` object is less than the other.
     */
    bool operator<(const huge& other) const;

    /**
     * @brief Less than or equal operator.
     *
     * @param other The `huge` object to compare.
     * @return true if the `huge` object is less than or equal to the other.
     */
    bool operator<=(const huge& other) const;

    /**
     * @brief Greater than operator.
     *
     * @param other The `huge` object to compare.
     * @return true if the `huge` object is greater than the other.
     */
    bool operator>(const huge& other) const;

    /**
     * @brief Greater than or equal operator.
     *
     * @param other The `huge` object to compare.
     * @return true if the `huge` object is greater than or equal to the other.
     */
    bool operator>=(const huge& other) const;

    /**
     * @brief Stream insertion operator.
     * @details TODO: Implement.
     *
     * @param os The output stream.
     * @param num The `huge` object to insert into the stream.
     * @return A reference to the output stream.
     */
    friend std::ostream& operator<<(std::ostream& out, const huge& num);

    /**
     * @brief Stream extraction operator.
     * @details TODO: Implement.
     *
     * @param in The input stream.
     * @param num The `huge` object to extract from the stream.
     * @return A reference to the input stream.
     */
    friend std::istream& operator>>(std::istream& in, huge& num);

   private:
    /**
     * @brief The number of digits in the number.
     *
     */
    size_t _num_digits;

    /**
     * @brief Array of the digits of the number.
     * @details Least-significant digit is first.
     *
     */
    unsigned char* _digits;

    /**
     * @brief Trims leading zeroes.
     * @details (I.e., shortens the array.)
     *
     */
    void trim();
};

huge::huge() {
    this->_num_digits = 1;
    this->_digits = new unsigned char[1]{0};
}

huge::huge(const std::string& num)
    : _num_digits(num.size()), _digits(new unsigned char[num.size()]) {
    for (size_t index = 0; index < this->_num_digits; index++) {
        unsigned char digit = num[index] - '0';
        if (digit > 9) {
            throw std::invalid_argument("Invalid character: " +
                                        std::string(1, num[index]));
        }

        this->_digits[this->_num_digits - index - 1] = digit;
    }
}

huge::huge(const char* num) : huge(std::string(num)) {}

template <typename T>
huge::huge(T num) : huge(std::to_string((unsigned long long)num)) {}

huge::huge(const huge& other)
    : _num_digits(other._num_digits),
      _digits(new unsigned char[other._num_digits]) {
    for (size_t digit = 0; digit < this->_num_digits; digit++) {
        this->_digits[digit] = other._digits[digit];
    }
}

huge::huge(huge&& other) noexcept
    : _num_digits(other._num_digits), _digits(other._digits) {
    other._num_digits = 0;
    other._digits = nullptr;
}

huge::~huge() { delete[] this->_digits; }

huge& huge::operator=(const huge& other) {
    if (this == &other) return *this;

    this->_num_digits = other._num_digits;

    delete[] this->_digits;
    this->_digits = new unsigned char[other._num_digits];
    for (size_t i = 0; i < this->_num_digits; i++) {
        this->_digits[i] = other._digits[i];
    }

    return *this;
}

huge& huge::operator=(huge&& other) noexcept {
    if (this == &other) return *this;

    this->_num_digits = other._num_digits;

    delete[] this->_digits;
    this->_digits = other._digits;

    other._num_digits = 0;
    other._digits = nullptr;

    return *this;
}

huge huge::operator+(const huge& other) const {
    // Addition is done within the `result` object, which is returned.
    huge result;
    // In the end, `result._num_digits <= max(this, other) + 1`.
    // E.g.,  `99 + 999    =  1,098`.
    //     `max(2,  3) + 1 =    4`
    result._num_digits = std::max(this->_num_digits, other._num_digits) + 1;
    result._digits = new unsigned char[result._num_digits];
    // Initialize all digits to zero.
    for (size_t i = 0; i < result._num_digits; i++) {
        result._digits[i] = 0;
    }

    // Whether we are carrying 10 from the last column.
    // Eg.,   `(6 + 7)`
    //      `= 3  +  10`
    //         ^     ^
    //         |     |
    //         |     L-- Tens place.
    //         |
    //         L-- Ones place.
    bool carrying = false;

    // For each digit:
    for (size_t i = 0; i < result._num_digits; i++) {
        // This digit, or 0 (if `this` doesn't have a digit here).
        unsigned char this_digit =
            (i < this->_num_digits ? this->_digits[i] : 0);
        // The other digit, or 0 (if `other` doesn't have a digit here).
        unsigned char other_digit =
            (i < other._num_digits ? other._digits[i] : 0);

        // Perform the addition.
        unsigned char result_digit = this_digit + other_digit + carrying;

        // Update the result and the carrying value.
        carrying = (result_digit / 10 > 0);
        result._digits[i] = result_digit % 10;
    }

    // Finally, trimming off leading zeroes.
    result.trim();
    return result;
}

huge& huge::operator+=(const huge& other) { return *this = *this + other; }

huge huge::operator-(const huge& other) const {
    // Class currently doesn't support negative numbers.
    if (*this < other)
        throw std::runtime_error("Invalid subtraction: negative result.");

    // Subtraction is done within the `result` object, which is returned.
    huge result;
    // In the end, `(result._num_digits <= this->_num_digits)`.
    // E.g.,  `99 - 1 = 98`
    //        `2      = 2`
    result._num_digits = this->_num_digits;
    result._digits = new unsigned char[result._num_digits];
    // Initialize all digits to zero.
    for (size_t i = 0; i < result._num_digits; i++) {
        result._digits[i] = 0;
    }

    // Whether we are taking 10 from the next column.
    // E.g.,        `(22 - 13)`
    //      `=  (2 - 3)  +  (20 - 10)`
    //      `= (12 - 3)  +  (10 - 10)`
    //      `=     9     +      0`
    //      `=     9`           ^
    //             ^            |
    //             |            L-- Tens place.
    //             |
    //             L-- Ones place.
    // I'm not sure how to explain it in words, but hopefully that example
    // makes it at least a little more clear.
    bool carrying = false;

    // For each digit:
    for (size_t i = 0; i < result._num_digits; i++) {
        // We're guaranteed to have a digit from `this`.
        unsigned char this_digit = this->_digits[i];
        // We may not have a corresponding digit from `other`.
        unsigned char other_digit =
            (i < other._num_digits ? other._digits[i] : 0);

        // If carrying from the last subtraction, decrement.
        if (carrying) this_digit--;

        // If we need to carry 10 over, add 10.
        carrying = (this_digit < other_digit);
        if (carrying) this_digit += 10;

        result._digits[i] = this_digit - other_digit;
    }

    // Trim off leading zeroes.
    result.trim();
    return result;
}

huge& huge::operator-=(const huge& other) { return *this = *this - other; }

huge huge::operator*(const huge& other) const {
    // Multiplication is done within the `result` object, which is return.
    huge result;
    // In the end, `result._num_digits <= this + other`.
    // E.g., `99 * 999 = 98,901`.
    //       `2  +  3  =   5`
    result._num_digits = this->_num_digits + other._num_digits;
    result._digits = new unsigned char[result._num_digits];
    // Initialize all digits to zero.
    for (size_t i = 0; i < result._num_digits; i++) {
        result._digits[i] = 0;
    }

    // How much we are carrying over from one column to the next.
    // E.g.,          `9  *  99`
    //        `=  (9 * 9) + (9 * 90)`
    //        `=    (81)  + (9 * 90)`
    //        `= (1 + 80) + (9 * 90)`
    //                 ^
    //                 |
    //                 L-- Carrying 80 over to the next column.
    unsigned char carrying = 0;

    // For each digit from `this`,
    for (size_t this_index = 0; this_index < this->_num_digits; this_index++) {
        // and for each digit from `other`:
        for (size_t other_index = 0; other_index < other._num_digits;
             other_index++) {
            // The index in the resulting array is the sum of the two indices.
            size_t result_index = this_index + other_index;

            unsigned char this_digit = this->_digits[this_index];
            unsigned char other_digit = other._digits[this_index];

            // Multiply and then add the carried value.
            // Also, there may be a value already in the array from
            // the last round; add that, too.
            unsigned char result_digit = this_digit * other_digit + carrying +
                                         result._digits[result_index];

            // Update the result and the carried value.
            result._digits[result_index] = result_digit % 10;
            carrying = result_digit / 10;
        }
    }

    // Trim off leading zeroes.
    result.trim();
    return result;
}

huge& huge::operator*=(const huge& other) { return *this = *this * other; }

huge huge::operator/(const huge& other) const {
    huge quotient = 0;
    huge result = *this;
    while (result >= other) {
        result -= other;
        quotient++;
    }

    quotient.trim();
    return quotient;
}

huge& huge::operator/=(const huge& other) { return *this = *this / other; }

huge huge::operator%(const huge& other) const {
    huge quotient = 0;
    huge result = *this;
    while (result >= other) {
        result -= other;
        quotient++;
    }

    result.trim();
    return result;
}

huge& huge::operator%=(const huge& other) { return *this = *this % other; }

huge& huge::operator++() { return *this += 1; }

huge huge::operator++(int) & {
    huge result = *this;
    *this += 1;
    return result;
}

huge& huge::operator--() { return *this -= 1; }

huge huge::operator--(int) & {
    huge result = *this;
    *this -= 1;
    return result;
}

bool huge::operator==(const huge& other) const {
    if (this->_num_digits != other._num_digits) return false;

    huge this_copy = *this;
    for (size_t index = 0; index < this->_num_digits; index++) {
        if (this->_digits[index] != other._digits[index]) return false;
    }

    return true;
}

bool huge::operator!=(const huge& other) const { return (!(*this == other)); }

bool huge::operator<(const huge& other) const {
    if (this->_num_digits != other._num_digits)
        return (this->_num_digits < other._num_digits);

    huge this_copy = *this;

    if (this_copy == other) return false;

    // Most significant digit first.
    for (size_t index = this->_num_digits - 1; index >= 0; index--) {
        if (this->_digits[index] != other._digits[index]) {
            return (this->_digits[index] < other._digits[index]);
        }
    }

    // Unreachable condition.
    return false;
}

bool huge::operator<=(const huge& other) const {
    if (this->_num_digits != other._num_digits)
        return (this->_num_digits < other._num_digits);

    huge this_copy = *this;
    return (this_copy == other || this_copy < other);
}

bool huge::operator>(const huge& other) const { return (!(*this <= other)); }

bool huge::operator>=(const huge& other) const { return (!(*this < other)); }

void huge::trim() {
    if (this->_digits[this->_num_digits - 1] != 0) return;

    size_t seek_index = this->_num_digits;
    while (this->_digits[--seek_index] == 0) {
    }

    size_t new_num_digits = seek_index;
    auto* new_digits = new unsigned char[new_num_digits];
    for (unsigned int i = 0; i < new_num_digits; i++) {
        new_digits[i] = this->_digits[i];
    }

    delete[] this->_digits;

    this->_num_digits = new_num_digits;
    this->_digits = new_digits;
}

/*
    //=============\\
    ||  Temporary  ||
    \\=============//
 */

// bool huge::operator<(huge const&) const { return false; }

// bool huge::operator!=(huge const&) const { return false; }

// std::ostream& operator<<(std::ostream& out, huge const&) { return out; }

#endif

#pragma clang diagnostic pop
