//
// Created by daniel on 15/06/23.
//

#ifndef CODE_GWO_LIB_H
#define CODE_GWO_LIB_H

#include "random.hpp"

#include <utility>
#include <vector>
#include <functional>

#define POP_SIZE 50


using Random = effolkronium::random_static;


struct wolf {

    double fitness = 0.0;
    std::vector<double> sol;

    wolf(std::vector<double> &sol, double fitnes) : fitness(fitnes), sol(sol) {};

    wolf(std::vector<double> &sol) : fitness(0.0), sol(sol) {};


    bool operator<(const wolf &rhs) const {
        return fitness < rhs.fitness;
    }

    bool operator>(const wolf &rhs) const {
        return (rhs < *this);
    }

    bool operator<=(const wolf &rhs) const {
        return (this->fitness <= rhs.fitness);
    }

    bool operator>=(const wolf &rhs) const {
        return (rhs.fitness <= this->fitness);
    }

    bool operator==(const wolf &rhs) const {
        return this->sol == rhs.sol;
    }

    bool operator!=(const wolf &rhs) const {
        return rhs.fitness != this->fitness;
    }


    wolf &operator=(const wolf &rhs) {
        this->fitness = rhs.fitness;
        this->sol = rhs.sol;
        return *this;
    }


    wolf &operator=(std::vector<double> rhs) {
        this->sol = std::move(rhs);
        fitness = 0;
        return *this;
    }


};


std::vector<double>
gwo(const std::function<double(double *)> &fitnes, double min, double max, int dim, int MAX_EVALS, bool concurrent=false);

#endif //CODE_GWO_LIB_H
