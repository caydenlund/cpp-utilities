/**
 * @file args.tests.cpp
 * @author Cayden Lund (cayden.lund@utah.edu)
 *
 * @brief Defines unit tests for the `util::thread_queue` class for running jobs in parallel.
 *
 * @license MIT license (https://opensource.org/licenses/MIT).
 * @copyright (c) 2023 by Cayden Lund.
 *
 */

#include "thread_queue.hpp"
#include "testing/testing.hpp"

#include <iostream>

int main() {
    util::thread_queue jobs(4, false);

    constexpr unsigned int num_runs = 100;

    for (unsigned int ind = 0; ind < num_runs; ++ind) jobs.add_job([ind]() { std::cout << ind << std::endl; });

    return 0;
}
