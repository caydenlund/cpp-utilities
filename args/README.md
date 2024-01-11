# `args`: A command-line argument parser ![Status: ready for use](https://img.shields.io/badge/Status-Ready_for_use-green?style=flat&labelColor=%232c3e50&color=%2327ae60)

Are you tired of parsing command-line arguments?
This module makes it dead simple.

## Features

* Options that accept arguments (`-n3`, `--output file.txt`, `--foo=bar`).
* Arguments that can appear multiple times.
  For example, you might want to count
  the number of `-v` options to determine the verbosity level.
* Concatenation of multiple single-letter options (`-abc` is equivalent to `-a -b -c`).
* Double-hyphen: all following arguments are treated as program operands.

## Concepts

The argument style is inspired by the POSIX and GNU argument syntax,
according to the following rules:

* There are five types of arguments:
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

## Usage

First, instantiate the object with a list of all options that take option-arguments.

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

Alternatively, you can use the `begin()` and `end()` methods or a range-based `for` loop
to iterate over the operands.

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

Finally, the `name()` method returns the program name.

```c++
int main(int argc, char** argv) {
    //  . . .

    const std::string program_name = parser.name();

    //  . . .
}
```
