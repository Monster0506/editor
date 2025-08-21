#include "command.hpp"


Command Command::build_command(char cmd, Address a1_, Address a2_, const string &params_, int dot, int lines_size){
    Command result = Command();


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




    result.command = cmd;
    result.a1=a1_;
    result.a2=a2_;
    result.params = params_;
    return result;



}
