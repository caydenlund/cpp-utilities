/**
 * @brief Header for the `convert` class.
 * @details Used to convert a number from one format to another.
 *
 * @author Cayden Lund
 * @date 25 August 2022
 *
 * @copyright MIT License <https://opensource.org/licenses/MIT>.
 */

#ifndef CONVERT_HPP
#define CONVERT_HPP

#include <string>

/**
 * @brief Converts numbers from one format to another.
 *
 */
class convert {
   private:
    /**
     * @brief The number that this object represents, saved in binary notation.
     *
     */
    std::string _binary_num;

    /**
     * @brief Saves a number given in binary notation.
     *
     * @param binary_number The binary number to save.
     */
    void save_binary(const std::string &binary_number);

    /**
     * @brief Saves a number given in decimal notation.
     *
     * @param decimal_number The decimal number to save.
     */
    void save_decimal(const std::string &decimal_number);

    /**
     * @brief Saves a number given in hexadecimal notation.
     *
     * @param hex_number The hexadecimal number to save.
     */
    void save_hex(const std::string &hex_number);

    /**
     * @brief Saves a number given in octal notation.
     *
     * @param octal_number The octal number to save.
     */
    void save_octal(const std::string &octal_number);

   public:
    /**
     * @brief Defines the "types" of numbers that can be represented.
     *
     */
    enum types { binary, decimal, hex, octal };

    /**
     * @brief Given a string and a type of number, constructs a new `convert`
     *     object.
     *
     * @param num The input number to convert.
     * @param type The type of number given.
     */
    convert(std::string num, types type);

    /**
     * @brief Returns a string of the number given in binary format.
     *
     */
    std::string as_binary();

    /**
     * @brief Returns a string of the number given in hexadecimal format.
     *
     */
    std::string as_hex();

    /**
     * @brief Returns a string of the number given in decimal format.
     *
     */
    std::string as_decimal();

    /**
     * @brief Returns a string of the number given in octal format.
     *
     */
    std::string as_octal();
};

#endif  // CONVERT_HPP
