# `testing`: A simple unit test suite

Existing unit testing suites can be a hassle to set up,
especially when portability is a concern.
This library makes it easy:
include this single module in your project, and it just works.

First, several assertions are defined:

* `assert_eq<obj_t>(obj_t a, obj_t b, std::string message)`:
  Asserts that object `a` is equal (`==`) to object `b`.
  Throws an exception with the given message otherwise.
* `assert_neq<obj_t>(obj_t a, obj_t b, std::string message)`:
  Asserts that object `a` is not equal (`!=`) to object `b`.
  Throws an exception with the given message otherwise.
* `assert_leq<obj_t>(obj_t a, obj_t b, std::string message)`:
  Asserts that object `a` is less than or equal (`<=`) to object `b`.
  Throws an exception with the given message otherwise.
* `assert_geq<obj_t>(obj_t a, obj_t b, std::string message)`:
  Asserts that object `a` is greater than or equal (`>=`) to object `b`.
  Throws an exception with the given message otherwise.
* `assert_lt<obj_t>(obj_t a, obj_t b, std::string message)`:
  Asserts that object `a` is less (`<`) than object `b`.
  Throws an exception with the given message otherwise.
* `assert_gt<obj_t>(obj_t a, obj_t b, std::string message)`:
  Asserts that object `a` is greater (`>`) than object `b`.
  Throws an exception with the given message otherwise.
* `assert_true(bool cond, std::string message)`:
  Asserts that the given condition is true.
  Throws an exception with the given message otherwise.
* `assert_false(bool cond, std::string message)`:
  Asserts that the given condition is false.
  Throws an exception with the given message otherwise.
* `assert_throws<err_t>(std::function<void()> func, std::string message)`:
  Asserts that the given function throws the given type of exception.
  The exception type defaults to `std::exception`.
* `assert_throws<err_t>(std::function<void()> func, std::string expected_message, std::string message)`:
  Asserts that the given function throws the given type of exception.
  The exception type defaults to `std::exception`.
  When the exception is thrown, asserts that the exception's message (`::what()`) matches the given expected message.

Next, there is a function defined (`testing::run_all(const std::vector<test_t>& tests)`) for running all given tests.
A test (`test_t`) is a pair of a function with no arguments and a void return value,
and a string name (`std::tuple<std::function<void()>, std::string>`).
A test fails when an exception is thrown; it passes otherwise.
This function prints output to the console.
