//
// Created by daniel on 15/06/23.
//
extern "C" {
#include "cec17.h"
}

#include <iostream>
#include <vector>
#include "gwo_lib.h"

using namespace std;


int main() {
    std::vector<double> sol;
    int dim = 10;

    for (int funcid = 1; funcid <= 30; funcid++) {
        sol.resize(dim);
        double fitness;

        cec17_init("gwo", funcid, dim);

        sol = gwo(cec17_fitness, -100.0, 100.0, dim, 10000 * dim,true);

        cout << "" << endl;

        fitness = cec17_fitness(&sol[0]);


        cout << "Fitness[F" << funcid << "]: " << scientific << cec17_error(fitness) << endl;
    }


}


void delete_sol(vector<double *> &sol) {
    for (int i = 0; i < sol.size(); i++) {
        delete sol[i];
    }
    sol.clear();
}



