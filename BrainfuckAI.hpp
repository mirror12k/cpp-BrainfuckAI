
#pragma once


#include "GenerationalAI.hpp"



class BrainfuckAIInstance : public AIInstance
{
public:
    BrainfuckAIInstance(const string& code);
    virtual ~BrainfuckAIInstance();

    virtual int create_score (const string& expected, const string& output);

    virtual char new_instruction () const;

    virtual string info() const;

    virtual void run (const string& input, const string& expected);
    virtual AIInstance* create_child() const;
    virtual AIInstance* clone() const;


//private:
    const string code;
};


tuple<string, string> challenge_hello_world();
tuple<string, string> challenge_1234();

tuple<string, string> challenge_reverse_4();
tuple<string, string> challenge_reverse_8();

tuple<string, string> challenge_addition();



