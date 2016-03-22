
#pragma once


#include "GenerationalAI.hpp"



class BrainfuckAIInstance : public AIInstance
{
public:
    BrainfuckAIInstance(string& code, const string& expected);
    virtual ~BrainfuckAIInstance();

    virtual int create_score (const string& output);

    virtual char new_instruction () const;

    virtual string info() const;

    virtual void run ();
    virtual AIInstance* create_child() const;


//private:
    string code;
    const string expected;
};



