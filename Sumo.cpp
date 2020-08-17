#include "Arduino.h"
#include "Sumo.h"

void Sumo::Begin(){
  pinMode(TRIGR, OUTPUT); 
  pinMode(TRIGL, OUTPUT);
  pinMode(ENA, OUTPUT);
  pinMode(ENB, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ECHOR, INPUT);
  pinMode(ECHOL, INPUT);
  pinMode(IR, INPUT);
  pinMode(START, INPUT);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);

  go = false;
  black = true;
  autonomous = true;
  inChar = -1;
}

void Sumo::setDistance(){
  lDistance = getLftDistance();
  if(lDistance == 0 || lDistance > RINGDIAMETER){
    lDistance = RINGDIAMETER + 1;
  }
  //delay(4);//Need this delay so waves don't interfere with one another
  rDistance = getRtDistance();
  if(rDistance == 0 || rDistance > RINGDIAMETER){
    rDistance = RINGDIAMETER + 1;
  }
}

int Sumo::getLftDistance(){
  // Sends out the signal 10 ms apart which is the minimum time needed to use pulsein
  digitalWrite(TRIGL, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIGL, LOW);
  //Starts timing when input is high, and stops timing when input is low, returns value
  long duration = pulseIn(ECHOL, HIGH, 10000);
  // Calculating the distance (cm), .034 cm/s, distance = time * velocity / 2
  int distance= duration*0.034/2;
  return distance;
}

int Sumo::getRtDistance(){
  // Sends out the signal 10 ms apart which is the minimum time needed to use pulsein
  digitalWrite(TRIGR, HIGH);
  delay(10);
  digitalWrite(TRIGR, LOW);
  //Starts timing when input is high, and stops timing when input is low, returns value
  long duration = pulseIn(ECHOR, HIGH, 10000);
  // Calculating the distance (cm), .034 cm/s, distance = time * velocity / 2
  int distance= duration*0.034/2;
  return distance;
}

void Sumo::push(){
  while(black == true && go == true){
    forwards(PUSHSPEED);
  }
  black = true;//the interrupt will set this
}

void Sumo::backwards(int vel, int del) {
  //set speed
  analogWrite(ENA, vel);
  analogWrite(ENB, vel);
  // Turn on motors
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
  delay(del);
}

void Sumo::rotateLeft(int vel) {
  // Set the speed
  analogWrite(ENA, vel);
  analogWrite(ENB, vel);
  // Turn on motors
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void Sumo::rotateRight(int vel) {
  // Set the speed
  analogWrite(ENA, vel);
  analogWrite(ENB, vel);
  // Turn on motors
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}

void Sumo::forwards(int vel) {
  // Set the speed
  analogWrite(ENA, vel);
  analogWrite(ENB, vel);
  // Turn on motors
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}

void Sumo::halt(){
  // Set the speed
  analogWrite(ENA, 0);
  analogWrite(ENB, 0);
  // Turn on motors
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
}
