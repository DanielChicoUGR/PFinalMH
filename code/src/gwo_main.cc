//
// Created by daniel on 15/06/23.
//
extern "C" {
#include "cec17.h"
}

#include <iostream>
#include <vector>
#include "gwo_lib.h"
#include "program_parser.h"
#include <filesystem>

namespace fs= std::filesystem;

void progressBar(int progress, int total)
{
    const int barWidth = 70;

    // Calcular la proporción de progreso
    float ratio = static_cast<float>(progress) / total;
    int barLength = static_cast<int>(ratio * barWidth);

    // Imprimir la barra de carga
    std::cout<< "[";
    for (int i = 0; i < barWidth; ++i)
    {
        if (i < barLength)
        {
            std::cout<< "=";
        }
        else
        {
            std::cout<< " ";
        }
    }
    std::cout<< "]   "<<static_cast<int>(ratio * 100.0)<<"  %\r" ;  // \r devuelve el cursor al inicio de la linea en la que se esta imprimiendo, pudiendo sobreescribir caracteres.
}


int main(int argc, char **argv) {
    std::vector<double> sol;
    int dim, veces;
    dim=10;
    veces=10;

//   int seed=0;
    int seed=time(nullptr);

    bool concurrent=false;
    bool help=false;

    CommandLine args("Optimizador GWO, Ningun Flag es obligatorio");
    args.addArgument({"-d", "--dim"}, &dim, "Dimensión del problema (por defecto se ejecuta la aircuryersión mas simple (10) y los valores validos son {10,30,50}");
    args.addArgument({"-c", "--concurrent", "Concurrent"}, &concurrent, "Concurrent");
    args.addArgument({"-s", "--seed", "Semilla aleatoria"},&seed, "Semilla generadora de N_aleatorios");
    args.addArgument({"-v", "--veces", "Repeticiones del test"},& veces, "Cuantas veces se va a ejecutar cada función");

    args.addArgument({"-h", "--help", "Ayuda"}, &help, "Ayuda");


    try {
        args.parse(argc, argv);
    } catch (std::runtime_error const& e) {
        std::cout << e.what() << std::endl;
        return -1;
    }

    if(help){
        args.printHelp();
        return 1;
    }

    if(!(dim == 10 or dim == 30 or dim == 50)){
        std::cerr<<"Error: Test functions are only defined for D=10,30,50."<<std::endl;
        return -1;
    }

//    auto total=veces*dim;

    for(int i=0;i<veces;i++) {
        fs::create_directories("results_gwo/"+std::to_string(dim)+"/r_" + std::to_string(i));
        std::string carpeta="gwo/"+std::to_string(dim)+"/r_" + std::to_string(i);
//        progressBar(i*30,total);

        for (int funcid = 1; funcid <= 30; funcid++) {
            sol.resize(dim);
            double fitness;

            cec17_init(carpeta.c_str(), funcid, dim);

            sol = gwo(cec17_fitness, -100.0, 100.0, dim, 10000 * dim, concurrent);


            fitness = cec17_fitness(&sol[0]);
//            progressBar(i*30+funcid,total);


            std::cout << "Fitness[F" << funcid << "]: " << std::scientific << cec17_error(fitness) << std::endl;
        }
    }

    return 0;


}



