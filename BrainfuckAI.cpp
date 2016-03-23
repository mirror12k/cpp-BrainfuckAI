
#include "BrainfuckAI.hpp"
#include "brainfuck.hpp"


#include <cstdlib>
#include <algorithm>

#include <iostream>
using std::cout;
using std::endl;




BrainfuckAIInstance::BrainfuckAIInstance(const string& code) : code(code)
{}

BrainfuckAIInstance::~BrainfuckAIInstance() {}

int BrainfuckAIInstance::create_score(const string& expected, const string& output)
{
    int score = 0;
    string::const_iterator output_iter = output.begin();
    string::const_iterator expected_iter = expected.begin();
    while (expected_iter != expected.end() && output_iter != output.end())
    {
        score += 0xff & (*expected_iter++ - *output_iter++);
    }
    while (expected_iter != expected.end())
    {
        expected_iter++;
        score += 0xff;
    }
    if (score == 0)
    {
//        cout << "debug 0: " << expected << " <> " << output << endl;
    }
    return score;
}

char BrainfuckAIInstance::new_instruction () const
{
    switch (rand() % 9)
    {
    case 0:
        return '+';
    case 1:
        return '-';
    case 2:
        return '>';
    case 3:
        return '<';
    case 4:
        return '[';
    case 5:
        return ']';
    case 6:
        return '.';
    case 7:
        return ',';
    case 8:
        return ' ';
    }
    return '?';
}

string BrainfuckAIInstance::info() const
{
    return string("") + std::to_string(this->get_score()) + " : '" + this->code + "'";
}

void BrainfuckAIInstance::run(const string& input, const string& expected)
{
    string output = brainfuck_run(5000, this->code, input);
    this->set_score(this->create_score(expected, output));
}


AIInstance* BrainfuckAIInstance::create_child() const
{
    string new_code = this->code;

    for (int i = 0; i < 10; i++)
    {
        if (new_code.size() > 0)
            if (rand() % 15 == 0) {
                new_code[rand() % new_code.size()] = this->new_instruction();
            }
        if (new_code.size() > 0)
            if (rand() % 24 == 0)
            {
                new_code.erase(rand() % new_code.size());
            }
        if (new_code.size() < 500)
            if (rand() % 24 == 0)
            {
                if (new_code.size() == 0)
                    new_code += this->new_instruction();
                else
                    new_code.insert(rand() % new_code.size(), 1, this->new_instruction());
            }
        if (new_code.size() > 2)
            if (rand() % 48 == 0)
            {
                unsigned int index = rand() % new_code.size();
                unsigned int transplant_size = rand() % (new_code.size() - index + 1);
                if (transplant_size == 0)
                    transplant_size = 1;
                if (transplant_size == new_code.size())
                    transplant_size--;
                unsigned int transplant_index = rand() % (new_code.size() - transplant_size);

                string transplant = new_code.substr(index, transplant_size);
                new_code.erase(index, transplant_size);
                new_code.insert(transplant_index, transplant);
            }
    }

    return new BrainfuckAIInstance(new_code);
}

AIInstance* BrainfuckAIInstance::clone() const
{
    return new BrainfuckAIInstance(this->code);
}



tuple<string, string> challenge_hello_world()
{
    return std::make_tuple("", "hello world!");
}

tuple<string, string> challenge_1234()
{
    return std::make_tuple("", "1234");
}

vector<string> challenge_strings_4 = {
    "asdf",
    "qwer",
    "tyui",
    "uiop",
    "kwdh",
    "hjkl",
    "abdc",
    "zxcv",
    "nerf",
    "now!",
    "4123",
    "8537",
    "1752",
    "mnbv",
    "helt",
    "lehl",
    "magi",
    "test",
    "fire",
    "boat",
    "comt",
    "left",
    "rite",
    "ripe",
    "many",
    "whom",
    "howo",
    "cann",
    "say!",
    "FEWR",
};

tuple<string, string> challenge_reverse_4()
{
    string input = challenge_strings_4[rand() % challenge_strings_4.size()];
    string expected = input;
    std::reverse(expected.begin(), expected.end());
    return std::make_tuple(input, expected);
}


tuple<string, string> challenge_reverse_8()
{
    string input = challenge_strings_4[rand() % challenge_strings_4.size()];
    string second = input;
    while(second == input)
        second = challenge_strings_4[rand() % challenge_strings_4.size()];

    input += second; // append another to double the difficulty
    string expected = input;
    std::reverse(expected.begin(), expected.end());
//    cout << "debug: " << input << " => " << expected << endl;
    return std::make_tuple(input, expected);
}



tuple<string, string> challenge_addition()
{
    char val1 = rand() % 128;
    char val2 = rand() % 128;
    string input;
    input += val1;
    input += val2;
    string expected;
    expected += (val1 + val2);
    return std::make_tuple(input, expected);
}


