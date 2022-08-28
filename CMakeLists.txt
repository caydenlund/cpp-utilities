cmake_minimum_required(VERSION 3.22)
project(cpp_utilities)

set(CMAKE_CXX_STANDARD 17)

include_directories(.)
#include_directories(arg.h)
#include_directories(assertions.h)
include_directories(convert)
include_directories(fff)
#include_directories(huge.h)


#  //=========\\
#  ||  arg.h  ||
#  \\=========//


#  //================\\
#  ||  assertions.h  ||
#  \\================//


#  //===========\\
#  ||  Convert  ||
#  \\===========//

add_executable(convert_binary_to_decimal convert/binary_to_decimal.cpp convert/convert.cpp)
add_executable(convert_binary_to_hex convert/binary_to_hex.cpp convert/convert.cpp)
add_executable(convert_binary_to_octal convert/binary_to_octal.cpp convert/convert.cpp)

add_executable(convert_decimal_to_binary convert/decimal_to_binary.cpp convert/convert.cpp)
add_executable(convert_decimal_to_hex convert/decimal_to_hex.cpp convert/convert.cpp)
add_executable(convert_decimal_to_octal convert/decimal_to_octal.cpp convert/convert.cpp)

add_executable(convert_hex_to_binary convert/hex_to_binary.cpp convert/convert.cpp)
add_executable(convert_hex_to_decimal convert/hex_to_decimal.cpp convert/convert.cpp)
add_executable(convert_hex_to_octal convert/hex_to_octal.cpp convert/convert.cpp)

add_executable(convert_octal_to_binary convert/octal_to_binary.cpp convert/convert.cpp)
add_executable(convert_octal_to_decimal convert/octal_to_decimal.cpp convert/convert.cpp)
add_executable(convert_octal_to_hex convert/octal_to_hex.cpp convert/convert.cpp)


#  //=======\\
#  ||  fff  ||
#  \\=======//

add_executable(fff fff/fff.cpp)


#  //==========\\
#  ||  huge.h  ||
#  \\==========//

#add_executable(test_huge huge.h/huge.test.cpp)

