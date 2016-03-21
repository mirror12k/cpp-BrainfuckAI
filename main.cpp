

#include <iostream>

using std::cout;
using std::endl;

#include "brainfuck.hpp"

int main (int argc, char** argv)
{

    cout << "hello world" << endl;

    string output = brainfuck_run(" ++++++ [->++++++++?<] > +++ . -- . ++ . ++++ . ---- . [-]", 10000);

    cout << "got output: " << output << endl;

    return 0;
}


