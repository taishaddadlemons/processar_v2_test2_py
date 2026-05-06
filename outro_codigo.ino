// ============================================================
// Leitura de microfone INMP441 com Arduino Nano via I2S Software
// O Arduino Nano NÃO tem I2S por hardware, então usamos a
// biblioteca "I2S" por software via pinos digitais manuais.
//
// Biblioteca necessária: "I2S" by Axel Richter (Arduino Library Manager)
// ou use bit-banging manual (implementado abaixo sem biblioteca externa)
// ============================================================

// --- Definição dos pinos ---
// O INMP441 usa protocolo I2S. Conecte assim:
// INMP441   ->  Arduino Nano
//   VDD     ->  3.3V
//   GND     ->  GND
//   WS      ->  Pino 9  (Word Select / LRCLK)
//   SCK     ->  Pino 8  (Clock / BCLK)
//   SD      ->  Pino 2  (Dados de saída do mic)
//   L/R     ->  GND     (seleciona canal esquerdo)
#include<arduino.h>
#define I2S_WS  9   // Word Select (LRCLK) - define canal L ou R
#define I2S_SCK 8   // Clock do barramento I2S
#define I2S_SD  2   // Dados de áudio saindo do microfone

// --- Configurações do protocolo ---
#define BITS_PER_SAMPLE 24   // O INMP441 envia 24 bits úteis em 32 bits
#define SAMPLE_RATE     16000 // Taxa de amostragem (16kHz)

// Calcula o semiperíodo do clock em microsegundos
// Ex: 16000 Hz * 32 bits * 2 canais = 1.024.000 pulsos/s
// Mas como fazemos bit-bang, aproximamos com um delay mínimo
#define HALF_PERIOD_US 3  // ~1us por meio ciclo (aprox. 500kHz de clock)



void setup() {
  Serial.begin(2000000);
  delay(1000);
  Serial.println("=== TESTE I2S SOFTWARE - Arduino Nano + INMP441 ===");

  // Configura os pinos de clock e WS como saída (gerados pelo Arduino)
  pinMode(I2S_SCK, OUTPUT);
  pinMode(I2S_WS, OUTPUT);

  // Configura o pino de dados como entrada (recebe os bits do microfone)
  pinMode(I2S_SD, INPUT);

  // Estado inicial: clock baixo, WS baixo (canal esquerdo)
  digitalWrite(I2S_WS, HIGH);
  digitalWrite(I2S_SCK, LOW);
  

  Serial.println("Pinos configurados. Iniciando leitura...");
}

// --- Função que lê um sample de 32 bits via I2S bit-banging ---
// O protocolo I2S funciona assim:
//   - WS = LOW  -> canal esquerdo
//   - WS = HIGH -> canal direito
//   - Os dados são enviados pelo mic a cada borda de subida do clock
//   - O bit mais significativo (MSB) vem primeiro
  int32_t readI2SSample(char canal) {
  int32_t sample = 0;

  // Seleciona canal esquerdo (L/R do INMP441 está em GND = esquerdo)
  digitalWrite(I2S_WS, canal);

  digitalWrite(I2S_SCK, LOW);
  // período perdido
  atraso(HALF_PERIOD_US);
  digitalWrite(I2S_SCK, HIGH);
  atraso(HALF_PERIOD_US);
  digitalWrite(I2S_SCK, LOW);
  atraso(HALF_PERIOD_US);
  int in=23;
  // Lê 24 bits, um por vez, pulsando o clock manualmente
  for (int i = 30; i >= 0; i--) {
    // Sobe o clock - o mic coloca o próximo bit na linha SD
    digitalWrite(I2S_SCK, HIGH);
    // Lê o bit atual no pino SD e coloca na posição correta
    atraso(HALF_PERIOD_US);
    if(in>=0){
      if (digitalRead(I2S_SD)) {
        sample |= (1UL << in);  // define o bit i como 1
      }
      in--;
    }
    digitalWrite(I2S_SCK, LOW);
    atraso(HALF_PERIOD_US);
  }
  if(sample&(1<<23))sample|=(0b11111111000000000000000000000000);
  /*
  tenho que adicionar
  int24=a
  int32=b
  for(int i=0;i<24;i++){
    if(a&(1<<i))b+=(1<<i);
    if(a&(1<<i) && i==23) b+=((1<<9-1)<<24);
  }
  */

  // O INMP441 envia 24 bits úteis, os 8 menos significativos são lixo
  // Faz o shift para remover os bits inúteis e manter o sinal correto
  //sample = sample >> 8;  // descarta os 8 bits menos significativos

  return sample;
}
void atraso(int x){
 for(int a=0; a<x ;a++){
 }
}
void loop() {
  // Lê um sample de 24 bits (em formato 32 bits) do microfone
  signed int sample = readI2SSample(0);
  char a[40];
  // Reduz para 16 bits para facilitar a leitura no Serial Monitor/Plotter
  // (o valor bruto de 24 bits é grande demais para visualizar direto)
  //int16_t s16 = (int16_t)(sample >> 8);
  sprintf(a, "%32b", sample);
  // Imprime o valor - use o Serial Plotter para ver a forma de onda!
  Serial.print(sample);
  Serial.print(",");


  sample = readI2SSample(1);
  sprintf(a, "%32b", sample);
  Serial.println(a);
  // Delay para não inundar a serial (remova para máxima taxa de amostragem)
  
}
