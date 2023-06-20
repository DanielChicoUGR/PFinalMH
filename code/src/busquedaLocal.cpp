#include <vector>
#include <cmath>
#include <limits>
#include "random.hpp"
#include <chrono>


using namespace std;
using Random = effolkronium::random_static;
using namespace std::chrono;

std::normal_distribution<double> distribution2(0.0, sqrt(0.3));

void mutacion(vector<double> &w, int indice, double media, double desv_tipica)
{
    //1. Suma o resta un valor aleatorio a un peso de indice "indice" siguiendo una distribución normal
    w[indice] += Random::get(distribution2);
    if (w[indice] < 0)
        w[indice] = 0;
    if (w[indice] > 1)
        w[indice] = 1;
}


vector<double> busquedaLocal(vector<vector<double>> &entrenamiento)
{
    //Empezamos a medir el tiempo
    auto momentoInicio = high_resolution_clock::now(); 

    double media = 0.0;
    int tam = entrenamiento[0].size() - 1;
    double desv_tipica = sqrt(0.3);
    int iteraciones_max = 20 * tam;
    int max_evaluaciones = 15000;
    std::uniform_real_distribution<double> distribution(0.0, 1.0);
    vector<int> indices = vector<int>();

    // 1. la solucion inicial se  genera de forma aleatoria con la semilla pasada por parametro,
    // además incializamos el vector de indices.
    vector<double> pesos = vector<double>();
   

    for (int i = 0; i < tam; ++i)
    {
        pesos.push_back(Random::get(distribution));
        indices.push_back(i);
    }
    

    // 2. Se calcula el fitness de la solucion inicial
    int num_vecinos = 0;
    int num_evaluaciones = 0;
    double tasa_red = TasaRed(pesos);
    double tasa_clas = TasaClas(entrenamiento, entrenamiento, pesos, true);
    double fitness = funcion_objetivo(tasa_red, tasa_clas);
    double fitness_nuevo;

    int indice = 0;
    vector<double> pesos_nuevo;

    // 3. Se genera una permutacion de los indices de los pesos
    Random::shuffle(indices); 
    
    // 4. Comenzamos el bucle prinipal del que salimos cuando se cumpla alguna de las dos condiciones del while
    while (num_vecinos <= iteraciones_max and num_evaluaciones <= max_evaluaciones)
    {
        //4.1 Genera un nuevo vecino 
        num_vecinos++;
        pesos_nuevo = pesos;
        mutacion(pesos_nuevo, indices[indice], media, desv_tipica); // creo
        fitness_nuevo = funcion_objetivo(TasaRed(pesos_nuevo), TasaClas(entrenamiento, entrenamiento, pesos_nuevo, true));
        num_evaluaciones++;
        indice++;

        //4.2 Si el nuevo vecino es mejor que la solucion actual, se realiza el cambio, si se han mutado
        // todos los pesos, se vuelve a generar la permutacion de los indices
        if (fitness_nuevo > fitness)
        {
            pesos = pesos_nuevo;
            fitness = fitness_nuevo;
            num_vecinos = 0;
            indice = indices.size();
        }

        if (indice == indices.size())
        {
            indice = 0;
            Random::shuffle(indices);
        }
    }

    //Terminamos de medir el tiempo
    return pesos;
}