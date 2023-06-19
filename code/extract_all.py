#!/bin/python3
import os
import sys

# Obtener el nombre de la carpeta como parámetro
carpeta = sys.argv[1]  # El primer argumento después del nombre del script

print(f'{carpeta}')
# Comprobar si se proporcionó un argumento
if not carpeta:
    print("Debe proporcionar el nombre de la carpeta como argumento.")
    sys.exit(1)

# Comprobar si la carpeta existe
if not os.path.isdir(carpeta):
    print(f"La carpeta '{carpeta}' no existe.")
    sys.exit(1)

# Obtener la lista de archivos en la carpeta
archivos = os.listdir(carpeta)


# Recorrer cada archivo en la carpeta
for archivo in archivos:
    # Comprobar si el archivo es un archivo de Python
    comando = f"python3 extract.py {carpeta}/{archivo}"
    os.system(comando)
