/**
 * @file calc.cpp
 * @brief A simple calculator program for the terminal.
 *
 * @author Cayden Lund (cayden.lund@utah.edu)
 * @license MIT (https://opensource.org/licenses/MIT)
 */

#include <ncurses.h>

#include <sstream>

typedef unsigned long long ull;

/**
 * @brief Given a stringstream and a character,
 *     handles that character by modifying the stringstream
 *     and printing to stdout.
 *
 * @param line The stringstream.
 * @param c The character to handle.
 * @return True if we can continue to read from stdin.
 */
bool handle_char(std::stringstream &line, unsigned short c) {
    switch (c) {
        // Ctrl+C:
        case KEY_BREAK:
        // Ctrl+D:
        case 4:
            return false;

        // Return key:
        case '\n':
        case '\r':
        case KEY_ENTER:
            // Push to stack.
            break;

        // Backspace:
        case KEY_BACKSPACE:
            printw("asdf");
            delch();
            delch();
            delch();
            refresh();
            break;

        default:
            clear();
            printw("%s (%c): %d\n", keyname(c), (char)c, c);
            refresh();
            break;
    }

    return true;
}

int main() {
    initscr();
    cbreak();
    noecho();
    keypad(stdscr, true);

    clear();

    printw("Lines: %d\nColumns: %d\n", LINES, COLS);
    refresh();

    std::stringstream line;
    unsigned short c = getch();
    while (handle_char(line, c)) {
        c = getch();
    }

    endwin();

    return 0;
}
