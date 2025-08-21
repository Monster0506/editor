#include <iostream>

#include "address.hpp"
using namespace std;
ostream &operator<<(ostream &os, AddressType t) {
    switch (t) {
        case AddressType::NONE:
            os << "NONE";
            break;
        case AddressType::LAST:
            os << "LAST";
            break;
        case AddressType::NUMBER:
            os << "NUMBER";
            break;
        case AddressType::CURRENT:
            os << "CURRENT";
            break;
        case AddressType::RELATIVE:
            os << "RELATIVE";
            break;
        case AddressType::REGEX_FWD:
            os << "REGEX_FWD";
            break;
        case AddressType::REGEX_BACK:
            os << "REGEX_BACK";
            break;
        case AddressType::MARK:
            os << "MARK";
            break;
    }
    return os;
}

ostream &operator<<(ostream &os, const Address &a) {
    os << a.number << "(" << a.type;
    if (!a.extra.empty()) os << ":" << a.extra;
    os << ")";
    return os;
}
