#include <regex>
#include <string>
#include <vector>

#include "regex.hpp"
using namespace std;

int clamp(int a, int b, int c) { return (a < b) ? b : (a > c ? c : a); }

int RegexEngine::findMatch(const string &pattern, const vector<string> &lines,
                           int startline, bool forward) {
    /* 1. Handle empty lines vector */
    if (lines.empty()) {
        return -1;
    }
    /* 2. Convert startline to zero indexed */

    startline = clamp(startline - 1, 0, lines.size() - 1);
    regex re(pattern);

    auto search = [&](int begin, int end, int step) -> int {
        /* 3.1 Single for loop, move through lines */
        for (int i = begin; i != end; i += step) {
            if (regex_search(lines.at(i), re)) {
                return i + 1;
            }
        }
        return -1;
    };
    int result;
    /* 3. Search forward or backwards through lines */
    if (forward) {
        result = search(startline, lines.size(), 1);
        if (result == -1) result = search(0, startline, 1);
    } else {
        result = search(startline, -1, -1);
        if (result == -1) result = search(lines.size() - 1, startline, -1);
    }

    /* 4. Wrap around file boundaries */

    /* 5. Return 1-indexed line number or -1 if no match */
    return result;
}
