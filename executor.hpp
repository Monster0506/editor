#pragma once
#include "address.hpp"
#include "command.hpp"
#include <vector>
#include <stack>
#include <unordered_map>
using namespace std;



void helper_UpdateMarksAfterDeletion(int start, int end, unordered_map<string, int> &marks);
void helper_setUndoState(stack<pair<vector<string>, int>> &undo_stack, stack<pair<vector<string>, int>> &redo_stack, vector<string>lines, int dot);
class Executor{

    public: 
        bool debug = true;
        bool modified = false;
        int dot=1; 
        string filename;
        // TODO: make these just cmd histories so we don't have to deal with stupid stuff
        stack< pair<vector<string>, int>> undo_stack;
        stack< pair<vector<string>, int>> redo_stack;
        unordered_map<char, vector<string>> registers;  // handles copied lines, etc
        unordered_map<string, int> marks;   // handles saved line positions



        bool executeCommands(Command &c, vector<string> &lines);
        Executor() {}
        Executor(bool debug_) : debug(debug_) {}
        Executor(const string &fname) : filename(fname) {}
        Executor(const string &fname, bool debug_) : debug(debug_), filename(fname) {}




        bool cmd_delete(Command c, vector<string> &lines, int &dot, bool &modified);
        bool cmd_append(Command c, vector<string> &lines, int &dot, bool &modified);
        bool cmd_insert(Command c, vector<string> &lines, int &dot, bool &modified);
        bool cmd_write(Command c, vector<string> &lines, string &params, string &filename, bool &modified) ;


        bool cmd_put(Command c, vector<string> &lines, int &dot);
        bool cmd_put_with_numbers(Command c, vector<string> &lines, int &dot);


        bool cmd_put_register(Command c,
                vector<string> &lines, 
                int &dot, 
                bool &modified, 
                char reg = '"', 
                bool after = true);

        bool cmd_yank(Command c,
                vector<string> &lines, 
                char reg = '"');


        bool cmd_undo(vector<string> &lines) ;
        bool cmd_redo(vector<string> &lines) ;


};



