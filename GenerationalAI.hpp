
#pragma once

#include <vector>
#include <string>

using std::vector;
using std::string;



// instance of an AI competitor
// abstract class with with the GenerationalAI works
class AIInstance
{
public:
    virtual ~AIInstance();

    int get_score() const;
    friend bool operator< (AIInstance const& a, AIInstance const& b);

    virtual string info() const = 0;

    // virtual function responsible for executing the AI and setting its own score
    virtual void run () = 0;
    // virtual function responsible for replicating and mutating this instance
    virtual AIInstance* create_child() const = 0;
    virtual AIInstance* clone() const = 0; // creates an identical copy of this object and returns it
protected:
    // setter that must be called by run
    void set_score(int score);
private:
    int score = 0;
};

// a generational ai container
// contains AIInstances which compete each generation for chance to reproduce
class GenerationalAI
{
public:
    // initializer which takes an function for deciding the next generation and a vector of initial competitors
    GenerationalAI(vector<AIInstance*> (*next_generation_function)(vector<AIInstance*>& instances), vector<AIInstance*> initial);
    // runs a single generation on the competitors
    void run_generation();

    virtual void print_scores(unsigned int number) const;
    virtual int best_score() const;

protected:
    // internal methods for completing subtasks of a generation
    virtual void run_instances ();
    virtual void rank_instances ();
    virtual void generate_next_generation ();

    vector<AIInstance*> (*next_generation_function)(vector<AIInstance*>& instances);
    vector<AIInstance*> instances;
    vector<AIInstance*> last_generation;
};

// simple next generation function which reproduces the top quartile of instances
vector<AIInstance*> generation_duplicate_top_quartile (vector<AIInstance*>& instances);



// an extension of the generational ai which implements multiple sectioned pools where instances only compete inside their pool
// only occasionally transferring some instances among pools
class PartitionedGenerationalAI : public GenerationalAI
{
public:
    PartitionedGenerationalAI(vector<AIInstance*> (*next_generation_function)(vector<AIInstance*>& instances),
        vector<AIInstance*> initial, unsigned int pool_count);

    virtual void print_scores(unsigned int number) const;
    virtual int best_score() const;

protected:
    virtual void run_instances ();
    virtual void rank_instances ();
    virtual void generate_next_generation ();

    vector<vector<AIInstance*>> instance_pools;
    vector<vector<AIInstance*>> last_generations;
};







