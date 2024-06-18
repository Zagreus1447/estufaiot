#include <Adafruit_Sensor.h>
#include <DHT.h>
#include <DHT_U.h>
#include <Servo.h>
#include <string.h>

Servo Servo1;

const int servoPin = 3; //pino servo
int pos = 0;    // Posição atual do servo
int minPos = 0; // Posição mínima
int maxPos = 90; // Posição máxima


// Definindo os pinos e tipos de sensores
#define DHTPIN1 A0 // Pino digital conectado ao primeiro DHT11
#define DHTPIN2 A1 // Pino digital conectado ao segundo DHT11
#define DHTTYPE DHT11 // Definindo o tipo de sensor DHT

// Inicializando os sensores DHT
DHT dht1(DHTPIN1, DHTTYPE);
DHT dht2(DHTPIN2, DHTTYPE);

//vetor para armazenar as temperturas
float temperatura[2];

//variaveis de tempo para pegar guardar array de tempo
int tempAt = 0;
int tempPa = 0; 
int intervalotemp = 2000; 

//pino ventoinha
int vent = 4;

void fecharTeto(); 
void abrirTeto();
void pegarTemperatura(float *temperatura); //funcao para pegar temperaturas

void setup() {
  pinMode(vent, OUTPUT); //defini o pino da ventoinha com output
  Servo1.attach(servoPin); //Associa o pino com a instancia do pino
  Serial.begin(9600); // Inicializa a comunicação serial a 9600 bps
  dht1.begin(); // Inicializa o primeiro sensor DHT
  dht2.begin(); // Inicializa o segundo sensor DHT

}

void loop() {
  // ler a posicao inicial do servo
  pos = Servo1.read();
  // ligar inicialmente a ventoinha
  digitalWrite(vent, HIGH);

  unsigned long tempAt = millis();//contador para ler valores de 200
  if (Serial.available()) { // Lê a linha recebida
    String receivedData = Serial.readStringUntil('\n');
    
    Serial.println("Recebido do ESP32: " + receivedData); // Exibe os dados recebidos no monitor serial
   
    String prefix = receivedData.substring(0, 2);  // Compara os primeiros 2 caracteres de receivedData com "m0" e "m1"
    if (prefix == "m1") {
        Serial.println("modo manual ativado");
        while(1){
          digitalWrite(vent, LOW);
          if (Serial.available()) {            
            String receivedData = Serial.readStringUntil('\n'); // Lê a linha recebida       
            Serial.println("Recebido do ESP32: " + receivedData);  // Exibe os dados recebidos no monitor serial     
            String prefix = receivedData.substring(0, 2);
            if (prefix == "m1") {
              Serial.println("modo manual ativado");
            }else if (prefix == "m0") {
              Serial.println("modo manual desativado");
              break;
            }else if(prefix == "t0"){
              fecharTeto();
            }else if(prefix == "t1"){
              abrirTeto();
            }else{
              Serial.println("Modo desconhecido");
            }
          }
        }
    } else if (prefix == "m0") {
        Serial.println("modo manual desativado");
    } else {
        Serial.println("Modo desconhecido");
    }
    receivedData = ""; // Limpa a string recebida
  }
  if((tempAt - tempPa) >= intervalotemp){ //de dois em dois segundo faz a leitura
    tempPa = tempAt;

    pegarTemperatura(temperatura);

    if(temperatura[1]<temperatura[0]){ //compara a temperatura de dentro com a de fora
      abrirTeto();
    }else{
      fecharTeto();
    } 
  }
  
}

void pegarTemperatura(float *temperatura){ //passa o ponteiro do array

  // Leitura do primeiro sensor
  float h1 = dht1.readHumidity();
  float t1 = dht1.readTemperature();
  float f1 = dht1.readTemperature(true);

  // Verifica se houve falha na leitura do primeiro sensor
  if (isnan(h1) || isnan(t1) || isnan(f1)) {
    Serial.println("Falha na leitura do sensor DHT1!");
  } else {
    // Calcula o índice de calor (heat index) para o primeiro sensor
    float hif1 = dht1.computeHeatIndex(f1, h1);
    float hic1 = dht1.computeHeatIndex(t1, h1, false);
  }

  // Leitura do segundo sensor
  float h2 = dht2.readHumidity();
  float t2 = dht2.readTemperature();
  float f2 = dht2.readTemperature(true);

  // Verifica se houve falha na leitura do segundo sensor
  if (isnan(h2) || isnan(t2) || isnan(f2)) {
    Serial.println("Falha na leitura do sensor DHT2!");
  } else {
    // Calcula o índice de calor (heat index) para o segundo sensor
    float hif2 = dht2.computeHeatIndex(f2, h2);
    float hic2 = dht2.computeHeatIndex(t2, h2, false);
  } 
  temperatura[0] = t1; //troca os valores no array temperatura
  temperatura[1] = t2;  

}

void abrirTeto(){
 Serial.println("abrir");
    for (; pos <= maxPos; pos += 1) { 
    Servo1.write(pos); // Define o servo para a posição 'pos'
    delay(15);          // Aguarda 15 milissegundos para desacelerar o movimento
  }
  delay(1000); // Aguarda 1 segundo antes de inverter a direção
}


void fecharTeto(){
  Serial.println("fechar");
  for (; pos >= minPos; pos -= 1) { 
    Servo1.write(pos); // Define o servo para a posição 'pos'
    delay(15);         // Aguarda 15 milissegundos para desacelerar o movimento
  }
  delay(1000); // Aguarda 1 segundo antes de inverter a direção  
}

