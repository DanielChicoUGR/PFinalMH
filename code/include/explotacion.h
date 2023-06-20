//
// Created by daniel on 20/06/23.
//

#ifndef CEC17_DANIEL_CHICO_VALDERAS_EXPLOTACION_H
#define CEC17_DANIEL_CHICO_VALDERAS_EXPLOTACION_H

#include <vector>
#include <functional>

std::vector<double> busqueda_local(const std::function<double(double*)> &fitness_func,const double &min, const double &max, const std::vector<double>&sol_ini,int MAX_ITERS=200,double mean=0.0,double std= std::sqrt(0.3));

std::vector<double> Enfriamiento_simulado(const std::function<double(double*)> &fitness_func,const double&min, const double&max,const std::vector<double>&sol_ini,int MAX_ITERS=200,double mean=0.0,double std=1);


#endif //CEC17_DANIEL_CHICO_VALDERAS_EXPLOTACION_H
