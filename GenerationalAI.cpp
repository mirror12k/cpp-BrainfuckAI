
#include "GenerationalAI.hpp"



#include <algorithm>

using std::sort;


int AIInstance::get_score() const
{
    return this->score;
}

void AIInstance::set_score(int score)
{
    this->score = score;
}

bool operator< (AIInstance const& a, AIInstance const& b)
{
    return a.get_score() < b.get_score();
}






GenerationalAI::GenerationalAI(vector<AIInstance*> (*next_generation_function)(vector<AIInstance*>& instances), vector<AIInstance*> initial)
: next_generation_function(next_generation_function), instances(initial) {}


bool compareAIInstances(const AIInstance* const a, const AIInstance* const b)
{
    return *a < *b;
}

void GenerationalAI::run_generation()
{
    this->run_instances();
    this->rank_instances();
    this->generate_next_generation();
}


void GenerationalAI::run_instances ()
{
    for (vector<AIInstance*>::iterator iter = this->instances.begin(); iter != this->instances.end(); iter++)
        (*iter)->run();
}

void GenerationalAI::rank_instances ()
{
    sort(this->instances.begin(), this->instances.end(), compareAIInstances);
}

void GenerationalAI::generate_next_generation ()
{
    vector<AIInstance*> old_instances = this->instances;
    this->instances = this->next_generation_function(this->instances);
    while (old_instances.size() > 0)
    {
        delete old_instances.back();
        old_instances.pop_back();
    }
}





vector<AIInstance*> generation_duplicate_top_quartile (vector<AIInstance*>& instances)
{
    vector<AIInstance*> new_instances;
    vector<AIInstance*>::const_iterator midpoint = instances.begin() + (instances.size() / 4);
    for (vector<AIInstance*>::const_iterator iter = instances.begin(); iter != midpoint; iter++)
    {
        new_instances.push_back((*iter)->create_child());
        new_instances.push_back((*iter)->create_child());
        new_instances.push_back((*iter)->create_child());
        new_instances.push_back((*iter)->create_child());
    }
    return new_instances;
}

