#include <IRremote.h>

const int trigPin = 6;
const int echoPin = 7;
const int ledvermPin = 4;
const int ledverdPin = 5;
const int buzzPin = 2;
const int distanciaAlarme = 10;
int RECV_PIN = 8;

int distancia;

IRrecv irrecv(RECV_PIN);

decode_results results;

void setup() {

  Serial.begin(9600);
  pinMode(ledvermPin, OUTPUT);
  pinMode(ledverdPin, OUTPUT);
  pinMode(buzzPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  irrecv.enableIRIn(); // Start the receiver
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
  delay(distancia*100-30);
  
  if (irrecv.decode(&results)) {
    Serial.println(results.value, HEX);
    irrecv.resume(); // Receive the next value
  }
  
  if (results.value == 0xFFC23D) {
        
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
  
   desarma();
   
 }
 
}

void desarma() {

  digitalWrite(ledvermPin, LOW);
  digitalWrite(ledverdPin, LOW);
  digitalWrite(buzzPin, LOW);

}
