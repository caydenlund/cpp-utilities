/**
 * @file fff.cpp
 * @brief `fff`: Find my freaking file/folder.
 * @details Recursively searches for a file or place
 *     in a breadth-first manner.
 *     Supports the following options:
 *
 *     "-name <name>"   --  Search for a file or place with the given name
 *                          or pattern.
 *     "-iname <name>"  --  Same as above, but ignores case.
 *     "-type <t>"      --  Searches for a place of the given type.
 *                          Types:
 *                              - f/file  -  file
 *                              - d/dir   -  directory
 *     "-mindepth <d>"  --  Begin the search at the given depth.
 *     "-maxdepth <d>"  --  End the search at the given depth.
 *
 */

// TODO: arg.h.
// TODO: Multithreading.

// TODO: Remove these temporary definitions.
// #define dbg std::cout << "[DEBUG]  "
// #define red "\033[0;31m"
// #define green "\033[0;32m"
// #define blue "\033[0;34m"
// #define reset_code "\033[0m"

#include <filesystem>
#include <iostream>
#include <list>
#include <regex>
#include <string>
#include <unordered_set>

/**
 * @brief Represents a single place to search, including a depth and a
 * place.
 *
 */
struct place {
    /**
     * @brief The place represented by this struct.
     *
     */
    std::string path;

    /**
     * @brief The depth in the search space of this `place`.
     *
     */
    unsigned int depth;

#pragma clang diagnostic push
#pragma ide diagnostic ignored "google-explicit-constructor"
    /**
     * @brief Constructor.
     *
     * @param path The path that this `place` represents.
     * @param depth The depth of this `place` in the search space.
     */
    place(std::string path, unsigned int depth = 0)
        : path(std::move(path)), depth(depth) {}
#pragma clang diagnostic pop
};

/**
 * @brief Main program entry point.
 *
 * @param argc The count of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return 0 on success; 1 on failure.
 */
int main(int argc, char **argv) {
    /**
     * @brief The type of thing that we're looking for.
     *
     */
    enum search_types { file, directory, any };
    search_types search_type = search_types::any;

    /**
     * @brief Whether we're ignoring case in our search.
     *
     */
    bool ignore_case;

    /**
     * @brief The name or pattern to match.
     *
     */
    std::string name;

    /**
     * @brief The places_queue to search.
     *
     */
    std::list<place> places_queue;

    /**
     * @brief A set of the searched directories.
     * @details Used to avoid searching directories twice.
     *
     */
    std::unordered_set<std::string> searched;

    /**
     * @brief The minimum depth to search.
     *
     */
    unsigned int min_depth = 0;

    /**
     * @brief The maximum depth to search.
     *
     */
    unsigned int max_depth = -1;

    // Process arguments.
    for (unsigned int index = 1; index < argc; index++) {
        std::string arg(argv[index]);

        if (arg == "-name") {
            ignore_case = false;
            name = std::string(argv[++index]);
        } else if (arg == "-iname") {
            ignore_case = true;
            name = std::string(argv[++index]);
        } else if (arg == "-type") {
            std::string next(argv[++index]);
            if (next == "f" || next == "file") {
                search_type = search_types::file;
            } else if (next == "d" || next == "dir" || next == "place") {
                search_type = search_types::directory;
            } else {
                std::cerr << "Usage:" << std::endl
                          << "    " << argv[0] << "-type <t> [...]" << std::endl
                          << "        Where <t> is one of: {f, file, d, dir, "
                             "place}."
                          << std::endl;
                exit(1);
            }
        } else if (arg == "-mindepth") {
            std::string next(argv[++index]);
            min_depth = std::stoi(next);
        } else if (arg == "-maxdepth") {
            std::string next(argv[++index]);
            max_depth = std::stoi(next);
        } else {
            // Otherwise, it's one of the "root" files or directories to search.
            places_queue.emplace_back(arg);
            searched.insert(arg);
        }
    }

    // Begin search.
    while (!places_queue.empty()) {
        place current_place = places_queue.front();
        places_queue.pop_front();

        try {
            bool is_directory =
                std::filesystem::is_directory(current_place.path);

            // If this is a directory, and we're not at our max depth,
            // add this place's children to the queue.
            if (is_directory && current_place.depth < max_depth) {
                // Iterate over all children.
                std::filesystem::directory_iterator it(current_place.path);
                for (const auto &entry : it) {
                    // Skip directories that we've already searched.
                    if (searched.count(entry.path()) > 0) continue;

                    // Add it to the queue and the `searched` tracker.
                    place new_place(entry.path(), current_place.depth + 1);
                    places_queue.emplace_back(new_place);
                    searched.insert(entry.path());
                }
            }

            // Next, if we haven't reached our min depth yet,
            // we don't need to perform the name checks.
            if (current_place.depth < min_depth) continue;

            // If we're not searching for this kind of item, then continue.
            if ((is_directory && search_type == search_types::file) ||
                (!is_directory && search_type == search_types::directory))
                continue;

            // Now, we check for a name match.
            std::string short_name =
                std::filesystem::path(current_place.path).filename();

            // TODO: Patterns.
            // TODO: Regular expressions.
            if (short_name == name) {
                std::cout << current_place.path << std::endl;
                continue;
            }
        } catch (std::filesystem::filesystem_error &error) {
            // TODO: Formatting.
            std::cerr << error.path1() << ": " << error.what() << std::endl;

            continue;
        }
    }

    return 0;
}
