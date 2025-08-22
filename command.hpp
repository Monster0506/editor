#pragma once
#include <string>
#include <unordered_map>

#include "address.hpp"

class Command {
   public:
    Address a1 = Address();
    Address a2 = Address();
    char command = 'n';
    std::string params;
    static Command build_command(char cmd, Address a1_, Address a2_,
                                 std::string &params_, int dot, int lines_size,
                                 std::unordered_map<std::string, int> &marks);
};
std::ostream &operator<<(std::ostream &os, const Command &c);
