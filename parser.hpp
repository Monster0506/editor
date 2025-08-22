#pragma once
#include <iostream>

#include "address.hpp"
#include "command.hpp"
class Parser {
   public:
    static void displayCommand(Command c) {
        std::cout << "---------COMMAND---------" << std::endl;
        std::cout << c << std::endl;
        std::cout << "-------------------------" << std::endl;
    }
    static bool parseSingleAddress(const std::string &s, size_t &pos,
                                   Address &addr);
    bool parseCommand(const std::string &cmd, Address &addr1, Address &addr2,
                      char &c, std::string &params);
};
