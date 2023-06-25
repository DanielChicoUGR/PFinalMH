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

# import pandas as pd
# import sys

# # leer la tabla
# df = pd.read_table(sys.argv[1], index_col=0, skiprows=1)

# # separar la tabla en dos
# df1 = df.iloc[:, :7]
# df2 = df.iloc[:, 7:]

# # cambiar los nombres de las columnas de la primera tabla
# df1.columns = ['1', '2', '3', '5', '10', '20', '30']

# # cambiar los nombres de las columnas de la segunda tabla
# df2.columns = ['40', '50', '60', '70', '80', '90', '100']

# # escribir las tablas en formato LaTeX
# with open(sys.argv[1], 'w') as f:
#     f.write('\\begin{table}[H]\n')
#     f.write('    \\caption{Media de ranking 10 Dimensiones}\n')
#     f.write('    \\label{table:pos_med_10}\n')
#     f.write('    \\begin{tabular}{lrrrrrrr}\n')
#     f.write('        \\toprule\n')
#     f.write('        {D10}  & 1        & 2        & 3        & 5        & 10       & 20       & 30       \\\\\\midrule\n')
#     f.write(df1.to_latex(escape=False))
#     f.write('        \\bottomrule\n')
#     f.write('    \\end{tabular}\n')
#     f.write('\n')
#     f.write('    \\begin{tabular}{lrrrrrrr}\n')
#     f.write('        \\toprule\n')
#     f.write('        {D10}  & 40       & 50       & 60       & 70       & 80       & 90       & 100      \\\\\\midrule\n')
#     f.write(df2.to_latex(escape=False))
#     f.write('        \\bottomrule\n')
#     f.write('    \\end{tabular}\n')
#     f.write('\\end{table}\n')
