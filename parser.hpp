#pragma once
#include <iostream>
#include "address.hpp"
#include "command.hpp"
using namespace std;
class Parser{
    public:
        static void displayCommand(Command c){

            cout<<"---------COMMAND---------"<<endl;
            cout<<c<<endl;
            cout<<"-------------------------"<<endl;
        }
        static bool parseSingleAddress(const string &s, size_t &pos, Address &addr);
        bool parseCommand(const string &cmd, Address &addr1, Address &addr2, char &c, string &params);
};
