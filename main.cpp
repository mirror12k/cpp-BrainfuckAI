

#include <iostream>

using std::cout;
using std::endl;

#include "brainfuck.hpp"
#include "GenerationalAI.hpp"
#include "BrainfuckAI.hpp"




int main (int argc, char** argv)
{

    srand(time(NULL));

//    cout << "hello world" << endl;

//    string output = brainfuck_run(" ++++++ [->++++++++<] > +++ . -- . ++ . ++++ . ---- .", 10000);
//    string output = brainfuck_run("[---<.]", 10000);

//    cout << "got output: " << output << endl;

    string start_code = "++++++++++++++++++";
    string expected = "0123456789";


    BrainfuckAIInstance inst(start_code, expected);
    vector<AIInstance*> children;
    for (int i = 0; i < 100; i++)
    {
        children.push_back(inst.create_child());
    }

    GenerationalAI ai(generation_duplicate_top_quartile, children);
    ai.run_generation(); // first generation to set values

    int generation = 1;

    while (ai.best_score() > 10)
    {
        ai.run_generation();
        if (generation++ % 50 == 0)
        {
            cout << "generation " << generation << endl;
            ai.print_scores();
        }
    }
    ai.print_scores();


    cout << "end\n" << endl;

    return 0;
}


