#include <Servo.h>
#include <SPI.h>
#include "Adafruit_MAX31855.h"


#define MAXDO   3
#define MAXCS   4
#define MAXCLK  5


Adafruit_MAX31855 thermocouple(MAXCLK, MAXCS, MAXDO);

void check();
void stop();
void zero();
void delay();
void sped();
void angl();
void read();


int analogPinIn = A1; //white
int ledPin = 13;
int relayPin = 5; //yellow
int time = 3000;    // changes the delay the servo makes in milisecionds
int speed = 3;    // changes the speed of the servo in degrees/15millisecionds
int angle = 90;   // change the angle the servo moves too in degrees
int pos = 0;    // used to change position of servo in for loop
int clock = 0;    // used to set delay in for loop
unsigned long tim = millis();
String entry = "zero";
String comp = "empty";
Servo myservo;


void setup() {
  Serial.begin(115200);
  pinMode(relayPin, OUTPUT);
  pinMode(ledPin, OUTPUT);
  myservo.attach(9);
}

void loop() {
  check();
  if (entry == "s"){
    stop();
  }
  if (entry == "zero"){
    zero();
  }
  if (entry == "delay"){
    delay();
  }
  if (entry == "speed"){
    sped();
  }
  if (entry == "angle"){
    angl();
  }
  if (entry == "start"){
    read();
    for (pos = 0; pos <= angle; pos += speed){
      check();
      read();
      if (entry == "s"){
        break;
      }
      myservo.write(pos);
      delay(15);
      read();
    }
    for (clock = 0; clock <= time; clock += 10){
      check();
      read();
      if (entry == "s"){
        break;
      } else{
        delay(10);
        read();
      }
    }
    for (pos = pos; pos >= 0; pos -= speed){
      check();
      read();
      if (entry == "s"){
        break;
      }
      myservo.write(pos);
      delay(15);
      read();
    }
    read();
    if (entry != "s"){
      entry = "empty";  
    }
  }
}





void check() {
  if (Serial.available()>0){
   entry= Serial.readString();
   entry.trim();
 }
}

void stop() {
  Serial.println("sequence stopped type resume to continue");
  while (entry == "s"){
    myservo.write(0);
    if (Serial.available()>0){
      comp = Serial.readString();
      comp.trim();
    }
    if (comp == "resume"){
      Serial.println("resuming");
      entry = "empty";
     comp = "empty";
    }
  }
}

void zero() {
  myservo.write(0);
  delay(15);
  entry = "empty";
}

void delay() {
  Serial.println("enter value");
  while (entry=="delay") {
    if (Serial.available()>0){
      time= (Serial.read() - 48)*1000;
      Serial.print("value set to ");
      Serial.println(time, DEC);
      entry= "empty";
    }
  }
}

void sped() {
 Serial.println("enter value");
  while (entry == "speed"){
    if (Serial.available()>0){
      speed = (Serial.read() - 48);
      Serial.print("value set to ");
      Serial.println(speed, DEC);
      entry = "empty";
    }
  }
}

void angl() {
  Serial.println("enter value");
  while (entry == "angle"){
    if (Serial.available()>0){
      angle = (Serial.read() - 48)*10;
      Serial.print("value set to ");
      Serial.println(angle, DEC);
      entry = "empty";
    }
  }
}

void read() {
  tim = millis();
  Serial.print(tim);
  Serial.print(":");
  Serial.print("\t");
  double c = thermocouple.readCelsius();
  Serial.print(c);
  Serial.println();
}





