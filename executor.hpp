#pragma once
#include <stack>
#include <string>
#include <unordered_map>
#include <vector>

#include "command.hpp"

void helper_UpdateMarksAfterDeletion(
    int start, int end, std::unordered_map<std::string, int> &marks);
void helper_setUndoState(
    std::stack<std::pair<std::vector<std::string>, int>> &undo_stack,
    std::stack<std::pair<std::vector<std::string>, int>> &redo_stack,
    std::vector<std::string> lines, int dot);
class Executor {
   public:
    bool debug = false;
    bool modified = false;
    int dot = 1;
    std::string filename;
    // TODO: make these just cmd histories so we don't have to deal with stupid
    // stuff
    std::stack<std::pair<std::vector<std::string>, int>> undo_stack;
    std::stack<std::pair<std::vector<std::string>, int>> redo_stack;
    std::unordered_map<char, std::vector<std::string>>
        registers;                               // handles copied lines, etc
    std::unordered_map<std::string, int> marks;  // handles saved line positions

    bool executeCommands(Command &c, std::vector<std::string> &lines);
    Executor() {}
    Executor(bool debug_) : debug(debug_) {}
    Executor(const std::string &fname) : filename(fname) {}
    Executor(const std::string &fname, bool debug_)
        : debug(debug_), filename(fname) {}

    bool cmd_delete(Command c, std::vector<std::string> &lines, int &dot,
                    bool &modified);
    bool cmd_append(Command c, std::vector<std::string> &lines, int &dot,
                    bool &modified);
    bool cmd_insert(Command c, std::vector<std::string> &lines, int &dot,
                    bool &modified);
    bool cmd_write(Command c, std::vector<std::string> &lines,
                   std::string &params, std::string &filename, bool &modified);

    bool cmd_put(Command c, std::vector<std::string> &lines, int &dot);
    bool cmd_put_with_numbers(Command c, std::vector<std::string> &lines,
                              int &dot);

    bool cmd_put_register(Command c, std::vector<std::string> &lines, int &dot,
                          bool &modified, char reg = '"', bool after = true);

    bool cmd_yank(Command c, std::vector<std::string> &lines, char reg = '"');

    bool cmd_undo(std::vector<std::string> &lines);
    bool cmd_redo(std::vector<std::string> &lines);
};
