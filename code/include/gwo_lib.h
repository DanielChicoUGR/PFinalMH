//
// Created by daniel on 15/06/23.
//

#ifndef CODE_GWO_LIB_H
#define CODE_GWO_LIB_H

#include "random.hpp"

#include <utility>
#include <vector>
#include <functional>


#define POP_SIZE 10

using Random = effolkronium::random_static;

struct wolf
{

    double fitness = 0.0;
    std::vector<double> sol;

    wolf(std::vector<double> &sol, double fitnes) : fitness(fitnes), sol(sol){};

    wolf(std::vector<double> &sol) : fitness(0.0), sol(sol){};

    wolf() : fitness(0.0), sol(){};

    bool operator<(const wolf &rhs) const
    {
        return fitness < rhs.fitness;
    }

    bool operator>(const wolf &rhs) const
    {
        return (rhs < *this);
    }

    bool operator<=(const wolf &rhs) const
    {
        return (this->fitness <= rhs.fitness);
    }

    bool operator>=(const wolf &rhs) const
    {
        return (rhs.fitness <= this->fitness);
    }

    bool operator==(const wolf &rhs) const
    {
        return this->sol == rhs.sol;
    }

    bool operator!=(const wolf &rhs) const
    {
        return rhs.fitness != this->fitness;
    }

    wolf &operator=(const wolf &rhs)
    {
        this->fitness = rhs.fitness;
        this->sol = rhs.sol;
        return *this;
    }

    wolf& operator+=(const wolf &rhs){
this->fitness=this->fitness+rhs.fitness;
return *this;         
    }

    wolf &operator=(std::vector<double> rhs)
    {
        this->sol = std::move(rhs);
        fitness = 0;
        return *this;
    }
};

/**
 * Runs the Grey Wolf optimizer for a given fitness function.
 *
 * @param fitnes The fitness function to optimize.
 * @param min The minimum value for the search space.
 * @param max The maximum value for the search space.
 * @param dim The dimensionality of the search space.
 * @param MAX_EVALS The maximum number of evaluations for the optimization.
 * @param concurrent (optional) Whether to run the optimization concurrently. Defaults to false.
 *
 * @return A vector of double values representing the best solution found by the optimizer.
 *
 * @throws std::invalid_argument if min is greater than or equal to max or if dim is less than or equal to zero.
 */
std::vector<double>
gwo(const std::function<double(double *)> &fitnes, double min, double max, int dim, int MAX_EVALS);


std::vector<double> gwo_hibrido(const std::function<double(double *)> &fitnes, double min, double max, int dim, int MAX_EVALS,bool es=false);

#endif // CODE_GWO_LIB_H
