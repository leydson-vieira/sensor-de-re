/* 

Autor: Leydson Vieira / leydson.vieira@gmail.com

Sensor de distância com display embutido

*/

/* Bibliotecas */
#include <IRremote.h>
#include <Wire.h> 
#include <LiquidCrystal_I2C.h>


/* Constantes e variáveis dos pinos de I/O */
const int trigPin = 6;
const int echoPin = 7;
const int ledvermPin = 4;
const int ledverdPin = 5;
const int buzzPin = 2;
const int distanciaAlarme = 15;
int RECV_PIN = 8;

int distancia; // Valor recebido do sensur ultrassônico

LiquidCrystal_I2C lcd(0x27,20,4);  // Seta o endereço do display de 16 colunas e 2 linhas.
IRrecv irrecv(RECV_PIN);           // Objeto do receptor infravermelho.

decode_results results;

void setup()
{
  Serial.begin(9600);
  pinMode(ledvermPin, OUTPUT);
  pinMode(ledverdPin, OUTPUT);
  pinMode(buzzPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  irrecv.enableIRIn(); // Start the receiver

  lcd.init();                      // Iniciando o lcd
  lcd.backlight();
  lcd.setCursor(3,0);  
}

void loop() {
   
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  unsigned long duracao = pulseIn(echoPin, HIGH);
  distancia = duracao / 58;
  
  Serial.print("Distancia em CM: ");
  Serial.println(distancia);
  delay(500);
  lcd.clear();
  lcd.setCursor(7,0);
  lcd.print(distancia);
  lcd.setCursor(12,0);
  lcd.print("CM");
  lcd.setCursor(0,0);
  lcd.print("DIST.");
  
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    irrecv.resume(); // Receive the next value
  }
  
  if (results.value == 0xFFC23D) {
     lcd.setCursor(0.1);
     lcd.print("Ativ.");    
     if (distancia < distanciaAlarme) {

         digitalWrite(ledverdPin, LOW);    
         digitalWrite(ledvermPin, HIGH);
         digitalWrite(buzzPin, HIGH);
         delay(distancia*100-50);
         digitalWrite(ledvermPin, LOW);
         digitalWrite(buzzPin, LOW);

      } else {
        digitalWrite(ledvermPin, LOW);
        digitalWrite(ledverdPin, HIGH);
      }

   } else {
     lcd.setCursor(0.1);
     lcd.print("Desativ.");
     desarma();
   }

  }

void desarma() {
  digitalWrite(ledvermPin, LOW);
  digitalWrite(ledverdPin, LOW);
  digitalWrite(buzzPin, LOW);
}
