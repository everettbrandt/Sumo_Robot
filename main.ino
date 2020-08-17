#include "Sumo.h"
#include <Adafruit_SSD1306.h>
#include "SoftwareSerial.h"

Sumo sumo;
SoftwareSerial serialConnection(0, 1);
Adafruit_SSD1306 display(-1);
int timer1, timer2;

void setup() {
  sumo.Begin();
  Serial.begin(9600);
  serialConnection.begin(9600);//Initialize communications with the bluetooth module
  
  Wire.setSCL(33);//oled display pins for I2C
  Wire.setSDA(34);
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.display();
  sumo.Begin();

  attachInterrupt(digitalPinToInterrupt(START), buttonPush, FALLING);
  attachInterrupt(digitalPinToInterrupt(IR), irISR, CHANGE); 
}
void loop() {
  sumo.setDistance();
  display.clearDisplay();
  display.setCursor(0,0);
  display.print(sumo.lDistance);
  display.setCursor(0,16);
  display.print(sumo.rDistance);

  checkBluetooth();
  if(sumo.autonomous == true){
    if(sumo.go == true){
      sensor_control();
    }
    if(sumo.go == false){
      sumo.halt();
      display.setCursor(32, 16);
      display.print("OFF");
    }
  }
  else{
    display.setCursor(32,0);
    display.print("RC CNTRL");
  }
  display.display();//displays all OLED values
}

void sensor_control(){
  display.setCursor(32, 0);
  if(sumo.rDistance > RINGDIAMETER && sumo.lDistance > RINGDIAMETER){//Searching for the object
    if(millis() - timer1 < ROTATEDELAY){
      display.print("LEFT");
      sumo.rotateLeft(SPEED);
    }
    else{
      display.print("FOWARD");
      sumo.forwards(SPEED);
      display.display();
      delay(MOVEDELAY);
      timer1 = millis();
    }
  }
  else if(sumo.lDistance < PUSHDISTANCE || sumo.rDistance < PUSHDISTANCE){//push object
    display.print("PUSH");
    display.display();
    sumo.push();
    timer1 = millis();
  }
  else if(sumo.lDistance < RINGDIAMETER && sumo.rDistance < RINGDIAMETER){// found  , Move forwards
    display.print("BLOCK");
    sumo.forwards(SPEED);
    timer1 = millis;
  }
  else if(sumo.lDistance < RINGDIAMETER){//turn left towards block
    display.print("LEFT");
    sumo.rotateLeft(SPEED);
    timer1 = millis;
  }
  else if(sumo.rDistance < RINGDIAMETER){//turn right towards block
    display.print("RIGHT");
    sumo.rotateRight(SPEED);
    timer1 = millis();
  }
  else{//stop
    display.print("HALT");
    sumo.halt();
    timer1 = millis();
  }
}

void checkBluetooth(){
  if(serialConnection.available() > 0){
    sumo.inChar = serialConnection.read();//Read one byte
    if(sumo.inChar == ' '){
      sumo.autonomous = !sumo.autonomous;
    }
    if(sumo.autonomous == true){
      return;
    }
    else{
      display.setCursor(32,16);
      if(sumo.inChar == 'r'){
        sumo.rotateRight(PUSHSPEED);
        display.print("RIGHT");
      }
      else if(sumo.inChar == 'l'){
        sumo.rotateLeft(PUSHSPEED);
        display.print("LEFT");
      }
      else if(sumo.inChar == 'u'){
        sumo.forwards(PUSHSPEED);
        display.println("Forwards");
      }
      else if(sumo.inChar == 'd'){
        sumo.backwards(PUSHSPEED, 0);
        display.println("Backwards");
      }
      display.display();
      timer2 = millis();
      //put in a delay so that buffer can be read and
      //motors could actually go for a second
      while(millis() - timer2 > 50){
        serialConnection.read();
      }
      sumo.halt();
    }
  }
}

void buttonPush(){
  sumo.go = ! sumo.go;
  timer1 = millis();//initializing timer here because this is where robots starts
}

void irISR(){
    sumo.black = false;
    if(sumo.go == true && sumo.autonomous == true){
      sumo.backwards(PUSHSPEED, BACKDELAY);
    }
}
