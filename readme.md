# C++ Utilities

This repository contains several miscellaneous utility libraries and applications.

## `args`: A command line argument parser.

A single header file is included (`args/args.hpp`).
This header file defines a new class, `util::args`, which parses the command-line arguments.

The arguments are inspired by the POSIX and GNU argument syntax, according to the following rules:

* There are four types of arguments:
    1. **The program name**. This is, of course, the first argument in the array.
       In a call to `clang++ file.cpp -o exec`, the program name is `clang++`.
    2. **Options**. There are two kinds of options: short (single-character) and long (multi-character).
       In a call to `clang++ file.cpp -o exec`, `-o` is a short option.
       Short options may only be made of letters (uppercase or lowercase) or numbers.
       Long options may only be made of letters, numbers, hyphens, and underscores,
       and they may not start with a hyphen.
       Options are case-sensitive.
    3. **Option-arguments**. These are arguments of an option.
       In a call to `clang++ file.cpp -o exec`, `exec` is an option-argument (i.e., the argument of the option `-o`).
    4. **Operands**. These are arguments of the main program (and not, that is, arguments of an option).
       In a call to `clang++ file.cpp -o exec`, `file.cpp` is an operand.
    5. **Double-hyphen**. This is a special kind of argument that doesn't fit into the above categories.
       All following arguments are operands, even if they start with hyphens.
       In a call to `./exec -a -b -c -- -d -e -f`, `-d`, `-e`, and `-f` are operands, not options.
* Short options are always preceded by a single hyphen. (E.g., `./exec -a -b -c`.)
* Short options may be combined.
  Note that as soon as there is a short option that can accept an option-argument,
  the rest of the argument is considered the option-argument.
  (E.g., `./exec -a -b -c` is equivalent to `./exec -abc`, provided `-a` and `-b` don't accept option-arguments.)
* There are three ways to specify option-arguments:
    1. For short options only, the option-argument may be specified directly after the option, with no delimiter.
       In a call to `./exec -n5`, `5` is the option-argument of `-n`.
    2. The option-argument may be specified with a space between the option and the option-argument.
       In a call to `./exec --number 5`, `5` is the option-argument of `--number`.
       Similarly, in a call to `./exec -an 5`, `5` is the option-argument of `-n`.
    3. The option-argument may be specified with the equals character between the option and the option-argument.
       In a call to `./exec --number=5`, `5` is the option-argument of `--number`.
       Similarly, in a call to `./exec -an=5`, `5` is the option-argument of `-n`.

Usage is simple. First, instantiate the object with a list of all options that take option-arguments.

```c++
int main(int argc, char** argv) {
    const std::vector<std::string> options_with_args {
        "-n", "--number",
        "-i", "--input"
    };

    util::args parser(options_with_args);

    //  . . .
}
```

Next, use it to parse the argument array.

```c++
int main(int argc, char** argv) {
    //  . . .

    parser.parse(argc, argv);

    //  . . .
}
```

After that, you can count the number of times an option has been specified
(or check for the presence of the option) using the subscript `[]` operator.

```c++
int main(int argc, char** argv) {
    //  . . .

    if (parser["-h"] || parser["--help"]) {
        print_help_message();
        return 0;
    }

    std::cout << "The option `-n` has been used " << parser["-n"] << " times." << std::endl;

    //  . . .
}
```

You can get a vector of all the option-arguments used for a given option with the function call `()` operator.

```c++
int main(int argc, char** argv) {
    //  . . .

    if (parser["-n"]) {
        //  If the program has been called as `./exec -n 1 -n 2 -n 3`,
        //  then this will return the vector `{"1", "2", "3"}`.
        const std::vector<std::string> all_numbers = parser("-n");

        for (const std::string& number : all_numbers) {
            std::cout << number << std::endl;
        }
    }

    //  . . .
}
```

Access the operands using the subscript `[]` operator with an integer index.
You can also use the `size()` method to get the number of operands.

```c++
int main(int argc, char** argv) {
    //  . . .

    for (unsigned int arg_index = 0; arg_index < parser.size(); ++arg_index) {
        std::cout << "Arg " << arg_index << ": " << parser[arg_index] << std::endl;
    }

    //  . . .
}
```

You can use the `begin()` and `end()` methods to iterate over the operands.

```c++
int main(int argc, char** argv) {
    //  . . .

    const std::vector<std::string> operands(parser.begin(), parser.end());

    for (const std::string& operand : parser) {
        std::cout << "Operand: " << operand << std::endl;
    }

    //  . . .
}
```

The `name()` method returns the program name.

```c++
int main(int argc, char** argv) {
    //  . . .

    const std::string program_name = parser.name();

    //  . . .
}
```

There is also a file `args/args.tests.cpp` which contains unit tests for the class.

## `fff`: Find my freaking file!

This utility finds files or directories with names that match the given pattern.
Compare it to GNU's `find`.
The search is done in a breadth-first manner.

## `testing`: A simple unit test suite.

A single header file is included (`testing/testing.hpp`).
This header file defines several top-level functions for unit testing.

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
A test (`test_t`) is a pair of a function with no arguments and a void return value, and a string
name (`std::tuple<std::function<void()>, std::string>`).
A test fails when an exception is thrown; it passes otherwise.
This function prints output to the console.
