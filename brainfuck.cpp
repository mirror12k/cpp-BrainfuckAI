

#include "brainfuck.hpp"

#include <stack>
#include <exception>
#include <stdexcept>

using std::stack;
using std::runtime_error;


string brainfuck_run(const string& code, int max_steps)
{
    // initialize runtime
    stack<string::const_iterator> call_stack; // stack of loops entered
    stack<string::const_iterator> end_stack; // for future use

    // initialize buffer
    unsigned int buffer_size = 8 * 4096;
    unsigned int buffer_mask = buffer_size - 1;
    char buffer[buffer_size];
    unsigned int buffer_pointer = 0;

    int steps_taken = 0;

    string output = "";

    string::const_iterator current_end = code.cend(); // current end of code that we are looking out for

    // run the brainfuck code
    for (string::const_iterator iter = code.cbegin(); iter != current_end; iter++)
    {
        if (++steps_taken == max_steps) // ensure we haven't taken too many steps
            break;

        switch (*iter) // interpret character
        {
        case '+': // increment value
            buffer[buffer_pointer]++;
            break;
        case '-': // decrement value
            buffer[buffer_pointer]--;
            break;
        case '>': // increment pointer
            buffer_pointer = buffer_mask & (buffer_pointer + 1);
            break;
        case '<': // decrement pointer
            buffer_pointer = buffer_mask & (buffer_pointer - 1);
            break;
        case '.': // output
            output += buffer[buffer_pointer];
            break;
        default: // ignore anything else
            break;
        }
    }

    return output;
}


