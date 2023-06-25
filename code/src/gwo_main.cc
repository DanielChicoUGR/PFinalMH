//
// Created by daniel on 15/06/23.
//
extern "C" {
#include "cec17.h"
}

#include "gwo_lib.h"
#include "program_parser.h"
#include <filesystem>
#include <iostream>
#include <vector>

namespace fs = std::filesystem;

int main(int argc, char **argv) {
  std::vector<double> sol;
  int dim, veces;
  dim = 10;
  veces = 10;
  int algoritmo = 0;
  bool bl = false;

  //   int seed=0;
  int seed = time(nullptr);

  bool concurrent = false;
  bool help = false;

  CommandLine args("Optimizador GWO, Ningun Flag es obligatorio");
  args.addArgument({"-d", "--dim"}, &dim,
                   "Dimensión del problema (por defecto se ejecuta la versión "
                   "mas simple (10) y los valores validos son {10,30,50}");

  args.addArgument(
      {"-s", "--seed"}, &seed,
      "Semilla generadora de N_aleatorios. por defecto seed=time(nullptr)");
  args.addArgument({"-v", "--veces"}, &veces,
                   "Cuantas veces se va a ejecutar cada función");
  args.addArgument({"-a", "--alg"}, &algoritmo,
                   "Algoritmo a ejecutar. 0 = básico, 1 = básico+híbrido, 2 = "
                   "mejoras Teóricas");
  args.addArgument(
      {"-bl"}, &bl,
      "Al ejecutar el modelo hibrido, ejecutar la busqueda local o el "
      "enfriamiento simulado, si no se indica se ejecuta con ES");

  // ./gwo -s 1 -t 10 -hibrido -es -d 10

  args.addArgument({"-h", "--help", "--ayuda"}, &help, "Ayuda");

  try {
    args.parse(argc, argv);
  } catch (std::runtime_error const &e) {
    std::cout << e.what() << std::endl;
    return -1;
  }

  if (help) {
    args.printHelp();
    return 1;
  }

  // Comrpobamos que la dimension sea correcta
  if (!(dim == 10 or dim == 30 or dim == 50)) {
    std::cerr << "Error: Test functions are only defined for D=10,30,50."
              << std::endl;
    return -1;
  }

  // Comprobamos que el algoritmo sea correcto
  if (algoritmo != 0 && algoritmo != 1 && algoritmo != 2) {
    std::cerr << "Error: Test functions are only defined for 0,1,2."
              << std::endl;
    return -1;
  }

  std::string ejecucion;
  if (algoritmo == 1)
    if (!bl)
      ejecucion = "gwo_hibrido_es/" + std::to_string(dim) + "/r_";
    else
      ejecucion = "gwo_hibrido_bl/" + std::to_string(dim) + "/r_";
  else
    ejecucion = "gwo/" + std::to_string(dim) + "/r_";

  for (int i = 0; i < veces; i++) {
    fs::create_directories("results_" + ejecucion + std::to_string(i));
    std::string carpeta = ejecucion + std::to_string(i);

    for (int funcid = 1; funcid <= 30; funcid++) {
      sol.resize(dim);
      double fitness;

      cec17_init(carpeta.c_str(), funcid, dim);
      switch (algoritmo) {
      case 1:
        sol = gwo_hibrido(cec17_fitness, -100.0, 100.0, dim, 10000 * dim, !bl);
        break;
      case 0:
        sol = gwo(cec17_fitness, -100.0, 100.0, dim, 10000 * dim);
        break;
      case 2:
        sol = gwo_mejorado(cec17_fitness, -100.0, 100.0, dim, 10000 * dim);
        break;
      default:

        throw std::runtime_error("Algoritmo no implementado");

        break;
      }
      fitness = cec17_fitness(&sol[0]);

      std::cout << "Fitness[F" << funcid << "]: " << std::scientific
                << cec17_error(fitness) << std::endl;
    }
  }

  return 0;
}
