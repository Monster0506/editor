#pragma once
#include <iostream>
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
    std::string extra;  // regex or mark name

    Address() : type(AddressType::NONE), number(0) {}
    Address(AddressType type_) : type(type_), number(0) {}
    Address(AddressType type_, int number_) : type(type_), number(number_) {}
    Address(AddressType type_, const std::string &extra_)
        : type(type_), number(0), extra(extra_) {}
};

std::ostream &operator<<(std::ostream &os, AddressType t);
std::ostream &operator<<(std::ostream &os, const Address &a);
