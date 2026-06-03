#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//Definição dos valores
#define TAMANHO_MAX 100000
#define INICIO_REF 30000
#define TAMANHO_REF 25000

float tempo[TAMANHO_MAX];
float ch1_peak[TAMANHO_MAX];
float ch2_peak[TAMANHO_MAX];
float ch3_peak[TAMANHO_MAX];
float ch4_peak[TAMANHO_MAX];
float ch1_filtrado[TAMANHO_MAX] = {0};
float ch2_filtrado[TAMANHO_MAX] = {0};
float ch3_filtrado[TAMANHO_MAX] = {0};
float ch4_filtrado[TAMANHO_MAX] = {0};
float s_ref[TAMANHO_REF];

void pular_linhas(FILE *file) {
    int linha = 0;
    int carac;
    while (linha < 16) {
        carac = fgetc(file);
        if (carac == '\n') {
            linha++;
        }
    }
}

int main() {

    FILE *x1;
    x1 = fopen("C:\\Users\\taish\\Downloads\\T0004ALL.csv", "r");
    if (x1 == NULL) {
        printf("Erro ao abrir arquivo\n");
        return 1;
    }
    pular_linhas(x1);
    char linha[256];
    int i = 0;
    float media1 = 0;
    float media2 = 0;
    float media3 = 0;
    float media4 = 0;

    while (fgets(linha, sizeof(linha), x1) != NULL && i < TAMANHO_MAX) {
        char *parte = strtok(linha, ",");
        int coluna = 0;
        //Recebendo os valores do arquivo
        while (parte != NULL) {
            switch (coluna) {
                case 0:
                    tempo[i] = atof(parte);
                    break;

                case 1:
                    ch1_peak[i] = atof(parte);
                    media1 += ch1_peak[i];
                    break;

                case 3:
                    ch2_peak[i] = atof(parte);
                    media2 += ch2_peak[i];
                    break;

                case 5:
                    ch3_peak[i] = atof(parte);
                    media3 += ch3_peak[i];
                    break;

                case 7:
                    ch4_peak[i] = atof(parte);
                    media4 += ch4_peak[i];
                    break;
            }

            parte = strtok(NULL, ",");
            coluna++;
        }
        i++;
    }
    int total_amostras = i;
    fclose(x1);
    // Realiza a média (soma de cada coluna/total de amostras)
    media1 = media1 / total_amostras;
    media2 = media2 / total_amostras;
    media3 = media3 / total_amostras;
    media4 = media4 / total_amostras;
    for (i = 0; i < TAMANHO_REF; i++) {
        // Faz a média do sinal de referência
        s_ref[i] = ch2_peak[INICIO_REF + i] - media2;
    }
    //Filtro casado dicsreto
    for (i = 0; i < total_amostras - TAMANHO_REF; i++) {
        float soma1 = 0;
        float soma2 = 0;
        float soma3 = 0;
        float soma4 = 0;
        int j;
        for (j = 0; j < TAMANHO_REF; j++) {
        //[i+j] testando todos os possíveis lugares de combinação no sinal e fazendo correlação através da multiplicação
        soma1+=(ch1_peak[i+j] - media1) * s_ref[TAMANHO_REF-1-j];
        soma2+=(ch2_peak[i + j] - media2) * s_ref[TAMANHO_REF-1-j];
        soma3+=(ch3_peak[i + j] - media3) * s_ref[TAMANHO_REF-1-j];
        soma4+=(ch4_peak[i + j] - media4) * s_ref[TAMANHO_REF-1-j];
        }

        //Dividindo os sinais filtrados pelo tamanho de referencia
        ch1_filtrado[i] = soma1 / TAMANHO_REF;
        ch2_filtrado[i] = soma2 / TAMANHO_REF;
        ch3_filtrado[i] = soma3 / TAMANHO_REF;
        ch4_filtrado[i] = soma4 / TAMANHO_REF;

}
    int max_ch1 = 0;
    int max_ch2 = 0;
    int max_ch3 = 0;
    int max_ch4 = 0;
    float maior1 = -1000;
    float maior2 = -1000;
    float maior3 = -1000;
    float maior4 = -1000;

    for (i = 1; i < total_amostras - TAMANHO_REF; i++) {

        if (ch1_filtrado[i] > maior1){
            max_ch1 = i;
            maior1 = ch1_filtrado[i];
            }

        if (ch2_filtrado[i] > maior2){
            max_ch2 = i;
            maior2 = ch2_filtrado[i];
            }

        if (ch3_filtrado[i] > maior3){
            max_ch3 = i;
            maior3 = ch3_filtrado[i];
            }

        if (ch4_filtrado[i] > maior4){
            max_ch4 = i;
            maior4 = ch4_filtrado[i];
            }
    }

    printf("O indice do maior numero na coluna 1 e: %d\n", max_ch1);
    printf("O tempo (milisegundos) desse indice na coluna 1 e: %f\n\n",tempo[max_ch1] * 1000);

    printf("O indice do maior numero na coluna 2 e: %d\n", max_ch2);
    printf("O tempo (milisegundos) desse indice na coluna 2 e: %f\n\n",tempo[max_ch2] * 1000);

    printf("O indice do maior numero na coluna 3 e: %d\n", max_ch3);
    printf("O tempo (milisegundos) desse indice na coluna 3 e: %f\n\n",tempo[max_ch3] * 1000);

    printf("O indice do maior numero na coluna 4 e: %d\n", max_ch4);
    printf("O tempo (milisegundos) desse indice na coluna 4 e: %f\n\n",tempo[max_ch4] * 1000);
}
