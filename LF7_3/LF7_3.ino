// genutzte Bibliotheken definieren:
#include <LiquidCrystal.h>
#include <Servo.h>

// Messwerte in Zentimetern
const float MinMeasuredDistance = 5;
const float MaxMeasuredDistance = 20;

// Zisternenabstände in Metern
const float MaxDistance = 4;
const float MinDistance = 0;

const int DefaultDelay = 1500; // 1,5 Sekunden Pause

// Ultrachallsensor Pinbelegung
#define trigPin 8
#define echoPin 9

#define ButtonPin 13
bool showDistance = false;
#define LedPin 7 //LED Pinbelegung
#define MotorPin 10 //Servo Pinbelegung
const Servo servoMain; 

// LCD Pinbelegung definieren
const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

void setup() {
 // Ultraschallsensor Pinmodus festlegen
 pinMode(trigPin, OUTPUT);
 pinMode(echoPin, INPUT);
 pinMode(ButtonPin, INPUT_PULLUP);
 pinMode(LedPin, OUTPUT); //LED Pinmodus festlegen
 servoMain.attach(MotorPin); //Servo init
 
// Initialisierung der seriellen Kommunikation
 Serial.begin(9600);

 //LCD Größe definieren
 lcd.begin(16, 2);

 //Startanzeige auf LCD anzeigen 
 lcd.print("Ultraschall-");
 lcd.setCursor(0, 1);
 lcd.print("Sensor-Messung");

 delay(2000); // 1 Sekunden Pause
}

void loop() {
 //Knopf überprüfen
 if (digitalRead(ButtonPin) == 0){
   showDistance = !showDistance;//Bei aktivierung Modus ändern
   lcd.clear();
   lcd.print("OK");//Anzeigen, dass Knopfdruck wahrgenommen wurde
   while(digitalRead(ButtonPin) == 0); //Programm anhalten, solange Knopf gedrückt bleibt
 }

 int filledPercent = filledInPercent(measureDistance()); //Füllstand in Prozent
  
 displayWaterlevel(filledPercent); //Anzeige des Wasserstands
 
 manageServo(filledPercent); //Schleuse öffnen/schließen (<25%/>75%)

 if(filledPercent <= 25){
   displayWarning(); //Anzeige der Warnung
 }

 writeSerial(filledPercent);
}

//Entfernung des Ultraschallsensors messen
//Return: Entfernung in cm
float measureDistance(){
 digitalWrite(trigPin, LOW); // Setzt den Trigger-Pin auf Low
 delayMicroseconds(2); // Wartet 2 Mikrosekunden
 digitalWrite(trigPin, HIGH); // Setzt den Trigger-Pin auf High
 delayMicroseconds(10); // Wartet 10 Mikrosekunden
 digitalWrite(trigPin, LOW); // Setzt den Trigger-Pin auf Low
 long duration = pulseIn(echoPin, HIGH); // Liest die Entfernungsdauer vom Echo-Pin
 return duration * 0.034 / 2; // Berechnet die Entfernung in Zentimetern
}

//Wasserstand des PoC auf Auftragsmaße anwenden
float distanceLeftToScale(float percent){
  return (MaxDistance - MinDistance) * (percent/100);
}

//Messwerte in cm zu prozentualem Wasserstand umrechnen
int filledInPercent(float distance){
  float heightDifference = MaxMeasuredDistance - MinMeasuredDistance;
  float percent = (distance - MinMeasuredDistance) / heightDifference;
  return (1 - percent) * 100;
}

//Füllstand und Entfernung zum Wasser anzeigen
void displayWaterlevel(int filledPercent){
 lcd.clear();
 lcd.setCursor(0, 0);
 if (showDistance){
  lcd.print("Entfernung:");
  lcd.setCursor(0, 1);
  lcd.print(distanceLeftToScale(filledPercent));
  lcd.print("m");
 } else {
  lcd.print("Fuellstand:");
  lcd.setCursor(0, 1);
  lcd.print(filledPercent);
  lcd.print("%");
 }
 delay(DefaultDelay);
}

//Warnung bei zu geringem Füllstand anzeigen
void displayWarning(){
 lcd.clear();
 lcd.setCursor(2, 0);
 lcd.print("!!!WARNUNG!!!");
 lcd.setCursor(0, 1);
 lcd.print("! Wenig Wasser !");
 digitalWrite(LedPin, HIGH);

 delay(DefaultDelay);
 digitalWrite(LedPin, LOW);
}

//Schleuse öffnen/schließen
void manageServo(int filledPercent){
  if(filledPercent > 75){
    servoMain.write(0);
  }
  else if (filledPercent < 25){
    servoMain.write(180);
  }
}

// Ausgabe des Füllstands via serial zur anbindung an csv/db mithilfe von zusatzprogrammen (z.B. PuTTY)
void writeSerial(int filledPercent){
  // Aktuelles Datum und Uhrzeit abrufen
  String timestamp = getTimeStamp();

  /* zweites mögliches ausgabeformat
  // Sensorwert und Zeitstempel seriell ausgeben 
  Serial.print("Füllstand: ");
  Serial.println(filledPercent);
  Serial.print("Zeitstempel: ");
  Serial.println(timestamp);
  */

  // Sensorwert und Zeitstempel an den Computer senden
  Serial.print(timestamp);
  Serial.print(",");
  Serial.println(sensorValue);
}

String getTimeStamp() {
  // Aktuelle Zeit abrufen
  unsigned long currentTime = millis();

  // Zeit in das Format "TT.MM.JJJJ HH:MM:SS" umwandeln
  String timestamp;
  timestamp += String(day()) + ".";
  timestamp += String(month()) + ".";
  timestamp += String(year()) + " ";
  timestamp += String(hour()) + ":";
  timestamp += String(minute()) + ":";
  timestamp += String(second());

  return timestamp;
}