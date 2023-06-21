//
// Created by daniel on 20/06/23.
//
#include <random>
#include "explotacion.h"
#include "random.hpp"

using RANDOM = effolkronium::random_static;

void truncar(double &d, double min, double max)
{
    if (d < min)
    {
        d = min;
    }
    else if (d > max)
    {
        d = max;
    }
}

std::vector<double> mutar(std::vector<double> &sol, int indice, std::normal_distribution<double> &distribucion, double min, double max)
{
    auto ret = sol;
    ret[indice] += RANDOM::get(distribucion);
    truncar(ret[indice], min, max);
    return std::move(ret);
}

std::vector<double> busqueda_local(const std::function<double(double *)> &fitness_func, const double &min, const double &max, const std::vector<double> &sol_ini, int MAX_ITERS, double mean, double std)
{
    std::normal_distribution<double> distribution(mean, std);
    std::vector<double> sol = sol_ini;
    double mejor_fitness = fitness_func(&sol[0]);

    std::vector<int> indices(sol.size());
    std::iota(indices.begin(), indices.end(), 0);

    int iter = 0;
    while (iter < MAX_ITERS)
    {
        RANDOM::shuffle(indices.begin(), indices.end());

        for (size_t j = 0; j < indices.size() and iter < MAX_ITERS; j++)
        {
            std::vector<double> nueva_sol = mutar(sol, indices[j], distribution, min, max);
            double fitness = fitness_func(&nueva_sol[0]);

            if (fitness < mejor_fitness)
            {
                mejor_fitness = fitness;
                sol = nueva_sol;
                j = indices.size();
            }
            iter += 1;
        }
    }

    return std::move(sol);
}

std::vector<double> Enfriamiento_simulado(const std::function<double(double *)> &fitness_func, const double &min, const double &max, const std::vector<double> &sol_ini, int MAX_ITERS, double mean, double std)
{

    std::normal_distribution<double> distribution(mean, std);

    // Generamos constantes y otros valores que usa el algoritmo
    double mu = 0.3;
    double phi = 0.2;
    int max_vecinos = 5 * sol_ini.size();
    int max_exitos = static_cast<int>(0.1 * static_cast<double>(max_vecinos));
    int n_enfriamientos = MAX_ITERS / max_vecinos;

    std::vector<int> indices(sol_ini.size());
    std::iota(indices.begin(), indices.end(), 0);

    // temperatura final

    const double T_FINAL = 0.001;

    auto mejor_sol = sol_ini;
    auto sol_actual = sol_ini;

    double f_mejor = fitness_func(&mejor_sol[0]);

    double f_actual = f_mejor;
    double temp_k = (mu * f_mejor) / -std::log(phi);

    // calculo el valor de beta
    double beta = (temp_k - T_FINAL) / (n_enfriamientos * temp_k * T_FINAL);

    // declaro temp_k para la temperatura actual en cada iteracion k

    // valor para entrar el bucle while
    int n_exitos = -1;
    int ev = 0;

    int pos ;

    while (n_exitos != 0 and ev < MAX_ITERS)
    {
        RANDOM::shuffle(indices);
        n_exitos = 0;
        int i = 0;
        for (int j = 0; j < max_vecinos and n_exitos < max_exitos and ev < MAX_ITERS; j++)
        {
            pos = indices[(i++) % indices.size()];
            std::vector<double> sol_nueva = mutar(sol_actual, pos, distribution, min, max);
            double f_nueva = fitness_func(&sol_nueva[0]);
            ev++;
            auto delta_f = f_nueva - f_actual;

            if (delta_f < 0 or RANDOM::get(0.0, 1.0) <= exp(-delta_f / temp_k))
            {
                f_actual = f_nueva;
                sol_actual = sol_nueva;
                n_exitos++;
                if (f_actual < f_mejor)
                {
                    f_mejor = f_actual;
                    mejor_sol = sol_actual;
                }
            }
        }

        temp_k = temp_k / (1 + temp_k * beta);
    }

    return std::move(mejor_sol);
}