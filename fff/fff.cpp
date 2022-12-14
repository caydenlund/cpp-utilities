/**
 * @file fff.cpp
 * @brief `fff`: Find my file or folder.
 * @details Recursively searches for a file or place
 *     in a breadth-first manner.
 *     Supports the following options:
 *
 *     "-name <name>"     --  Search for a file or folder with the given name
 *                            or pattern.
 *     "-iname <name>"    --  Same as above, but ignores case.
 *
 *     "-regex <regex>"   --  Search for a file or folder that matches the given
 *                            regular expression.
 *     "-iregex <regex>"  --  Same as above, but ignores case.
 *
 *     "-j <threads>"     --  The number of threads to use. (Unimplemented.)
 *
 *     "-mindepth <d>"    --  Begin the search at the given depth.
 *     "-maxdepth <d>"    --  End the search at the given depth.
 *
 *     "-type <t>"        --  Searches for a place of the given type.
 *                            Types:
 *                                - f/file  -  file
 *                                - d/dir   -  directory
 *
 */

// TODO: Multithreading.

#include <filesystem>
#include <iostream>
#include <list>
#include <mutex>
#include <regex>
#include <string>
#include <thread>
#include <unordered_set>

#include "arg.h"

/**
 * @brief Represents a single place to search, including a depth and a
 * place.
 *
 */
struct place {
    /**
     * @brief The path represented by this instance.
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
     * @details Can be implicitly constructed from a string path.
     *
     * @param path The path that this `place` represents.
     * @param depth The depth of this `place` in the search space.
     */
    place(std::string path, unsigned int depth = 0)
        : path(std::move(path)), depth(depth) {}
#pragma clang diagnostic pop
};

/**
 * @brief Defines the type of thing that we're looking for (file or folder).
 *
 */
enum search_types { file, directory, any };

/**
 * @brief Translates wildcard syntax into a regular expression pattern.
 *
 * @param pattern The pattern to be translated into the regular expression.
 * @return The translated pattern.
 */
std::string wild_to_regex(const std::string &pattern) {
    std::stringstream result;
    result << "^";

    for (char c : pattern) {
        if (c == '*') {
            result << ".*";
        } else {
            result << c;
        }
    }

    result << "$";
    return result.str();
}

/**
 * @brief Examines the given place.
 *
 * @param current_place The place to examine.
 * @param places_queue The queue of places to examine.
 * @param searched An unordered set of the places we've already searched.
 * @param pattern The regular expression pattern to match.
 * @param thread_counter The number of running threads.
 * @param output_lock A mutex to protect console output.
 * @param places_queue_lock A mutex to protect the queue of places to search.
 * @param thread_counter_lock A mutex to protect the thread counter.
 * @param search_type Whether to search a file, a directory, or both.
 * @param min_depth The minimum depth to search.
 * @param max_depth The maximum depth to search.
 * @param num_threads The number of threads to use.
 */
void examine_place(const place &current_place, std::list<place> &places_queue,
                   std::unordered_set<std::string> &searched,
                   std::regex &pattern, unsigned int &thread_counter,
                   std::mutex &output_lock, std::mutex &places_queue_lock,
                   std::mutex &thread_counter_lock, search_types search_type,
                   unsigned int min_depth, unsigned int max_depth,
                   unsigned int num_threads) {
    output_lock.lock();
    std::cout << current_place.path << std::endl;
    output_lock.unlock();

    try {
        bool is_directory = std::filesystem::is_directory(current_place.path);

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

        output_lock.lock();
        std::cout << "1" << std::endl;
        output_lock.unlock();

        // Next, if we haven't reached our min depth yet,
        // we don't need to perform the name checks.
        if (current_place.depth < min_depth) return;

        output_lock.lock();
        std::cout << "2" << std::endl;
        output_lock.unlock();

        // If we're not searching for this kind of item, then skip this item.
        if ((is_directory && search_type == search_types::file) ||
            (!is_directory && search_type == search_types::directory))
            return;

        // Now, we check for a name match.
        std::string short_name =
            std::filesystem::path(current_place.path).filename();

        if (std::regex_match(short_name, pattern)) {
            output_lock.lock();
            std::cout << current_place.path << std::endl;
            output_lock.unlock();
        }
    } catch (std::filesystem::filesystem_error &error) {
        // TODO: Formatting.
        output_lock.lock();
        std::cerr << error.path1() << ": " << error.what() << std::endl;
        output_lock.unlock();
    }

    thread_counter_lock.lock();
    places_queue_lock.lock();
    thread_counter--;

    while (thread_counter < num_threads && !places_queue.empty()) {
        place next_place = places_queue.front();
        places_queue.pop_front();

        thread_counter++;

        std::thread(examine_place, std::cref(next_place),
                    std::ref(places_queue), std::ref(searched),
                    std::ref(pattern), std::ref(thread_counter),
                    std::ref(output_lock), std::ref(places_queue_lock),
                    std::ref(thread_counter_lock), search_type, min_depth,
                    max_depth, num_threads)
            .detach();
    }

    places_queue_lock.unlock();
    thread_counter_lock.unlock();
}

/**
 * @brief Main program entry point.
 *
 * @param argc The count of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return 0 on success; 1 on failure.
 */
int main(int argc, char **argv) {
    /**
     * @brief The type of thing that we're looking for (file or folder).
     * @details Defaults to search for anything.
     *
     */
    search_types search_type = search_types::any;

    /**
     * @brief The pattern to match.
     * @details Defaults to match everything.
     *
     */
    std::regex pattern(".*");

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
     * @details Defaults to 2^32 - 1.
     *
     */
    unsigned int max_depth = -1;

    /**
     * @brief The number of threads to use.
     * @details Defaults to 1.
     *
     */
    unsigned int num_threads = 1;

    /**
     * @brief Processes the program arguments.
     *
     */
    argh args(argc, argv);

    /**
     * @brief Counter of all threads currently running.
     *
     */
    unsigned int thread_counter = 0;

    /**
     * @brief Mutex to protect console output.
     *
     */
    std::mutex output_lock;

    /**
     * @brief Mutex to protect the queue of places to search.
     *
     */
    std::mutex places_queue_lock;

    /**
     * @brief Mutex to protect the counter of running threads.
     *
     */
    std::mutex thread_counter_lock;

    // First, select the pattern.
    // The argument checks are ordered from most specific to least specific.
    std::string pattern_str[] = {"-regex", "-iregex", "-name", "-iname"};
    for (unsigned short index = 0; index < 4; index++) {
        if (args[pattern_str[index]] > 0) {
            switch (index) {
                // "-regex":
                case 0:
                    pattern = std::regex(args("-regex"));
                    break;
                // "-iregex":
                case 1:
                    pattern = std::regex(args("-iregex"),
                                         std::regex_constants::icase);
                    break;
                // "-name":
                case 2:
                    pattern = std::regex(wild_to_regex(args("-name")));
                    break;
                // "-iname":
                case 3:
                    pattern = std::regex(wild_to_regex(args("-iname")),
                                         std::regex_constants::icase);
                    break;
                default:
                    break;
            }
        }
    }

    // Next, handle the type of search.
    if (args["-type"] > 0) {
        std::string type = args("-type");
        if (type == "d" || type == "directory") {
            search_type = search_types::directory;
        } else if (type == "f" || type == "file") {
            search_type = search_types::file;
        } else if (type == "a" || type == "any") {
            search_type = search_types::any;
        } else {
            std::cerr << "Error: unknown search type \"" << type << "\"."
                      << std::endl;
            exit(1);
        }
    }

    // After that, handle the depth arguments.
    if (args["-mindepth"] > 0) {
        min_depth = std::stoi(args("-mindepth"));
    }
    if (args["-maxdepth"] > 0) {
        max_depth = std::stoi(args("-maxdepth"));
    }
    if (max_depth < min_depth) {
        std::cerr << "Error: max depth is less than min depth." << std::endl;
        exit(1);
    }

    // Handle the `-j` (threads) argument, if given:
    if (args["-j"] > 0) {
        num_threads = std::stoi(args("-j"));

        if (num_threads < 1) {
            std::cerr << "Error: must use at least 1 thread." << std::endl;
            exit(1);
        }
    }

    // Finally, handle the "-h" and "--help" commands.
    if (args["-h"] + args["--help"] > 0) {
        std::cout << "Usage:" << std::endl
                  << "    " << argv[0]
                  << " <d> <p> [-type <t>] [-mindepth <i>] [-maxdepth <a>]"
                  << std::endl
                  << "        Where <d> is one or more files or directories."
                  << std::endl
                  << "        Where <p> is one of: {-name, -iname, -regex, "
                     "-iregex} <pattern>."
                  << std::endl
                  << "        Where <t> is one of: {f, file, d, dir, place}."
                  << std::endl;

        exit(0);
    }

    // The remaining arguments are files or directories to search.
    for (size_t index = 1; index < args.size(); index++) {
        std::string place(args[index]);
        places_queue.emplace_back(place);
        searched.emplace(place);
    }

    // Begin search.
    places_queue_lock.lock();
    for (unsigned int thread_id = 0; thread_id < num_threads; thread_id++) {
        if (places_queue.empty()) break;
        place current_place = places_queue.front();
        places_queue.pop_front();

        thread_counter_lock.lock();
        thread_counter++;
        thread_counter_lock.unlock();

        std::thread(examine_place, std::cref(current_place),
                    std::ref(places_queue), std::ref(searched),
                    std::ref(pattern), std::ref(thread_counter),
                    std::ref(output_lock), std::ref(places_queue_lock),
                    std::ref(thread_counter_lock), search_type, min_depth,
                    max_depth, num_threads)
            .detach();
    }
    places_queue_lock.unlock();

    thread_counter_lock.lock();
    bool running_threads = thread_counter > 0;
    thread_counter_lock.unlock();

    while (running_threads) {
        thread_counter_lock.lock();
        running_threads = thread_counter > 0;
        thread_counter_lock.unlock();
    }

    return 0;
}
