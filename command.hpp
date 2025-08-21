#pragma once
#include <string>
#include "address.hpp"
using namespace std;


class Command{
    public:
        Address a1= Address();
        Address a2= Address();
        char command = 'n';
        string params;
        static Command build_command(char cmd, Address a1_, Address a2_, const string &params_, int dot, int lines_size);
};
