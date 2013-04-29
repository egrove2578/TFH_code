/*
 18540 RPCS, S'13
 TFH Group
 
 Header for the relay control code. Includes hard coded designations for the
 things we are turning on and off.
 
 RELAY_AROMA_1 (assumes we would be having more than 1 aroma fan, hypotheticlaly)
 RELAY_BOXFAN
*/

#ifndef RELAY_H
#define RELAY_H

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#include <pins_arduino.h>
#endif


#include <stdint.h>

// these designate which relay control the device is connected to, not the pins
// that the relay is plugged in on
#define RELAY_AROMA_1     1
#define RELAY_BOXFAN      2
#define RELAY_BUBBLE      3


// plugs for the first through sixth relay plugs. plug in what you need.
#define relay1            48
#define relay2            49
#define relay3            50
#define relay4            0
#define relay5            0
#define relay6            0


// these control the relay timing so the wireless signal gets sent long enough
#define MIN_PRESS_TIME    100
#define MIN_REPEAT_TIME   600


class relay_controller {
public:
  relay_controller();
  relay_controller(int);
  boolean setDevice(int,int);
  void setDeviceState(int,int);
  boolean turnOnDevice(int);
  boolean turnOffDevice(int);
  boolean toggleDevice(int);
  boolean getDeviceState(int);
  
private:
  int numberOfRelays;
  int relayPins[6];
  boolean relay_state[6];
  unsigned long last_relay_press[6];
  
};

#endif
