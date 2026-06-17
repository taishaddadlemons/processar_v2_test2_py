import pandas as pd
import matplotlib.pyplot as plt
import numpy as np
# Caminho do arquivo CSV
arquivo = r"c:\Users\taish\Downloads\T0004ALL.csv"
df = pd.read_csv(arquivo, header=16, usecols=[0,1,3,5,7])
cores = ["blue", "red", "pink", "green"]
max_indice = 100000
pula=1
tempo = df.iloc[:max_indice, 0].to_numpy()
var = np.empty((len(tempo), 4))
for b in range(4):
    var[:, b] = df.iloc[:max_indice, b+1].to_numpy()
inicio = 30000
tamanho = 250000
s_ref = var[inicio:inicio+tamanho, 1]
s_ref = s_ref - np.mean(s_ref)
var_filtrado = np.empty_like(var)
for b in range(4):
    sinal = var[:, b] - np.mean(var[:, b])
    var_filtrado[:, b] = np.correlate(sinal, s_ref[::-1], mode='same')
    #np.correlate analisa quando os sinais se encaixam, nesse caso o sinal e o sinal de referência invertido
    #nesse comando np.correlate existe os modos full (correlaciona em todas posições possíveis-N+M-1)
    #valid (só onde se sobrepõe-N-M+1) e same(O mesmo valor do original-N)
    max1=np.argmax(var_filtrado[:,b])#identificar o índice do maior valor de cada coluna
    print("O indice do maior numero na coluna:",b+1,"é:",max1)
    print("O tempo (milisegundos) desse indice na coluna",b+1,"e:",tempo[max1])
# Substitui o sinal original pelo filtrado
var = var_filtrado/tamanho
plt.figure(figsize=(15,5))
plt.imshow(
    var.T,
    aspect='auto',
    origin='lower',
    cmap='jet',
    extent=[tempo[0]*1000, tempo[-1]*1000, 1, 4],
    interpolation='bilinear'
)
plt.colorbar(label='Amplitude da Correlação')
plt.yticks(
    [1,2,3,4],
    ['CH1','CH2','CH3','CH4']
)
plt.xlabel("Tempo (ms)")
plt.ylabel("Canal")
plt.title("Mapa de Calor do Filtro Casado")
for b in range(4):
    indice_max = np.argmax(var[:, b])
    plt.scatter(
        tempo[indice_max]*1000,
        b+1,
        marker='x',
        s=100
    )
plt.tight_layout()
plt.show()