

#include <iostream>

using std::cout;
using std::endl;

#include "brainfuck.hpp"
#include "GenerationalAI.hpp"
#include "BrainfuckAI.hpp"




int main (int argc, char** argv)
{

    srand(time(NULL));

//    string output = brainfuck_run(10000, ",[[[[+>---[,+[>++[+[-[<[>--]+++++++[[-----[[,+>-[<[>+[<-,+,]-,[.+[[<[>>-,+>-,+>+,[.[[,+[>+<[+++++++++++[++++[[+++++++++[++[+++++++++[++++++++[[<-.]+", "testvall");
//    cout << "got output: " << output << endl;

    string start_code = "++++++++++++++++++";
//    string start_code = "< [..>[- <>..]--+- ..-].+ -<[ >>.-]][-..[ -[>>]+<>>>-.><<<->]]<+++++++++[ +++[++[+<[--< < ]-[>[+<. .>][>---+]+[ .> [-[][>]<<+[+>>>+. +-.+..+>+..[]+]- >++[-<.]].   +< [<][+].<][][].. [.+. ><<<[. [..[+ <+]-.[->][>] ][< ]-+-.+ [> +-+-+->>-<+.>[+[+.>- ++> ]<] ]].+. [>[><>][[ ..]< << +[[+<-.+  [>>>+>>  -. ]].[+->.]>.  >]--.[-[ -[ ]<><]>--+.]<+< <+-[<+<>>>[.]> ><>>]-]<.--+-[]] ]]-->+++++++++[+++ +[++++ ++><++[-++++ +[++++[++ [++.+. [+.+ .[+.+[. + [><.+. +..----><----.[ +.+.+ . +.+. +.+.+.+.---------.]";
//    string expected = "hello world";


    BrainfuckAIInstance inst(start_code);
    vector<AIInstance*> children;
    for (int i = 0; i < 40; i++)
    {
        children.push_back(inst.create_child());
    }

//    GenerationalAI ai(generation_duplicate_top_quartile, children);
    PartitionedGenerationalAI ai(challenge_addition, generation_duplicate_top_quartile_with_ancestor, children, 8);
    ai.run_generation(); // first generation to set values

    int generation = 1;
    int generations_correct = 0;


    while (generations_correct < 20)
    {

        ai.run_generation();
        if (++generation % 100 == 0)
        {
            cout << "generation " << generation << endl;
            ai.print_scores(1);
        }

        if (ai.best_score() == 0)
            generations_correct++;
        else
            generations_correct = 0;
    }
    cout << "final contenders:" << endl;
    ai.print_scores(3);


    cout << "end\n" << endl;

    return 0;
}


