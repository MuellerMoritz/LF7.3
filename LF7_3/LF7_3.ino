/*
  LiquidCrystal Library - Hello World

 Demonstrates the use a 16x2 LCD display.  The LiquidCrystal
 library works with all LCD displays that are compatible with the
 Hitachi HD44780 driver. There are many of them out there, and you
 can usually tell them by the 16-pin interface.

 This sketch prints "Hello World!" to the LCD
 and shows the time.

  The circuit:
 * LCD RS pin to digital pin 12
 * LCD Enable pin to digital pin 11
 * LCD D4 pin to digital pin 5
 * LCD D5 pin to digital pin 4
 * LCD D6 pin to digital pin 3
 * LCD D7 pin to digital pin 2
 * LCD R/W pin to ground
 * LCD VSS pin to ground
 * LCD VCC pin to 5V
 * 10K resistor:
 * ends to +5V and ground
 * wiper to LCD VO pin (pin 3)

 Library originally added 18 Apr 2008
 by David A. Mellis
 library modified 5 Jul 2009
 by Limor Fried (http://www.ladyada.net)
 example added 9 Jul 2009
 by Tom Igoe
 modified 22 Nov 2010
 by Tom Igoe
 modified 7 Nov 2016
 by Arturo Guadalupi

 This example code is in the public domain.

 https://docs.arduino.cc/learn/electronics/lcd-displays

*/

// include the library code:
#include <LiquidCrystal.h>

//messung in metern
const int MaxWaterLevel = 0.01;
const int MinWaterLevel = 0.08;

//zisterne in metern
const int MaxDistance = 4;
const int MinDistance = 0;

const int trigPin = 8; // Ultraschallsensor Trigger-Pin

const int echoPin = 9; // Ultraschallsensor Echo-Pin
// initialize the library by associating any needed LCD interface pin
// with the arduino pin number it is connected to
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
 pinMode(trigPin, OUTPUT);

 pinMode(echoPin, INPUT);

 lcd.begin(16, 2); // Initialisierung des LCD-Displays

 lcd.print("Ultraschall-"); // Ausgabe auf LCD-Display

 lcd.setCursor(0, 1);

 lcd.print("Sensor-Messung");

 delay(2000); // 2 Sekunden Pause
}

void loop() {
 digitalWrite(trigPin, LOW); // Setzt den Trigger-Pin auf Low

 delayMicroseconds(2); // Wartet 2 Mikrosekunden

 digitalWrite(trigPin, HIGH); // Setzt den Trigger-Pin auf High

 delayMicroseconds(10); // Wartet 10 Mikrosekunden

 digitalWrite(trigPin, LOW); // Setzt den Trigger-Pin auf Low

 long duration = pulseIn(echoPin, HIGH); // Liest die Entfernungsdauer vom Echo-Pin





// Berechnet die Entfernung in Zentimetern (cm)
 int distanceInCm = duration * 0.034 / 2;

 int filledPercent = filledInPercent(distanceInCm);

 lcd.clear(); // Löscht das LCD-Display

 lcd.setCursor(0, 0);

 lcd.print("Entfernung: ");

 lcd.print(distanceLeftToScale(filledPercent));

 lcd.print("m"); // Ausgabe auf LCD-Display

 lcd.setCursor(0, 1);

 lcd.print("Füllstand: ");

 lcd.print(filledPercent);

 lcd.print("%"); // Ausgabe auf LCD-Display

 delay(500); // 0,5 Sekunden Pause

 if(filledPercent <= 0.25){
   displayWarning();
 }
}

//Returns distance from sensor to water
int distanceLeftToScale(int percent){
  return (MaxDistance - MinDistance) * (percent/100);
}

int filledInPercent(int distance){
  int heightDifference = MaxWaterLevel - MinWaterLevel;
  int percent = (distance - MinWaterLevel) / heightDifference;
  return percent * 100;
}

void displayWarning(){
 lcd.clear(); // Löscht das LCD-Display

 lcd.setCursor(0, 0);

 lcd.print("!!!WARNUNG!!!");

 lcd.setCursor(0, 1);

 lcd.print("Füllstand <= 25 %");

 lcd.print("%"); // Ausgabe auf LCD-Display

 delay(500); // 0,5 Sekunden Pause
}