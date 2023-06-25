//
// Created by daniel on 15/06/23.
//

#include "gwo_lib.h"
#include "explotacion.h"
#include <algorithm>
#include <exception>
#include <iostream>
#include <memory>

std::vector<std::shared_ptr<wolf>>
genera_soluciones_aleatorias(int N_soluciones, int dim, double min,
                             double max) {
    std::vector<std::shared_ptr<wolf>> ret;
    ret.reserve(N_soluciones);
    for (int i = 0; i < N_soluciones; i++) {
        std::vector<double> sol = Random::get<std::vector>(min, max, dim);
        auto aux = std::make_shared<wolf>(wolf(sol));
        ret.emplace_back(aux);
    }

    return ret;
}

double truncar(double d, double min, double max) {

    if (d < min) {
        d = min;
    } else if (d > max) {
        d = max;
    }
    return d;
}

// Actualiza la posicion de los lobos
void actualiza_lobos(int dim, std::vector<std::shared_ptr<wolf>> &agentes,
                     std::shared_ptr<wolf> alpha, std::shared_ptr<wolf> beta,
                     std::shared_ptr<wolf> delta, double a,
                     const std::function<double(double &)> &AUpdate) {
    for (int lobo = 3; lobo < POP_SIZE; lobo++) {

        for (auto it = 0; it < dim; it++) {
            double A1, C1, A2, C2, A3, C3;

            A1 = AUpdate(a);
            C1 = 2 * Random::get(0.0, 1.0);
            A2 = AUpdate(a);
            C2 = 2 * Random::get(0.0, 1.0);
            A3 = AUpdate(a);
            C3 = 2 * Random::get(0.0, 1.0);

            double D_alpha = std::abs(C1 * alpha->sol[it] - agentes[lobo]->sol[it]);
            double X1 = alpha->sol[it] - A1 * D_alpha;
            double D_beta = std::abs(C2 * beta->sol[it] - agentes[lobo]->sol[it]);
            double X2 = beta->sol[it] - A2 * D_beta;
            double D_delta = std::abs(C3 * delta->sol[it] - agentes[lobo]->sol[it]);
            double X3 = delta->sol[it] - A3 * D_delta;
            agentes[lobo]->sol[it] = (X1 + X2 + X3) / 3.0;
        }
    }
}

std::vector<double> gwo(const std::function<double(double *)> &fitnes,
                        double min, double max, int dim, int MAX_EVALS) {
    // inicializar  el problema (Tamaño de población y número de iteraciones
    // generar lambdas para las funciones)

    std::vector<std::shared_ptr<wolf>> agentes;
    agentes = genera_soluciones_aleatorias(POP_SIZE, dim, min, max);

    const int MAX_ITERATIONS = MAX_EVALS / POP_SIZE;

    auto update_fitnes = [&fitnes](std::shared_ptr<wolf> &agente) -> void {
        agente->fitness = fitnes(&agente->sol[0]);
    };

    std::for_each(agentes.begin(), agentes.end(), update_fitnes);

    std::sort(agentes.begin(), agentes.end());

    std::shared_ptr<wolf> alpha, beta, delta;
    alpha = agentes[0];
    beta = agentes[1];
    delta = agentes[2];

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

        // Actualizamos fitnes de la nueva posición de los lobos
        std::for_each(agentes.begin(), agentes.end(), update_fitnes);

        // Los ordenamos en función de su fitnes para obtener la cuspide de la
        // gerarquia
        std::sort(agentes.begin(), agentes.end());

        alpha = (agentes[0]);
        beta = (agentes[1]);
        delta = (agentes[2]);
    }

    return std::move(agentes[0]->sol);
}

std::vector<double> gwo_hibrido(const std::function<double(double *)> &fitnes,
                                double min, double max, int dim, int MAX_EVALS,
                                bool es) {
    {
        // inicializar  el problema (Tamaño de población y número de iteraciones
        // generar lambdas para las funciones)

        std::vector<std::shared_ptr<wolf>> agentes;
        agentes = genera_soluciones_aleatorias(POP_SIZE, dim, min, max);

        const int MAX_ITERATIONS = MAX_EVALS / POP_SIZE;
        int eval = 0;

        auto update_fitnes = [&fitnes,
                              &eval](std::shared_ptr<wolf> &agente) -> void {
            agente->fitness = fitnes(&agente->sol[0]);
            eval++;
        };
        std::for_each(agentes.begin(), agentes.end(), update_fitnes);

        std::sort(agentes.begin(), agentes.end());

        std::shared_ptr<wolf> alpha, beta, delta;
        alpha = (agentes[0]);
        beta = (agentes[1]);
        delta = (agentes[2]);

        double a;
        auto aUpdate = [&MAX_ITERATIONS](int iter) -> double {
            return 2 * (1 - (double(iter) / MAX_ITERATIONS));
        };

        auto AUpdate = [](double &a) -> double {
            return 2 * a * Random::get(0.0, 1.0) - a;
        };

        int evalCount = 1;

        while (eval < MAX_EVALS) {
            a = aUpdate(evalCount);

            actualiza_lobos(dim, agentes, alpha, beta, delta, a, AUpdate);

            // Actualizamos fitnes de la nueva posición de los lobos
            std::for_each(agentes.begin(), agentes.end(), update_fitnes);

            // Los ordenamos en función de su fitnes para obtener la cuspide de la
            // gerarquia
            std::sort(agentes.begin(), agentes.end());

            alpha = (agentes[0]);
            beta = (agentes[1]);
            delta = (agentes[2]);

            if (evalCount % 20 == 0) {
                if (es) {
                    std::tie(alpha->sol, alpha->fitness) = Enfriamiento_simulado(fitnes, min, max, alpha->sol, 200);
                    std::tie(beta->sol, beta->fitness) = Enfriamiento_simulado(fitnes, min, max, beta->sol, 200);
                    std::tie(delta->sol, delta->fitness) = Enfriamiento_simulado(fitnes, min, max, delta->sol, 200);
                } else {
                    std::tie(alpha->sol, alpha->fitness) = busqueda_local(fitnes, min, max, alpha->sol, 200);
                    std::tie(beta->sol, beta->fitness) = busqueda_local(fitnes, min, max, beta->sol, 200);
                    std::tie(delta->sol, delta->fitness) = busqueda_local(fitnes, min, max, delta->sol, 200);
                }
                eval += 600;
            }
        }

        return std::min_element(agentes.begin(), agentes.end())->get()->sol;
    }
}

std::vector<double> vmax(const std::vector<double> &v1,
                         const std::vector<double> &v2) {
    std::vector<double> tmp;
    tmp.reserve(v1.size());
    for (int i = 0; i < v1.size(); ++i) {
        if (v1[i] >= v2[i]) {
            tmp.push_back(v1[i]);
        } else {
            tmp.push_back(v2[i]);
        }
    }
    return tmp;
}

std::vector<double> vmin(const std::vector<double> &v1,
                         const std::vector<double> &v2) {
    std::vector<double> tmp;
    tmp.reserve(v1.size());
    for (int i = 0; i < v1.size(); ++i) {
        if (v1[i] <= v2[i]) {
            tmp.push_back(v1[i]);
        } else {
            tmp.push_back(v2[i]);
        }
    }
    return tmp;
}
std::pair<std::shared_ptr<wolf>, std::shared_ptr<wolf>>
BLX(const std::shared_ptr<wolf> &padre1, const std::shared_ptr<wolf> &padre2,
    double min, double max) {
    std::shared_ptr<wolf> hijo1, hijo2;
    hijo1 = std::make_shared<wolf>(padre1->sol);
    hijo2 = std::make_shared<wolf>(padre2->sol);

    auto coef_max = vmax(padre1.get()->sol, padre2.get()->sol);
    auto coef_min = vmin(padre1.get()->sol, padre2.get()->sol);
    double diff, interv_x, interv_y;

    for (int i = 0; i < coef_max.size(); i++) {

        // Calculo de I
        diff = coef_max[i] - coef_min[i];

        // Calculo de intervalos:
        interv_x = coef_min[i] - diff * ALFA_BLX;
        interv_y = coef_max[i] + diff * ALFA_BLX;

        // Se generan los valores de los hijos entre los intervalos calculados
        hijo1->sol[i] = truncar(Random::get(interv_x, interv_y), min, max);
        hijo2->sol[i] = truncar(Random::get(interv_x, interv_y), min, max);
    }

    return {std::move(hijo1), std::move(hijo2)};
}

void reproduccion(const std::function<double(double *)> &fitnes,
                  std::vector<std::shared_ptr<wolf>> &poblacion,
                  std::shared_ptr<wolf> alpha, std::shared_ptr<wolf> beta,
                  std::shared_ptr<wolf> delta, double min, double max) {
    // Aplico un cruce de tipo BLX (indicado en el guión de la P2 del problema
    // APC) Sobre el lobo alpha con el beta y sobre el alpha con el delta Se
    // reemplazan las 4 peores soluciones
    std::shared_ptr<wolf> l1, l2, l3, l4;

    std::tie(l1, l2) = BLX(alpha, beta, min, max);
    std::tie(l3, l4) = BLX(alpha, delta, min, max);
    l1->fitness = fitnes(&l1->sol[0]);
    l2->fitness = fitnes(&l2->sol[0]);
    l3->fitness = fitnes(&l3->sol[0]);
    l4->fitness = fitnes(&l4->sol[0]);

    auto tam = poblacion.size();
    poblacion[tam - 1].swap(l1);
    poblacion[tam - 2].swap(l2);
    poblacion[tam - 3].swap(l3);
    poblacion[tam - 4].swap(l4);
    std::sort(poblacion.begin(), poblacion.end());
}

std::vector<double> gwo_mejorado(const std::function<double(double *)> &fitnes,
                                 double min, double max, int dim,
                                 int MAX_EVALS) {
    // inicializar  el problema (Tamaño de población y número de iteraciones
    // generar lambdas para las funciones)

    std::vector<std::shared_ptr<wolf>> agentes;
    agentes = genera_soluciones_aleatorias(POP_SIZE, dim, min, max);

    const int MAX_ITERATIONS = MAX_EVALS / POP_SIZE;

    auto update_fitnes = [&fitnes](std::shared_ptr<wolf> &agente) -> void {
        agente->fitness = fitnes(&agente->sol[0]);
    };

    std::for_each(agentes.begin(), agentes.end(), update_fitnes);

    std::sort(agentes.begin(), agentes.end());

    std::shared_ptr<wolf> alpha, beta, delta;

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
        alpha = agentes[0];
        beta = agentes[1];
        delta = agentes[2];

        actualiza_lobos(dim, agentes, alpha, beta, delta, a, AUpdate);

        // Actualizamos fitnes de la nueva posición de los lobos
        std::for_each(agentes.begin(), agentes.end(), update_fitnes);

        // Los ordenamos en función de su fitnes para obtener la cuspide de la
        // gerarquia
        std::sort(agentes.begin(), agentes.end());

        alpha = agentes[0];
        beta = agentes[1];
        delta = agentes[2];

        //Operador que imita la reproducción de las manadas de lobos, los omegas nunca se reproducirán (pocas posibilidades de hacer en la vida real)
        reproduccion(fitnes, agentes, alpha, beta, delta, min, max);
    }

    return agentes[0]->sol;
}