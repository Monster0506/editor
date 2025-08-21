#pragma once
#include <string>
#include <unordered_map>
#include "address.hpp"
using namespace std;


class Command{
    public:
        Address a1= Address();
        Address a2= Address();
        char command = 'n';
        string params;
        static Command build_command(char cmd, Address a1_, Address a2_, string &params_, int dot, int lines_size, unordered_map<string, int> &marks);

};
ostream &operator<<(ostream &os, const Command &c);
