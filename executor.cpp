#include <algorithm>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "address.hpp"
#include "executor.hpp"
#include "parser.hpp"

using namespace std;

void helper_UpdateMarksAfterDeletion(int start, int end,
                                     unordered_map<string, int> &marks) {
    for (auto it = marks.begin(); it != marks.end();) {
        if (it->second >= start && it->second <= end) {
            it = marks.erase(it);
            // removing this mark means we have to check if we are at the end
            // already!
        } else {
            it->second -= (end - start + 1);
        }
        // that's what this is for
        if (it == marks.end()) break;
        ++it;
    }
}

void helper_setUndoState(stack<pair<vector<string>, int>> &undo_stack,
                         stack<pair<vector<string>, int>> &redo_stack,
                         vector<string> lines, int dot) {
    undo_stack.push({lines, dot});
    while (!redo_stack.empty()) redo_stack.pop();  // clear redo history
}

bool Executor::cmd_put_register(Command c, vector<string> &lines, int &dot,
                                bool &modified, char reg, bool after) {
    if (registers.find(reg) == registers.end() || registers[reg].empty())
        return false;

    int insert_idx = (c.a1.number == 0 ? dot : c.a1.number);
    if (after) insert_idx++;  // put after

    if (insert_idx < 0 || insert_idx > (int)lines.size()) return false;

    lines.insert(lines.begin() + insert_idx, registers[reg].begin(),
                 registers[reg].end());

    dot = insert_idx + (int)registers[reg].size();
    modified = true;
    return true;
}

/**
 * Copies lines between a1 and a2 (inclusive) and stores in a register.
 * If no register is provided, stores in the unnamed register
 * Registers: "_" for last copied text, '"' default for unnamed register
 * Marks: "!" for start of copy block, "@" for end of copy block
 */
bool Executor::cmd_yank(Command c, vector<string> &lines, char reg) {
    if (lines.empty()) return false;
    if (c.a1.number < 1 || c.a2.number < c.a1.number ||
        c.a2.number > (int)lines.size())
        return false;

    int begin = c.a1.number - 1;
    int end = c.a2.number - 1;
    marks["!"] = begin;
    marks["@"] = end;
    registers['_'].clear();
    registers[reg].clear();
    for (int i = begin; i <= end; i++) {
        registers[reg].push_back(lines[i]);
        registers['_'].push_back(lines[i]);
    }
    dot = c.a2.number;
    return true;
}

/**
 * Delete lines between a1 and a2 (inclusive).
 */
bool Executor::cmd_delete(Command c, vector<string> &lines, int &dot,
                          bool &modified) {
    if (lines.empty()) return false;

    if (c.a1.number < 1 || c.a2.number < c.a1.number ||
        c.a2.number > (int)lines.size())
        return false;
    helper_UpdateMarksAfterDeletion(c.a1.number, c.a2.number, marks);

    registers['_'].clear();
    registers['"'].clear();
    for (int i = c.a1.number - 1; i < c.a2.number; i++) {
        registers['_'].push_back(lines[i]);
        registers['"'].push_back(lines[i]);
    }

    auto start = lines.begin() + (c.a1.number - 1);
    auto end = lines.begin() + c.a2.number;

    lines.erase(start, end);

    if (c.a1.number <= (int)lines.size())
        dot = c.a1.number;
    else
        dot = lines.empty() ? 0 : (int)lines.size();

    modified = true;
    return true;
}

/**
 * Append lines after address a1.
 */
bool Executor::cmd_append(Command c, vector<string> &lines, int &dot,
                          bool &modified) {
    string input;
    int insert_idx = (c.a1.number == 0 ? 0 : c.a1.number);

    if (insert_idx < 0 || insert_idx > (int)lines.size()) return false;

    while (true) {
        if (!getline(cin, input)) break;
        if (input == ".") break;

        lines.insert(next(lines.begin(), insert_idx), input);
        modified = true;
        insert_idx++;
    }

    dot = insert_idx;
    return true;
}

/**
 * Insert before address a1.
 */
bool Executor::cmd_insert(Command c, vector<string> &lines, int &dot,
                          bool &modified) {
    c.a1.number = max(0, c.a1.number - 1);
    return cmd_append(c, lines, dot, modified);
}

/**
 * Write lines between a1 and a2 to file.
 */
bool Executor::cmd_write(Command c, vector<string> &lines, string &params,
                         string &filename, bool &modified) {
    if (lines.empty()) return false;

    string local_fname;
    if (!params.empty()) {
        filename = params;
        local_fname = filename;
    } else if (!filename.empty()) {
        local_fname = filename;
    } else {
        return false;
    }

    ofstream file(local_fname);
    if (!file.is_open()) return false;

    if (c.a1.number > c.a2.number) return false;

    int begin = (c.a1.type == AddressType::NONE ? 1 : c.a1.number);
    int end =
        (c.a2.type == AddressType::NONE ? (int)lines.size() : c.a2.number);

    begin = max(1, begin);
    end = min(end, (int)lines.size());

    for (int i = begin - 1; i < end; i++) {
        file << lines.at(i) << "\n";
    }

    cout << local_fname << endl;
    file.close();
    modified = false;
    return true;
}

/**
 * Print lines between a1 and a2.
 */
bool Executor::cmd_put(Command c, vector<string> &lines, int &dot) {
    if (lines.empty()) return true;

    if (c.a1.number == 0 && c.a2.number == 0) {
        if (dot >= 1 && dot <= (int)lines.size())
            cout << lines.at(dot - 1) << endl;
    } else {
        int begin = max(0, c.a1.number - 1);
        int end = (c.a2.number == 0 ? (int)lines.size() : c.a2.number) - 1;
        end = min(end, (int)lines.size() - 1);

        for (int i = begin; i <= end && i < (int)lines.size(); i++)
            cout << lines.at(i) << endl;

        dot = end + 1;
    }
    return true;
}

bool Executor::cmd_undo(vector<string> &lines) {
    if (!undo_stack.empty()) {
        redo_stack.push({lines, dot});  // save current state for redo
        auto prev = undo_stack.top();
        undo_stack.pop();

        lines = prev.first;
        dot = prev.second;
        modified = true;
        return true;
    }
    return false;
}

bool Executor::cmd_redo(vector<string> &lines) {
    if (!redo_stack.empty()) {
        undo_stack.push({lines, dot});  // save current state for undo
        auto next = redo_stack.top();
        redo_stack.pop();

        lines = next.first;
        dot = next.second;
        modified = true;
        return true;
    }
    return false;
}

/**
 * Print lines with line numbers.
 */
bool Executor::cmd_put_with_numbers(Command c, vector<string> &lines,
                                    int &dot) {
    if (lines.empty()) return true;

    if (c.a1.number == 0 && c.a2.number == 0) {
        if (dot >= 1 && dot <= (int)lines.size())
            cout << dot << "\t" << lines.at(dot - 1) << endl;
    } else {
        int begin = max(0, c.a1.number - 1);
        int end = (c.a2.number == 0 ? (int)lines.size() : c.a2.number) - 1;
        end = min(end, (int)lines.size() - 1);

        for (int i = begin; i <= end && i < (int)lines.size(); i++)
            cout << i + 1 << "\t" << lines.at(i) << endl;

        dot = end + 1;
    }
    return true;
}

/**
 * Helper: check if address is numerical.
 */
bool aisNumericalAddress(Address a) {
    return a.type == AddressType::NUMBER || a.type == AddressType::CURRENT ||
           a.type == AddressType::RELATIVE || a.type == AddressType::LAST;
}

/**
 * Execute a command.
 */
bool Executor::executeCommands(Command &c, vector<string> &lines) {
    if (c.command == '\0') {
        if (c.a1.type == AddressType::MARK && c.a2.type == AddressType::NONE) {
            // Allow mark command in format '[char]
            c.command = '\'';
            c.params = c.a1.extra;
        } else
            c.command = 'n';
    }

    if (dot == 0 && !lines.empty()) dot = 1;

    // Resolve addresses
    if (c.a1.type == AddressType::CURRENT) c.a1.number = dot;
    if (c.a2.type == AddressType::CURRENT) c.a2.number = dot;
    if (c.a1.type == AddressType::LAST) c.a1.number = (int)lines.size();
    if (c.a2.type == AddressType::LAST) c.a2.number = (int)lines.size();
    if (c.a1.type == AddressType::RELATIVE) c.a1.number += dot;
    if (c.a2.type == AddressType::RELATIVE) c.a2.number += dot;
    if (c.a1.type == AddressType::MARK) {
        if (marks.find(c.a1.extra) == marks.end()) return false;
        c.a1.number = marks[c.a1.extra];
    }
    if (c.a2.type == AddressType::MARK) {
        if (marks.find(c.a2.extra) == marks.end()) return false;
        c.a2.number = marks[c.a2.extra];
    }

    if ((c.a1.type == AddressType::LAST && lines.empty()) ||
        (c.a2.type == AddressType::LAST && lines.empty())) {
        return false;
    }

    if (aisNumericalAddress(c.a1) &&
        (c.a2.type == AddressType::NONE ||
         (c.a2.type == AddressType::NUMBER && c.a2.number == 0)))
        c.a2.number = c.a1.number;

    if (debug) Parser::displayCommand(c);

    switch (c.command) {
        case '#':
            return true;

        case 'd':
            helper_setUndoState(undo_stack, redo_stack, lines, dot);
            return cmd_delete(c, lines, dot, modified);

        case 'a':
            helper_setUndoState(undo_stack, redo_stack, lines, dot);
            return cmd_append(c, lines, dot, modified);

        case 'i':
            helper_setUndoState(undo_stack, redo_stack, lines, dot);
            return cmd_insert(c, lines, dot, modified);

        case 'c':
            helper_setUndoState(undo_stack, redo_stack, lines, dot);
            return cmd_delete(c, lines, dot, modified) &&
                   cmd_insert(c, lines, dot, modified);

        case 'w':
            return cmd_write(c, lines, c.params, filename, modified);

        case 'o':
            return cmd_put(c, lines, dot);

        case 'n':
            return cmd_put_with_numbers(c, lines, dot);

        case 'u':
            return cmd_undo(lines);

        case 'r':
            return cmd_redo(lines);

        case '=':
            cout << dot << endl;
            break;

        case 'q':
            if (!modified)
                exit(0);
            else
                return false;
            break;
        case 'Q':
            exit(0);
            break;

        case 'm':
            if (!c.params.empty() && isalpha(c.params[0])) {
                marks[string(1, c.params[0])] = dot;
                return true;
            }
            return false;
            break;

        case 'y':
            if (!c.params.empty() && isalpha(c.params[0]))
                return cmd_yank(c, lines, (unsigned char)c.params[0]);
            return cmd_yank(c, lines);

        case 'p':
            helper_setUndoState(undo_stack, redo_stack, lines, dot);
            if (c.params.empty())
                return cmd_put_register(c, lines, dot, modified, '"', true);
            return cmd_put_register(c, lines, dot, modified,
                                    (unsigned char)c.params[0], true);

        case 'P':
            helper_setUndoState(undo_stack, redo_stack, lines, dot);
            if (c.params.empty())
                return cmd_put_register(c, lines, dot, modified, '"', false);
            return cmd_put_register(c, lines, dot, modified,
                                    (unsigned char)c.params[0], false);

        case '\'':
            if (!c.params.empty()) {
                string mark(1, c.params[0]);
                if (marks.find(mark) != marks.end()) {
                    dot = marks.at(mark);
                    return true;
                }
            }
            return false;

        default:
            return false;
    }
    return true;
}
