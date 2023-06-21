#!/bin/python3

import sys

def divide_tabla_latex(latex_input):
    # Leer el contenido del archivo LaTeX
    with open(latex_input, 'r') as file:
        latex_content = file.read()

    # Buscar el inicio y fin de la tabla
    table_start = latex_content.find(r'\begin{tabular}')
    table_end = latex_content.find(r'\end{tabular}')

    # Extraer el contenido de la tabla
    table_content = latex_content[table_start:table_end]

    # Dividir el contenido de la tabla en filas
    rows = table_content.split(r'\\')

    # Calcular el número de columnas original
    num_columns = rows[0].count('&') + 1

    # Calcular el número de columnas para cada tira
    num_columns_tira = num_columns // 2

    # Calcular el índice de división de las columnas
    split_index = num_columns_tira

    # Construir la primera tira de la tabla
    table1 = r'\begin{tabular}' + '{' + 'l' + (num_columns_tira - 1) * 'r' + '}' + '\n'
    table1 += r'\toprule' + '\n'
    for row in rows:
        cells = row.split('&')
        table1 += cells[0] + '&'.join(cells[1:split_index]) + r'\\' + '\n'
    table1 += r'\bottomrule' + '\n'
    table1 += r'\end{tabular}' + '\n'

    # Construir la segunda tira de la tabla
    table2 = r'\begin{tabular}' + '{' + (num_columns - num_columns_tira) * 'r' + '}' + '\n'
    table2 += r'\toprule' + '\n'
    for row in rows:
        cells = row.split('&')
        table2 += '&'.join(cells[split_index:]) + r'\\' + '\n'
    table2 += r'\bottomrule' + '\n'
    table2 += r'\end{tabular}' + '\n'

    # Reemplazar la tabla original con las dos tiras de la tabla divididas
    divided_content = latex_content[:table_start] + table1 + '\n\n' + table2 + latex_content[table_end:]

    # Escribir el contenido dividido en el mismo archivo
    with open(latex_input, 'w') as file:
        file.write(divided_content)

    print("La tabla se ha dividido y actualizado en el archivo '{}'.".format(latex_input))




# Ejemplo de uso
latex_input = sys.argv[1]
divide_tabla_latex(latex_input)
