

#include <iostream>

using std::cout;
using std::endl;

#include "brainfuck.hpp"
#include "GenerationalAI.hpp"
#include "BrainfuckAI.hpp"




int main (int argc, char** argv)
{

    srand(time(NULL));

//    string output = brainfuck_run(" ++++++ [->++++++++<] > +++ . -- . ++ . ++++ . ---- .", 10000);

//    cout << "got output: " << output << endl;

    string start_code = "++++++++++++++++++";
//    string start_code = "< [..>[- <>..]--+- ..-].+ -<[ >>.-]][-..[ -[>>]+<>>>-.><<<->]]<+++++++++[ +++[++[+<[--< < ]-[>[+<. .>][>---+]+[ .> [-[][>]<<+[+>>>+. +-.+..+>+..[]+]- >++[-<.]].   +< [<][+].<][][].. [.+. ><<<[. [..[+ <+]-.[->][>] ][< ]-+-.+ [> +-+-+->>-<+.>[+[+.>- ++> ]<] ]].+. [>[><>][[ ..]< << +[[+<-.+  [>>>+>>  -. ]].[+->.]>.  >]--.[-[ -[ ]<><]>--+.]<+< <+-[<+<>>>[.]> ><>>]-]<.--+-[]] ]]-->+++++++++[+++ +[++++ ++><++[-++++ +[++++[++ [++.+. [+.+ .[+.+[. + [><.+. +..----><----.[ +.+.+ . +.+. +.+.+.+.---------.]";
    string expected = "01234567890123456789012345678901234567890123456789";


    BrainfuckAIInstance inst(start_code, expected);
    vector<AIInstance*> children;
    for (int i = 0; i < 40; i++)
    {
        children.push_back(inst.create_child());
    }

//    GenerationalAI ai(generation_duplicate_top_quartile, children);
    PartitionedGenerationalAI ai(generation_duplicate_top_quartile, children, 4);
    ai.run_generation(); // first generation to set values

    int generation = 1;

    while (ai.best_score() > 0)
    {
        ai.run_generation();
        if (generation++ % 50 == 0)
        {
            cout << "generation " << generation << endl;
            ai.print_scores(1);
        }
    }
    ai.print_scores(5);


    cout << "end\n" << endl;

    return 0;
}


