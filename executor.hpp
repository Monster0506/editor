#pragma once
#include "address.hpp"
#include <vector>
#include <stack>
#include <unordered_map>
using namespace std;

class Executor{

    public: 
        bool debug = true;
        bool modified = false;
        int dot=1; 
        string filename;
        stack< pair<vector<string>, int>> undo_stack;
        stack< pair<vector<string>, int>> redo_stack;
        // TODO: Store default registers, i.e., '"', "_", etc
        unordered_map<char, vector<string>> registers;  // handles copied lines, etc
        // TODO: Store default marks, i.e., "!","@", etc
        unordered_map<string, int> marks;   // handles saved line positions



        bool executeCommands(Address a1, Address a2, char c, vector<string> &lines, string &params);
        Executor() {}
        Executor(bool debug_) : debug(debug_) {}
        Executor(const string &fname) : filename(fname) {}
        Executor(const string &fname, bool debug_) : debug(debug_), filename(fname) {}

        bool cmd_delete(Address a1, Address a2, vector<string> &lines, int &dot, bool &modified);
        bool cmd_append(Address a1, vector<string> &lines, int &dot, bool &modified);
        bool cmd_insert(Address a1, vector<string> &lines, int &dot, bool &modified);
        bool cmd_write(Address a1, Address a2, vector<string> &lines, string &params, string &filename, bool &modified) ;
        bool cmd_undo(vector<string> &lines) ;
        bool cmd_redo(vector<string> &lines) ;
        bool cmd_put(Address a1, Address a2, vector<string> &lines, int &dot);
        bool cmd_put_with_numbers(Address a1, Address a2, vector<string> &lines, int &dot);


        bool cmd_put_register(Address a1, 
                vector<string> &lines, 
                int &dot, 
                bool &modified, 
                char reg = '"', 
                bool after = true);

        bool cmd_yank(Address a1, Address a2, 
                vector<string> &lines, 
                char reg = '"');
};
