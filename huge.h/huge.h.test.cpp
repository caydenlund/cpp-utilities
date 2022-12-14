
#include "huge.h"

#include "assertions.h/assertions.h"

/**
 * @brief Ensures that class constructors don't throw an exception.
 *
 */
#pragma clang diagnostic push
#pragma ide diagnostic ignored "performance-unnecessary-copy-initialization"
void test_constructors_noexcept() {
    huge new_empty;
    huge new_int(1);
    huge new_double(1.0);
    huge new_long(1000000000000L);
    huge new_string("1000000");
    huge new_huge_string("1000000000000000000000");
    huge new_implicit = 1;
    huge new_copy(new_huge_string);
}
#pragma clang diagnostic pop

/**
 * @brief Ensures that all the constructors that should produce the same result
 * do so.
 *
 */
void test_constructors_equivalence() {
    huge new_int(5);
    huge new_double(5.0);
    huge new_long(5L);
    huge new_string("5");
    huge new_implicit = 5;
    huge new_copy(new_string);

    assert::are_equal(new_int, new_double, "Integer vs. double equivalence");
    assert::are_equal(new_int, new_double, "Integer vs. double equivalence");
    assert::are_equal(new_int, new_double, "Integer vs. double equivalence");
    assert::are_equal(new_int, new_double, "Integer vs. double equivalence");
    assert::are_equal(new_int, new_double, "Integer vs. double equivalence");
    assert::are_equal(new_int, new_double, "Integer vs. double equivalence");
}

int main() {
    test_constructors_noexcept();

    return 0;
}
