#!/bin/python3
import os
import pandas as pd
import sys


def leer_archivos_xlsx(carpeta_orig):
    archivos_xlsx = []
    dataframes = []

    # Obtener la lista de archivos en la carpeta
    for carpeta in os.listdir(carpeta_orig):
        for file in os.listdir(os.path.join(carpeta_orig, carpeta)):
            if file.endswith(".xlsx"):
                archivos_xlsx.append(os.path.join(carpeta_orig, carpeta, file))

    # Leer cada archivo .xlsx y almacenar su contenido en dataframes
    for archivo in archivos_xlsx:
        dataframe = pd.read_excel(archivo)
        dataframes.append(dataframe)


    return dataframes
def calcular_media_dataframes(dataframes):
    # Combinar los dataframes utilizando concat
    combinado = pd.concat(dataframes)

    # Calcular la media de cada celda
    media = combinado.groupby(combinado.index).mean()

    return media


# Carpeta donde se encuentran los archivos .xlsx
carpeta_archivos = sys.argv[1]

resultado = leer_archivos_xlsx(carpeta_archivos)

media = calcular_media_dataframes(resultado)

media.to_excel(f'{carpeta_archivos}/media.xlsx')


print(media)
# print(resultado)
