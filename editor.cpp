#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "address.hpp"
#include "command.hpp"
#include "executor.hpp"
#include "parser.hpp"
using namespace std;

int main(int argc, char **argv) {
    vector<string> lines;
    vector<string> original_lines;
    string line;
    string filename;
    Address a1, a2;
    char c;
    string params;
    string cmd;
    Parser parser = Parser();
    if (argc < 2 || !argv[1]) exit(1);
    filename = argv[1];

    ifstream file(filename);

    Executor executor = Executor(true);
    executor.filename = filename;

    if (!file.is_open()) exit(1);

    while (getline(file, line)) {
        lines.push_back(line);
        original_lines.push_back(line);
    }

    while (true) {
        if (executor.debug) cout << "* ";
        getline(cin >> ws, cmd);

        if (!parser.parseCommand(cmd, a1, a2, c, params)) {
            cout << "bad parse" << endl;
            cout << "?" << endl;
            continue;
        }
        Command command;
        try {
            command = Command::build_command(c, a1, a2, params, executor.dot,
                                             lines.size(), executor.marks);
        } catch (...) {
            cout << "bad command construction" << endl;
            cout << "?" << endl;
            continue;
        }

        if (!executor.executeCommands(command, lines)) {
            cout << "bad command construction" << endl;
            cout << "?" << endl;
            continue;
        }
    }
    return 0;
}
