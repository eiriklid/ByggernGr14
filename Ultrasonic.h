/*
  Ultrasonic.h - Library for HR-SC04 Ultrasonic Ranging Module.
  Created by ITead studio. Alex, Apr 20, 2010.
  iteadstudio.com
*/


#ifndef Ultrasonic_h
#define Ultrasonic_h

#include "Arduino.h"

class Ultrasonic
{
  public:
    Ultrasonic(int TP, int EP); //sets Trig_pin and Echo_pin
    long Timing();              //driver for the sensor
    long Ranging();             //converts the ouput from Timing to centimeters

    private:
    int Trig_pin;
    int Echo_pin;
    long  duration,distance_cm;
    
};

#endif
