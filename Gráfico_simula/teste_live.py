import threading
import queue
import time
import random

from mapa_de_calor_Real import (
    VisualizadorMapaDeCalor,
    X_MIN, X_MAX, Y_MIN, Y_MAX,
    INTERVALO_SINAL,
)


def gerar_dados_teste(n_pontos=150):
    random.seed(42)
    dados = []
    x = (X_MIN + X_MAX) / 2
    y = (Y_MIN + Y_MAX) / 2
    for _ in range(n_pontos):
        passo_x = random.uniform(-1.5, 1.5)
        passo_y = random.uniform(-1.5, 1.5)
        x = min(max(x + passo_x, X_MIN), X_MAX)
        y = min(max(y + passo_y, Y_MIN), Y_MAX)
        dados.append((x, y))
    return dados


class SimuladorDeReproducao(threading.Thread):
    def __init__(self, fila_dados: queue.Queue, dados):
        super().__init__(daemon=True)
        self.fila_dados = fila_dados
        self.dados = dados
        self.ativo = True

    def run(self):
        for ponto in self.dados:
            if not self.ativo:
                break
            self.fila_dados.put(ponto)
            time.sleep(INTERVALO_SINAL)

    def parar(self):
        self.ativo = False


def main():
    dados_teste = gerar_dados_teste(150)

    fila_dados = queue.Queue()
    reprodutor = SimuladorDeReproducao(fila_dados, dados_teste)
    reprodutor.start()

    visualizador = VisualizadorMapaDeCalor(fila_dados)
    try:
        visualizador.executar()
    finally:
        reprodutor.parar()


if __name__ == "__main__":
    main()