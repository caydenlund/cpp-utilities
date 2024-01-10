/**
 * @file autotag.cpp
 * @author Cayden Lund (cayden.lund@utah.edu)
 *
 * @brief Implements the `autotag` executable, which automatically sets the ID3v2 tags of an MP3 file.
 * @details
 *     This does two things for each MP3 track:
 *       1. Sets the ID3v2 tags.
 *       2. Moves the file to the appropriate directory,
 *          making directories as appropriate.
 *
 *     Usage:    autotag <files...> [options...]
 *     Options:
 *         -h,  --help                  Print usage and exit.
 *         -D,  --dir     <output_dir>  Set the base output directory for the tagged files.
 *
 *         -a,  --artist  <artist>      Set the track's artist to the given artist for all input files.
 *         -A,  --album   <album>       Set the track's album to the given album for all input files.
 *         -y,  --year    <year>        Set the track's year to the given year for all input files.
 *         -T,  --tracks  <num_tracks>  Set the total number of tracks on the album, for all input files.
 *                                      Also, for each file, if the filename does not begin with a number,
 *                                      prompt for the track number on the album.
 *
 *         --no-a,  --no-artist         Don't set the artist tag.
 *         --no-A,  --no-album          Don't set the album tag.
 *         --no-y,  --no-year           Don't set the year tag.
 *         --no-mv                      Don't move the files (set the tags in-place).
 *
 * @license MIT license (https://opensource.org/licenses/MIT).
 * @copyright (c) 2023 by Cayden Lund.
 *
 */

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

int run_id3v2(const std::string& file_name, const std::string& track_name, const std::string& artist,
              const std::string& album, const std::string& year, const std::string& track_num) {
    const auto quote_str = [](const std::string& str) { return "\"" + str + "\""; };
    const auto build_str = [](const std::vector<std::string>& args) {
        std::stringstream result;
        if (!args.empty()) {
            for (const auto& arg : std::vector<std::string> {args.begin(), args.end() - 1}) { result << arg << " "; }
            result << args[args.size() - 1];
        }
        return result.str();
    };

    struct arg_pair {
        std::string arg;
        std::string val;
    };
    const std::vector<arg_pair> args {{"-t", quote_str(track_name)},
                                      {"-a", quote_str(artist)},
                                      {"-A", quote_str(album)},
                                      {"-y", year},
                                      {"-T", track_num}};

    std::vector<std::string> command {"id3v2"};
    for (const auto& pair : args) {
        if (!pair.val.empty()) {
            command.emplace_back(pair.arg);
            command.emplace_back(pair.val);
        }
    }
    command.emplace_back(quote_str(file_name));

    return std::system(build_str(command).c_str());
}

void print_help(const std::string& executable_name) {
    //  TODO: This.
}

int main(int argc, char** argv) {
    bool do_artist = true;
    bool do_album = true;
    bool do_year = true;
    bool do_mv = true;

    std::vector<std::string> files;

    std::string output_dir;
    std::string artist;
    std::string album;
    std::string year;
    std::string total_tracks;

    for (int arg_ind = 1; arg_ind < argc; ++arg_ind) {
        std::string arg = argv[arg_ind];
        if (arg == "-h" || arg == "--help") {
            //  Print usage and exit.
            print_help(argv[0]);
            return 0;
        } else if (arg == "-D" || arg == "--dir") {
            //  Set the base output directory for the tagged files.
            if (arg_ind + 1 == argc) {
                print_help(argv[0]);
                return 0;
            }
            output_dir = arg;
        } else if (arg == "-a" || arg == "--artist") {
            //  Set the track's artist to the given artist for all input files.
            if (arg_ind + 1 == argc) {
                print_help(argv[0]);
                return 0;
            }
            artist = arg;
        } else if (arg == "-A" || arg == "--album") {
            //  Set the track's album to the given album for all input files.
            if (arg_ind + 1 == argc) {
                print_help(argv[0]);
                return 0;
            }
            album = arg;
        } else if (arg == "-y" || arg == "--year") {
            //  Set the track's year to the given year for all input files.
            if (arg_ind + 1 == argc) {
                print_help(argv[0]);
                return 0;
            }
            year = arg;
        } else if (arg == "-T" || arg == "--tracks") {
            //  Set the total number of tracks on the album, for all input files.
            //  Also, for each file, if the filename does not begin with a number, prompt for the track number on the
            //  album.
            if (arg_ind + 1 == argc) {
                print_help(argv[0]);
                return 0;
            }
            total_tracks = arg;
        } else if (arg == "--no-a" || arg == "--no-artist") {
            //  Don't set the artist tag.
            do_artist = false;
        } else if (arg == "--no-A" || arg == "--no-album") {
            //  Don't set the album tag.
            do_album = false;
        } else if (arg == "--no-y" || arg == "--no-year") {
            //  Don't set the year tag.
            do_year = false;
        } else if (arg == "--no-mv") {
            //  Don't move the files (set the tags in-place).
            do_mv = false;
        } else {
            //  It's a filename.
            //  TODO: Check that it's a valid filename.
            files.emplace_back(arg);
        }
    }

    return 0;
}
