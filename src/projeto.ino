#include <IRremote.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int trigPin = 6;
const int echoPin = 7;
const int ledvermPin = 4;
const int ledverdPin = 5;
const int buzzPin = 2;
const int distanciaAlarme = 15;
int RECV_PIN = 8;

int distancia;

LiquidCrystal_I2C lcd(0x27,20,4);  // set the LCD address to 0x27 for a 16 chars and 2 line display
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

  lcd.init();                      // initialize the lcd
  lcd.init();
  // Print a message to the LCD.
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
