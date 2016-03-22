
#pragma once

#include <string>

using std::string;

// runs the given brainfuck code up to a maximum of max_steps instructions
// returns all output created by the '.' instruction, concatentated together
string brainfuck_run(int max_steps, const string& code, const string& input="");


