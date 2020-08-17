#ifndef Sumo_h
#define Sumo_h

#include "Arduino.h"

//Left motor connections
#define IN2 5
#define IN1 6
#define ENA 7

// Right motor connection
#define ENB 2
#define IN3 4
#define IN4 3

//Sensor pins
#define TRIGR 8
#define ECHOR 9
#define IR 10
#define TRIGL 11
#define ECHOL 12
#define START 24

//OLED Display pins
#define CLOCK 33
#define DATA 34

#define RINGDIAMETER 77
#define PUSHDISTANCE 12
#define PUSHSPEED 125
#define SPEED 32
#define BACKDELAY 400
#define ROTATEDELAY 4000
#define MOVEDELAY 2000

class Sumo{
  public:
    int lDistance;
    int rDistance;
    char inChar;//bluetooth char input
    bool go;
    bool autonomous;
    bool black;
    void Begin();
    void setDistance();
    void push();
    void backwards(int, int);
    void rotateLeft(int);
    void rotateRight(int);
    void forwards(int);
    void halt();
  private:
    int getLftDistance();//called by setDistance
    int getRtDistance();
};

#endif
