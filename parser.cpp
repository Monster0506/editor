#include "parser.hpp"

bool Parser::parseSingleAddress(const string &s, size_t &pos, Address &addr) {
    while (pos < s.size() && isspace((unsigned char)s[pos])) pos++;
    if (pos >= s.size()) return false;

    char ch = s[pos];
    if (ch == '.') { 
        addr = Address();
        addr.type = AddressType::CURRENT;
        pos++;
        return true;
    } else if (ch == '$') { // last line
        addr = Address();
        addr.type = AddressType::LAST;
        pos++;
        return true;
    } else if (isdigit(ch)) { // explicit number
        int num = 0;
        while (pos < s.size() && isdigit((unsigned char)s[pos])) {
            num = num * 10 + (s[pos] - '0');
            pos++;
        }
        addr = Address();
        addr.type = AddressType::NUMBER;
        addr.number = num;
        return true;
    } else if (ch == '+' || ch == '-') { // relative
        char sign = ch;
        pos++;
        int num = 1; // default offset
        if (pos < s.size() && isdigit((unsigned char)s[pos])) {
            num = 0;
            while (pos < s.size() && isdigit((unsigned char)s[pos])) {
                num = num * 10 + (s[pos] - '0');
                pos++;
            }
        }
        if (sign == '-') num = -num;
        addr = Address();
        addr.type = AddressType::RELATIVE;
        addr.number = num;
        return true;
    } else if (ch == '/' || ch == '?') { // regex search
        char delim = ch;
        pos++;
        string re;
        while (pos < s.size() && s[pos] != delim) {
            if (s[pos] == '\\' && pos + 1 < s.size()) {
                re.push_back(s[pos]);
                pos++;
            }
            re.push_back(s[pos]);
            pos++;
        }
        if (pos < s.size() && s[pos] == delim) pos++;
        if (delim == '/'){
            addr = Address();
            addr.type =AddressType::REGEX_FWD;
            addr.extra = re;
        }
        else{
            addr = Address();;
            addr.type =AddressType::REGEX_BACK;
            addr.extra = re;
        }
        return true;
    } else if (ch == '\'') { // mark
        pos++;
        if (pos < s.size()) {
            string mark(1, s[pos]);
            addr = Address();
            addr.type = AddressType::MARK;
            addr.extra = mark;

            pos++;
            return true;
        }
    }

    return false;
}

bool Parser::parseCommand(const string &cmd, Address &addr1, Address &addr2, char &c, string &params) {
    size_t pos = 0;
    addr1 = Address();
    addr2 = Address();
    c = '\0';
    params.clear();

    Address tmp;
    bool hasAddr1 = parseSingleAddress(cmd, pos, tmp);
    if (hasAddr1) addr1 = tmp;

    // Check for range separator
    if (pos < cmd.size() && (cmd[pos] == ',' || cmd[pos] == ';')) {
        char sep = cmd[pos];
        pos++;
        bool hasAddr2 = parseSingleAddress(cmd, pos, tmp);
        if (hasAddr2) {
            addr2 = tmp;
        } else {
            // special defaults
            if (sep == ',') {
                addr1 = Address();
                addr1.type = AddressType::NUMBER;
                addr1.number = 1;
                addr2 = Address();
                addr2.type = AddressType::LAST;
            } else if (sep == ';') {
                addr1 = Address();
                addr1.type = AddressType::CURRENT;
                addr2 = Address();
                addr2.type = AddressType::LAST;
            }
        }
    }

    while (pos < cmd.size() && isspace((unsigned char)cmd[pos])) pos++;

    if (pos < cmd.size()) {
        c = cmd[pos];
        pos++;
    } else {
        return true; 
    }

    while (pos < cmd.size() && isspace((unsigned char)cmd[pos])) pos++;
    if (pos < cmd.size()) {
        params = cmd.substr(pos);
    }

    return true;
}

