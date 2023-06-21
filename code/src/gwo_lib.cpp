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


// Actualiza la posicion de los lobos
void actualiza_lobos(int dim, std::vector<wolf> &agentes, wolf *alpha, wolf *beta, wolf *delta, double a,
                     const std::function<double(double &)>& AUpdate) {
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
gwo(const std::function<double(double *)> &fitnes, double min, double max, int dim, int MAX_EVALS) {
    // inicializar  el problema (Tamaño de población y número de iteraciones generar lambdas para las funciones)


    std::vector<wolf> agentes;
    agentes = genera_soluciones_aleatorias(POP_SIZE, dim, min, max);

    const int MAX_ITERATIONS = MAX_EVALS / POP_SIZE;

    auto update_fitnes = [&fitnes](wolf &agente) -> void {
        agente.fitness = fitnes(&agente.sol[0]);
    };

    std::for_each(agentes.begin(), agentes.end(), update_fitnes);

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

std::vector<double>
gwo_hibrido(const std::function<double(double *)> &fitnes, double min, double max, int dim, int MAX_EVALS, bool es) {
    {
        // inicializar  el problema (Tamaño de población y número de iteraciones generar lambdas para las funciones)


        std::vector<wolf> agentes;
        agentes = genera_soluciones_aleatorias(POP_SIZE, dim, min, max);

        const int MAX_ITERATIONS = MAX_EVALS / POP_SIZE;
        int eval=0;

        auto update_fitnes = [&fitnes, &eval](wolf &agente) -> void {
            agente.fitness = fitnes(&agente.sol[0]);
            eval++;
        };

        std::for_each(agentes.begin(), agentes.end(), update_fitnes);




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

        while ( eval < MAX_EVALS) {
            a = aUpdate(evalCount);



            actualiza_lobos(dim, agentes, alpha, beta, delta, a, AUpdate);

//Actualizamos fitnes de la nueva posición de los lobos
            std::for_each(agentes.begin(),agentes.end(),update_fitnes);

//Los ordenamos en función de su fitnes para obtener la cuspide de la gerarquia
            std::sort(agentes.begin(), agentes.end());

            alpha = &agentes[0];
            beta = &agentes[1];
            delta = &agentes[2];

            if(evalCount%20==0) {
                if (es) {
                    alpha->sol = Enfriamiento_simulado(fitnes, min, max, alpha->sol,200);
                    beta->sol = Enfriamiento_simulado(fitnes, min, max, beta->sol,200);
                    delta->sol = Enfriamiento_simulado(fitnes, min, max, delta->sol,200);
                } else {
                    alpha->sol = busqueda_local(fitnes, min, max, alpha->sol,200);
                    beta->sol = busqueda_local(fitnes, min, max, beta->sol,200);
                    delta->sol = busqueda_local(fitnes, min, max, delta->sol,200);
                }
                eval+=600;
            }




        }


        return std::min_element(agentes.begin(),agentes.end())->sol;
    }
}

