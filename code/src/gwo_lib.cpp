//
// Created by daniel on 15/06/23.
//

#include "gwo_lib.h"
#include <iostream>
#include "explotacion.h"


std::vector<wolf> genera_soluciones_aleatorias(int N_soluciones, int dim, double min, double max) {
    std::vector<wolf> ret;
    ret.reserve(N_soluciones);
    for (int i = 0; i < N_soluciones; i++) {
        std::vector<double> sol = Random::get<std::vector>(min, max, dim);

        ret.emplace_back(sol);
    }

    return ret;
}


void
actualiza_lobos(int dim, std::vector<wolf> &agentes, const wolf *alpha, const wolf *beta, const wolf *delta, double a,
                double (*AUpdate)(double &)) {

    for (int lobo = 0; lobo < POP_SIZE; lobo++) {


        for (auto it = 0; it < dim; it++) {
            double A1, C1, A2, C2, A3, C3;


            A1 = AUpdate(a);
            C1 = 2 * Random::get(0.0, 1.0);
            A2 = AUpdate(a);
            C2 = 2 * Random::get(0.0, 1.0);
            A3 = AUpdate(a);
            C3 = 2 * Random::get(0.0, 1.0);


            double D_alpha = std::abs(C1 * alpha->sol[it] - agentes[lobo].sol[it]);
            double X1 = alpha->sol[it] - A1 * D_alpha;
            double D_beta = std::abs(C2 * beta->sol[it] - agentes[lobo].sol[it]);
            double X2 = beta->sol[it] - A2 * D_beta;
            double D_delta = std::abs(C3 * delta->sol[it] - agentes[lobo].sol[it]);
            double X3 = delta->sol[it] - A3 * D_delta;
            agentes[lobo].sol[it] = (X1 + X2 + X3) / 3.0;
        }
    }
}


// // Crea una solucion aleatoria
// wolf genera_solucion_aleatoria(int dim, double min, double max) {
//     wolf solucion;
//     solucion.sol.reserve(dim);
//     for (int i = 0; i < dim; i++) {
//         solucion.sol[i] = Random::get(min, max);
//     }
//     return solucion;
// }

// // Crea una poblacion aleatoria
// std::vector<wolf> genera_soluciones_aleatorias(int size, int dim, double min, double max) {
//     std::vector<wolf> poblacion(size);
//     for (int i = 0; i < size; i++) {
//         poblacion[i] = genera_solucion_aleatoria(dim, min, max);
//     }
//     return poblacion;
// }

// Actualiza la posicion de los lobos
void actualiza_lobos(int dim, std::vector<wolf> &agentes, wolf *alpha, wolf *beta, wolf *delta, double a,
                     std::function<double(double &)> AUpdate) {
    for (int lobo = 0; lobo < POP_SIZE; lobo++) {


        for (auto it = 0; it < dim; it++) {
            double A1, C1, A2, C2, A3, C3;


            A1 = AUpdate(a);
            C1 = 2 * Random::get(0.0, 1.0);
            A2 = AUpdate(a);
            C2 = 2 * Random::get(0.0, 1.0);
            A3 = AUpdate(a);
            C3 = 2 * Random::get(0.0, 1.0);


            double D_alpha = std::abs(C1 * alpha->sol[it] - agentes[lobo].sol[it]);
            double X1 = alpha->sol[it] - A1 * D_alpha;
            double D_beta = std::abs(C2 * beta->sol[it] - agentes[lobo].sol[it]);
            double X2 = beta->sol[it] - A2 * D_beta;
            double D_delta = std::abs(C3 * delta->sol[it] - agentes[lobo].sol[it]);
            double X3 = delta->sol[it] - A3 * D_delta;
            agentes[lobo].sol[it] = (X1 + X2 + X3) / 3.0;
        }
    }
}


std::vector<double>
gwo(const std::function<double(double *)> &fitnes, double min, double max, int dim, int MAX_EVALS, bool concurrent) {
    // inicializar  el problema (Tamaño de población y número de iteraciones generar lambdas para las funciones)


    std::vector<wolf> agentes;
    agentes = genera_soluciones_aleatorias(POP_SIZE, dim, min, max);

    const int MAX_ITERATIONS = MAX_EVALS / POP_SIZE;

    auto update_fitnes = [&fitnes](wolf &agente) -> void {
        agente.fitness = fitnes(&agente.sol[0]);
    };

    std::for_each(agentes.begin(), agentes.end(), update_fitnes);


    for (int i = 0; i < POP_SIZE; i++) {
        update_fitnes(agentes[i]);
    }


    std::sort(agentes.begin(), agentes.end());

    wolf *alpha, *beta, *delta;
    alpha = &agentes[0];
    beta = &agentes[0] + 1;
    delta = &agentes[0] + 2;

    double a;
    auto aUpdate = [&MAX_ITERATIONS](int iter) -> double {
        return 2 * (1 - (double(iter) / MAX_ITERATIONS));
    };

    auto AUpdate = [](double &a) -> double {
        return 2 * a * Random::get(0.0, 1.0) - a;
    };


    int evalCount = 1;

    while (evalCount < MAX_ITERATIONS) {
        a = aUpdate(evalCount);
        evalCount++;


        actualiza_lobos(dim, agentes, alpha, beta, delta, a, AUpdate);

//Actualizamos fitnes de la nueva posición de los lobos
        std::for_each(agentes.begin(),agentes.end(),update_fitnes);

//Los ordenamos en función de su fitnes para obtener la cuspide de la gerarquia
        std::sort(agentes.begin(), agentes.end());

        alpha = &agentes[0];
        beta = &agentes[1];
        delta = &agentes[2];


    }
    auto ret = agentes[0].sol;

    return ret;
}

