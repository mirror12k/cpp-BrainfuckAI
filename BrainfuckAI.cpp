
#include "BrainfuckAI.hpp"
#include "brainfuck.hpp"


#include <cstdlib>





BrainfuckAIInstance::BrainfuckAIInstance(string& code, const string& expected) : code(code), expected(expected)
{}

BrainfuckAIInstance::~BrainfuckAIInstance() {}

int BrainfuckAIInstance::create_score(const string& output)
{
    int score = 0;
    string::const_iterator output_iter = output.begin();
    string::const_iterator expected_iter = this->expected.begin();
    for (; expected_iter != this->expected.end() && output_iter != output.end(); expected_iter++, output_iter++)
    {
        score += 0xff & (*expected_iter - *output_iter);
    }
    while (expected_iter != this->expected.end())
    {
        expected_iter++;
        score += 0xff;
    }
    return score;
}

char BrainfuckAIInstance::new_instruction () const
{
    switch (rand() % 8)
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
        return ' ';
    }
    return '?';
}

string BrainfuckAIInstance::info() const
{
    return string("") + std::to_string(this->get_score()) + " : '" + this->code + "'";
}

void BrainfuckAIInstance::run()
{
//    printf("debug start run\n");
    string output = brainfuck_run(this->code, 5000);
//    printf("debug mid run\n");
    this->set_score(this->create_score(output));
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
                int index = rand() % new_code.size();
                int transplant_size = rand() % (new_code.size() - index + 1);
                if (transplant_size == 0)
                    transplant_size = 1;
                if (transplant_size == new_code.size())
                    transplant_size--;
                int transplant_index = rand() % (new_code.size() - transplant_size);

                string transplant = new_code.substr(index, transplant_size);
                new_code.erase(index, transplant_size);
                new_code.insert(transplant_index, transplant);
            }
    }

    return new BrainfuckAIInstance(new_code, this->expected);
}







