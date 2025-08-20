#pragma once
#include <iostream>
using namespace std;
enum class AddressType {
    NONE,
    LAST,
    NUMBER,
    CURRENT,
    RELATIVE,
    REGEX_FWD,
    REGEX_BACK,
    MARK
};

// Pretty-print AddressType
class Address {
public:
    AddressType type;
    int number;
    string extra; // regex or mark name

    Address(): type(AddressType::NONE), number(0) {}
    Address(AddressType type_): type(type_), number(0) {}
    Address(AddressType type_, int number_): type(type_), number(number_) {}
    Address(AddressType type_, const string &extra_): type(type_), number(0), extra(extra_) {}
};



std::ostream &operator<<(std::ostream &os, AddressType t);
std::ostream &operator<<(std::ostream &os, const Address &a);
