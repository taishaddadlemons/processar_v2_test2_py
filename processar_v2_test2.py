import pandas as pd
import matplotlib.pyplot as plt
import numpy as np

# Caminho do arquivo CSV
arquivo = r"c:\Users\taish\Downloads\T0004ALL.csv"
df = pd.read_csv(arquivo, header=16, usecols=[0,1,3,5,7])
cores = ["blue", "red", "pink", "green"]
max_indice = 10000
pula=1
inicio = 50000
tempo = df.iloc[:max_indice, 0].to_numpy()
var = np.empty((max_indice, 4))
var2 = np.empty((max_indice, 4))
y = np.empty((len(tempo), 4))
for b in range(4):
    var[:max_indice,b] = df.iloc[inicio:max_indice+inicio, b+1].to_numpy()
    #Na sequência cálculo da filtragem de evento
    rapidez = 0.001
    evento = 2.45
    y[1]=0
    for c in range(2,max_indice):
        y[c,b] = y[c-1,b]*(1-rapidez) + (abs(var[c,b])>evento)*rapidez
    for c in range(2,max_indice):
        if np.any(y[c,b] > 0.5):
            break
    var2 = var[c,:max_indice]
    print("O valor de y na coluna:",b+1,"é:",y)
plt.figure(figsize=(10,6))
for b in range(4):
    plt.plot(tempo*1000, y[:, b], label=f"Sinal filtrado CH{b+1}", color=cores[b])
plt.xlabel("Tempo (milisegundos)")
plt.figure(figsize=(10,6))
for b in range(4):
    plt.plot(tempo*1000, var[:, b], label=f"Sinal filtrado CH{b+1}", color=cores[b])
plt.ylabel("Amplitude")
plt.grid()
plt.show()