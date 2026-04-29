#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<float.h>
#include<string.h>
#define VELOCIDADE_SOM 330.0

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

    x1 = fopen("C:\\Users\\taish\\Downloads\\T0000ALL.CSV", "r");

    if(x1 == NULL) {
        printf("Erro ao abrir os arquivos\n");
    }
    pular_linhas(x1);
    printf("16 linhas puladas\n\n");
    printf("RESULTADO APROXIMADO:\n");

    float tempo, ch1_peak, ch2_peak, ch3_peak, ch4_peak;
    float max_ch1=0, max_ch2=0, max_ch3=0, max_ch4=0;
    float t_receptor1, t_receptor2, t_receptor3, t_receptor4;

    //char linha[256];
    //int i=0;
    /*while (i<1000) {
        fgets(linha,256,x1);
        char *parte = strtok(linha, ",");
        int coluna = 0;
        while (parte != NULL) {
            switch (coluna) {
                case 0:
                    tempo = strtof(parte, NULL);
                    break;
                case 2:
                    ch1_peak = strtof(parte, NULL);
                    break;
                case 4:
                    ch2_peak = strtof(parte, NULL);
                    break;
                case 6:
                    ch3_peak = strtof(parte, NULL);
                    break;
                case 8:
                    ch4_peak = strtof(parte, NULL);
                    break;
            }
            parte = strtok(NULL, ",");
            coluna++;
        }

        if (ch1_peak > max_ch1) {
            max_ch1 = ch1_peak;
            t_receptor1 = tempo;
        }
        if (ch2_peak > max_ch2) {
            max_ch2 = ch2_peak;
            t_receptor2 = tempo;
        }
        if (ch3_peak > max_ch3) {
            max_ch3 = ch3_peak;
            t_receptor3= tempo;
        }
        if (ch4_peak > max_ch4) {
            max_ch4 = ch4_peak;
            t_receptor4 = tempo;
        }

        i++;
    }
    */
    t_receptor1= 0.00085144;
    t_receptor2= 0.00107976;
    t_receptor3= 0.00127384;
    t_receptor4= 0.00156336;


    printf("\nT1:%e\n", t_receptor1);
    printf("T2:%e\n", t_receptor2);
    printf("T3:%e\n", t_receptor3);
    printf("T4:%e\n", t_receptor4);

    float ds1 = (VELOCIDADE_SOM * t_receptor1);
    printf("DS1:%g",ds1);
    float ds2 = (VELOCIDADE_SOM * t_receptor2);
    printf("\nDS2:%g",ds2);
    float ds3 = (VELOCIDADE_SOM * t_receptor3);
    printf("\nDS3:%g",ds3);
    float ds4 = (VELOCIDADE_SOM * t_receptor4);
    printf("\nDS4:%g",ds4);

    float ys1 = 0;
    float ys2 = 0.3;
    float ys3 = .6;
    float ys4 = .9;
    float A,B,C,yA,xA;
    A=ds1-ds2;
    printf("\nA:%g",A);
    C=ds1-ds4;
    printf("\nC:%g",C);
    yA=(pow(ys1,2) - pow(ys4,2) - (pow(ys1,2)* C/A) + (pow(ys2,2) *C / A) - A * C +pow(C,2))/(2*ys1-2*ys4-2*ys1*C/A+2*ys2*C/A);
    printf("\n%.1f",yA);
    xA=sqrt(pow(ds1,2)-(pow(ys1-yA,2)));
    printf("\n%.2f",xA);

    fclose(x1);
    return 0;
}
