#include "command.hpp"


bool isNumericalAddress(Address a) {
    return a.type == AddressType::NUMBER ||
        a.type == AddressType::CURRENT ||
        a.type == AddressType::RELATIVE ||
        a.type == AddressType::LAST;
}
Command Command::build_command(char cmd, Address a1_, Address a2_, string &params_, int dot, int lines_size, unordered_map<string, int> &marks){
    Command result = Command();

    if (cmd == '\0')
    {
        if (a1_.type == AddressType::MARK && a2_.type==AddressType::NONE){
            // Allow mark command in format '[char]
            cmd='\''; 
            params_=a1_.extra;
        }
        else cmd = 'n';
    }

    if (dot == 0 && lines_size!=0) dot = 1;

    if (a1_.type == AddressType::MARK) {
        if (marks.find(a1_.extra) == marks.end()) throw(1);
        a1_.number = marks[a1_.extra];
    }
    if (a2_.type == AddressType::MARK) {
        if (marks.find(a2_.extra) == marks.end()) throw(1);
        a2_.number = marks[a2_.extra];
    }

    if (a1_.type == AddressType::CURRENT) a1_.number = dot;
    if (a2_.type == AddressType::CURRENT) a2_.number = dot;
    if (a1_.type == AddressType::LAST) a1_.number = lines_size;
    if (a2_.type == AddressType::LAST) a2_.number = lines_size;
    if (a1_.type == AddressType::RELATIVE) a1_.number += dot;
    if (a2_.type == AddressType::RELATIVE) a2_.number += dot;

    if ((a1_.type == AddressType::LAST && lines_size == 0) ||
            (a2_.type == AddressType::LAST && lines_size == 0)) {
        throw 0;
    }





    if (isNumericalAddress(a1_) &&
            (a2_.type == AddressType::NONE ||
             (a2_.type == AddressType::NUMBER && a2_.number == 0)))
        a2_.number = a1_.number;



    result.command = cmd;
    result.a1=a1_;
    result.a2=a2_;
    result.params = params_;
    return result;



}



ostream &operator<<(ostream &os, const Command &c) {

    os<<"---------COMMAND---------"<<endl;
    os<<(c.a1)<<","<<(c.a2)<<c.command<<"{"<<c.params<<"}"<<endl;
    os<<"-------------------------"<<endl;

    return os;
}
