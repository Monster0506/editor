#include <iostream>
#include <vector>
#include <sstream>
#include "parser.hpp"
#include "address.hpp"


using namespace std;

struct TestCase {
    string input;
    string expected;
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
    {"1p", "addr1=1(NUMBER) addr2=0(NONE) command=p params=''"},
    {".p", "addr1=0(CURRENT) addr2=0(NONE) command=p params=''"},
    {"$p", "addr1=0(LAST) addr2=0(NONE) command=p params=''"},
    {"+3p", "addr1=3(RELATIVE) addr2=0(NONE) command=p params=''"},
    {"-2p", "addr1=-2(RELATIVE) addr2=0(NONE) command=p params=''"},
    {"42", "addr1=42(NUMBER) addr2=0(NONE) command= params=''"},
    {"0p", "addr1=0(NUMBER) addr2=0(NONE) command=p params=''"},
    
    // Ranges
    {"1,5d", "addr1=1(NUMBER) addr2=5(NUMBER) command=d params=''"},
    {",p", "addr1=1(NUMBER) addr2=0(LAST) command=p params=''"},
    {";n", "addr1=0(CURRENT) addr2=0(LAST) command=n params=''"},
    {"1,5", "addr1=1(NUMBER) addr2=5(NUMBER) command= params=''"},
    {"1;5", "addr1=1(NUMBER) addr2=5(NUMBER) command= params=''"},
    {",", "addr1=1(NUMBER) addr2=0(LAST) command= params=''"},
    {";", "addr1=0(CURRENT) addr2=0(LAST) command= params=''"},
    {"1,5,7p", "addr1=5(NUMBER) addr2=7(NUMBER) command=p params=''"},
    
    // Regex addresses
    {"/foo/p", "addr1=0(REGEX_FWD:foo) addr2=0(NONE) command=p params=''"},
    {"?bar?d", "addr1=0(REGEX_BACK:bar) addr2=0(NONE) command=d params=''"},
    {"/foo/,/bar/d", "addr1=0(REGEX_FWD:foo) addr2=0(REGEX_FWD:bar) command=d params=''"},
    {"?baz?;5p", "addr1=0(REGEX_BACK:baz) addr2=5(NUMBER) command=p params=''"},
    {"/a\\/b/p", "addr1=0(REGEX_FWD:a\\/b) addr2=0(NONE) command=p params=''"},
    
    // Marks
    {"'a,'bz", "addr1=0(MARK:a) addr2=0(MARK:b) command=z params=''"},
    {"'c", "addr1=0(MARK:c) addr2=0(NONE) command= params=''"},
    
    // Commands with params
    {"w filename.txt", "addr1=0(NONE) addr2=0(NONE) command=w params='filename.txt'"},
    {"3,7s/old/new/g", "addr1=3(NUMBER) addr2=7(NUMBER) command=s params='/old/new/g'"},
    {"$a some text", "addr1=0(LAST) addr2=0(NONE) command=a params='some text'"},
    {"e otherfile", "addr1=0(NONE) addr2=0(NONE) command=e params='otherfile'"},
    {"E forcefile", "addr1=0(NONE) addr2=0(NONE) command=E params='forcefile'"},
    {"f myfile", "addr1=0(NONE) addr2=0(NONE) command=f params='myfile'"},
    {"r !ls", "addr1=0(NONE) addr2=0(NONE) command=r params='!ls'"},
    {"wq out.txt", "addr1=0(NONE) addr2=0(NONE) command=w params='q out.txt'"},
    
    // Misc commands
    {"q", "addr1=0(NONE) addr2=0(NONE) command=q params=''"},
    {"Q", "addr1=0(NONE) addr2=0(NONE) command=Q params=''"},
    {"u", "addr1=0(NONE) addr2=0(NONE) command=u params=''"},
    {"h", "addr1=0(NONE) addr2=0(NONE) command=h params=''"},
    {"H", "addr1=0(NONE) addr2=0(NONE) command=H params=''"},
    {"=", "addr1=0(NONE) addr2=0(NONE) command== params=''"},
    {"!date", "addr1=0(NONE) addr2=0(NONE) command=! params='date'"},
    {"2!sort", "addr1=2(NUMBER) addr2=0(NONE) command=! params='sort'"},
    {"# comment", "addr1=0(NONE) addr2=0(NONE) command=# params='comment'"}
};

    Parser p = Parser();
    Address a1, a2;
    char c;
    string params;
    for (int i = 0; i<(int)tests.size(); i++) {
        TestCase t = tests[i];
        p.parseCommand(t.input, a1, a2, c, params);

        ostringstream got;
        got << "addr1=" << a1 << " addr2=" << a2
            << " command=" << c << " params='" << params << "'";

        cout << "Input:  " << t.input << "\n";
        cout << "Expect: " << t.expected << "\n";
        cout << "Got:    " << got.str() << "\n";
        cout << (got.str() == t.expected ? "PASS" : "FAIL") << "\n\n";
    }
}
