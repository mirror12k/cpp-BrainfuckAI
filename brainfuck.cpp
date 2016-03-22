

#include "brainfuck.hpp"

#include <stack>

#include <stdio.h>

using std::stack;


string brainfuck_run(const string& code, int max_steps)
{
    // initialize runtime
    stack<string::const_iterator> call_stack; // stack of loops entered

    // initialize buffer
    const unsigned int buffer_size = 8 * 4096;
    const unsigned int buffer_mask = buffer_size - 1;
    char buffer[buffer_size];
    for (unsigned int i = 0; i < buffer_size; i++)
        buffer[i] = 0; // set all buffer memory to zero
    unsigned int buffer_pointer = 0;

    int steps_taken = 0;

    string output = "";

    string::const_iterator current_end = code.end(); // current end of code that we are looking out for

    // run the brainfuck code
    for (string::const_iterator iter = code.begin(); iter != current_end; iter++)
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
        case '[': // conditional loop
        {
//            printf("start [\n");
            if (buffer[buffer_pointer] == 0)
            {
                int nest_depth = 1;
//                printf("mid [ %c loop\n", *iter);
                for (iter++; nest_depth > 0 && iter != current_end; iter++)
                {
//                    if (iter == current_end)
//                        printf("debug nest_depth %d\n", nest_depth);
                    if (*iter == '[')
                        nest_depth++;
                    else if (*iter == ']')
                        nest_depth--;
                }
//                printf("mid [ %c loop end\n", *iter);
                if (nest_depth > 0 || iter == current_end)
                    return output; // improveme
            }
            else
            {
//                printf("other mid [\n");
                call_stack.push(iter - 1);
            }
//            printf("end [\n");
            break;
        }
        case ']':
        {
            if (call_stack.size() == 0)
                return output; // improveme

            if (buffer[buffer_pointer] != 0)
            {
                iter = call_stack.top();
            }
            call_stack.pop();
            break;
        }
        case '.': // output
            output += buffer[buffer_pointer];
            break;
        case '?':
            printf("debug: %i\n", buffer[buffer_pointer]);
            break;
        default: // ignore anything else
            break;
        }

        if (iter == current_end)
        {
            printf("this is the bug\n");
        }
    }

    return output;
}


