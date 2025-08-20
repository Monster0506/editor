#pragma once
#include <iostream>
#include "address.hpp"
using namespace std;
class Parser{
    public:
        static void displayCommand(Address a1, Address a2, char c, string params){

            cout<<"---------COMMAND---------"<<endl;
            cout<<(a1)<<","<<(a2)<<c<<"{"<<params<<"}"<<endl;
            cout<<"-------------------------"<<endl;
        }
        static bool parseSingleAddress(const string &s, size_t &pos, Address &addr);
        bool parseCommand(const string &cmd, Address &addr1, Address &addr2, char &c, string &params);
};
