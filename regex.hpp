#pragma once
#include <string>
#include <vector>

class RegexEngine {
   public:
    static int findMatch(const std::string &pattern,
                         const std::vector<std::string> &lines, int startline,
                         bool forward = true);
};
