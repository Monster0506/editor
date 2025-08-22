#pragma once
#include <iostream>
#include <string>
#include <vector>

class RegexEngine {
    /* TODO: Implement this function */
   public:
    static int findMatch(const std::string &pattern,
                         const std::vector<std::string> &lines, int startline,
                         bool forward = true);
};
