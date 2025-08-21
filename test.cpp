#include <iostream>
#include <vector>
#include <sstream>
#include "parser.hpp"
#include "address.hpp"
#include "command.hpp"


using namespace std;

struct TestCase {
    string input;
    string expected;
    int dot = 0;
    unordered_map<string, int> marks = {};
};

// Helper to stringify Address
string toString(const Address &a) {
    ostringstream os;
    os << a;
    return os.str();
}

int main() {
    vector<TestCase> tests = {
        // Simple addresses
        {"1p", "addr1=1(NUMBER) addr2=1(NONE) command=p params=''", 1,{}},
        {".p", "addr1=1(CURRENT) addr2=1(NONE) command=p params=''",1, {}},
        {"$p", "addr1=0(LAST) addr2=0(NONE) command=p params=''", 0, {}},
        {"+3p", "addr1=3(RELATIVE) addr2=0(NONE) command=p params=''", 0, {}},
        {"-2p", "addr1=-2(RELATIVE) addr2=0(NONE) command=p params=''", 0, {}},
        {"42", "addr1=42(NUMBER) addr2=0(NONE) command= params=''", 0, {}},
        {"0p", "addr1=0(NUMBER) addr2=0(NONE) command=p params=''", 0, {}},

        // Ranges
        {"1,5d", "addr1=1(NUMBER) addr2=5(NUMBER) command=d params=''", 0, {}},
        {",p", "addr1=1(NUMBER) addr2=0(LAST) command=p params=''", 0, {}},
        {";n", "addr1=0(CURRENT) addr2=0(LAST) command=n params=''", 0, {}},
        {"1,5", "addr1=1(NUMBER) addr2=5(NUMBER) command= params=''", 0, {}},
        {"1;5", "addr1=1(NUMBER) addr2=5(NUMBER) command= params=''", 1, {}},
        {",", "addr1=1(NUMBER) addr2=0(LAST) command= params=''", 0, {}},
        {";", "addr1=0(CURRENT) addr2=0(LAST) command= params=''", 0, {}},
        {"1,5,7p", "addr1=5(NUMBER) addr2=7(NUMBER) command=p params=''", 0, {}},

        // Regex addresses
        {"/foo/p", "addr1=0(REGEX_FWD:foo) addr2=0(NONE) command=p params=''", 0, {}},
        {"?bar?d", "addr1=0(REGEX_BACK:bar) addr2=0(NONE) command=d params=''", 0, {}},
        {"/foo/,/bar/d", "addr1=0(REGEX_FWD:foo) addr2=0(REGEX_FWD:bar) command=d params=''",0, {}},
        {"?baz?;5p", "addr1=0(REGEX_BACK:baz) addr2=5(NUMBER) command=p params=''",0, {}},
        {"/a\\/b/p", "addr1=0(REGEX_FWD:a\\/b) addr2=0(NONE) command=p params=''",0, {}},

        // Marks
        {"'a,'bz", "addr1=5(MARK:a) addr2=10(MARK:b) command=z params=''",0, {{"a",5}, {"b",10}}},
        {"'c", "addr1=35(MARK:c) addr2=0(NONE) command= params=''",0, {{"c",35}, {"d",0}}},

        // Commands with params
        {"w filename.txt", "addr1=0(NONE) addr2=0(NONE) command=w params='filename.txt'",0},
        {"3,7s/old/new/g", "addr1=3(NUMBER) addr2=7(NUMBER) command=s params='/old/new/g'",0},
        {"$a some text", "addr1=0(LAST) addr2=0(NONE) command=a params='some text'",0},
        {"e otherfile", "addr1=0(NONE) addr2=0(NONE) command=e params='otherfile'", 0},
        {"E forcefile", "addr1=0(NONE) addr2=0(NONE) command=E params='forcefile'",0},
        {"f myfile", "addr1=0(NONE) addr2=0(NONE) command=f params='myfile'",0},
        {"r !ls", "addr1=0(NONE) addr2=0(NONE) command=r params='!ls'", 0},
        {"wq out.txt", "addr1=0(NONE) addr2=0(NONE) command=w params='q out.txt'", 0},

        // Misc commands
        {"q", "addr1=0(NONE) addr2=0(NONE) command=q params=''", 0, {}},
        {"Q", "addr1=0(NONE) addr2=0(NONE) command=Q params=''", 0, {}},
        {"u", "addr1=0(NONE) addr2=0(NONE) command=u params=''", 0, {}},
        {"h", "addr1=0(NONE) addr2=0(NONE) command=h params=''", 0, {}},
        {"H", "addr1=0(NONE) addr2=0(NONE) command=H params=''", 0, {}},
        {"=", "addr1=0(NONE) addr2=0(NONE) command== params=''", 0, {}},
        {"!date", "addr1=0(NONE) addr2=0(NONE) command=! params='date'", 0, {}},
        {"2!sort", "addr1=2(NUMBER) addr2=0(NONE) command=! params='sort'", 0, {}},
        {"# comment", "addr1=0(NONE) addr2=0(NONE) command=# params='comment'",  0,{}}
    };

    Parser p = Parser();
    Address a1, a2;
    char c;
    string params;
    for (int i = 0; i<(int)tests.size(); i++) {
        TestCase t = tests[i];

        p.parseCommand(t.input, a1, a2, c, params);

        ostringstream got;


        cout << "Input:  " << t.input << "\n";
        cout << "Expect: " << t.expected << "\n";
        Command cm = Command::build_command(c,a1,a2,params,t.dot,5, t.marks);
        got << cm;
        cout << "Got:    " << got.str() << "\n";
        cout << (got.str() == t.expected ? "PASS" : "FAIL") << "\n\n";
    }
}
