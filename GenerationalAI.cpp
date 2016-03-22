
#include "GenerationalAI.hpp"



#include <iostream>
#include <algorithm>
#include <cstdlib>

using std::cout;
using std::endl;
using std::sort;


AIInstance::~AIInstance() {}

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

int GenerationalAI::best_score() const
{
    if (this->last_generation.size() == 0)
        return -1;
    else
        return this->last_generation[0]->get_score();

}

void GenerationalAI::print_scores(unsigned int number) const
{
    if (this->last_generation.size() >= number)
        for (unsigned int i = 0; i < number; i++)
            cout << this->last_generation[i]->info() << endl;
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
    for (vector<AIInstance*>::iterator iter = this->last_generation.begin(); iter != this->last_generation.end(); iter++)
        delete *iter;
    this->last_generation.clear();

    this->last_generation = this->instances;
    this->instances = this->next_generation_function(this->instances);
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

vector<AIInstance*> generation_duplicate_top_quartile_with_ancestor (vector<AIInstance*>& instances)
{
    vector<AIInstance*> new_instances;
    vector<AIInstance*>::const_iterator midpoint = instances.begin() + (instances.size() / 4);
    for (vector<AIInstance*>::const_iterator iter = instances.begin(); iter != midpoint; iter++)
    {
        if (rand() % 4)
            new_instances.push_back((*iter)->clone());
        else
            new_instances.push_back((*iter)->create_child());
        new_instances.push_back((*iter)->create_child());
        new_instances.push_back((*iter)->create_child());
        new_instances.push_back((*iter)->create_child());
    }
    return new_instances;
}









PartitionedGenerationalAI::PartitionedGenerationalAI(vector<AIInstance*> (*next_generation_function)(vector<AIInstance*>& instances),
    vector<AIInstance*> initial, unsigned int pool_count)
    : GenerationalAI(next_generation_function, vector<AIInstance*>())
{
    for (unsigned int i = 0; i < pool_count; i++)
    {
        vector<AIInstance*> pool;
        for (vector<AIInstance*>::const_iterator iter = initial.begin(); iter != initial.end(); iter++)
            pool.push_back((*iter)->clone());
        this->instance_pools.push_back(pool);
    }
}



void PartitionedGenerationalAI::print_scores(unsigned int number) const
{
    if (this->last_generations.size() >= number)
        for (unsigned int pool_index = 0; pool_index < this->last_generations.size(); pool_index++)
            for (unsigned int i = 0; i < number; i++)
                cout << pool_index << "# " << this->last_generations[pool_index][i]->info() << endl;
}

int PartitionedGenerationalAI::best_score() const
{
    if (this->last_generations.size() == 0)
    {
        return -1;
    }
    else
    {
        int best = this->last_generations[0][0]->get_score();
        for (vector<vector<AIInstance*>>::const_iterator pool = this->last_generations.begin(); pool != this->last_generations.end(); pool++)
        {
            if ((*pool)[0]->get_score() < best)
            {
                best = (*pool)[0]->get_score();
            }
        }
        return best;
    }
}

void PartitionedGenerationalAI::run_instances()
{
    for (vector<vector<AIInstance*>>::iterator pool = this->instance_pools.begin(); pool != this->instance_pools.end(); pool++)
        for (vector<AIInstance*>::iterator iter = pool->begin(); iter != pool->end(); iter++)
            (*iter)->run();
}

void PartitionedGenerationalAI::rank_instances()
{
    for (vector<vector<AIInstance*>>::iterator pool = this->instance_pools.begin(); pool != this->instance_pools.end(); pool++)
        sort(pool->begin(), pool->end(), compareAIInstances);
}


void PartitionedGenerationalAI::generate_next_generation()
{
    // delete the previous generation
    for (vector<vector<AIInstance*>>::iterator pool = this->last_generations.begin(); pool != this->last_generations.end(); pool++)
        for (vector<AIInstance*>::iterator iter = pool->begin(); iter != pool->end(); iter++)
            delete *iter;
    this->last_generations.clear();

    this->last_generations = this->instance_pools;
    // generate the next generation
    for (vector<vector<AIInstance*>>::iterator pool = this->instance_pools.begin(); pool != this->instance_pools.end(); pool++)
        *pool = this->next_generation_function(*pool);

    // random chance to perform cross-placement of instances across pools
    if (rand() % 1000 == 0)
    {
        for (int source_pool = 0; source_pool < this->instance_pools.size(); source_pool++)
        {
            // index two instances from different pools
            int source_index = rand() % this->instance_pools[source_pool].size();
            int target_pool = rand() % this->instance_pools.size();
            int target_index = rand() % this->instance_pools[target_pool].size();
//            printf("cross-polliating #%d[%d] with #%d[%d]\n", source_pool, source_index, target_pool, target_index);

            // perform a swap of the two instances
            AIInstance* inst = this->instance_pools[source_pool][source_index];
            this->instance_pools[source_pool][source_index] = this->instance_pools[target_pool][target_index];
            this->instance_pools[target_pool][target_index] = inst;
        }
    }
}



