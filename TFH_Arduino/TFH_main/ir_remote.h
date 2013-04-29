/*
 18540 RPCS, S'13
 TFH Group
 
 Header for the ir remote control code. Includes hard coded designations for the
 things we are turning on and off.
 
 */

#ifndef IR_REMOTE_H
#define IR_REMOTE_H

#if defined(ARDUINO) && ARDUINO >= 100
#include <Arduino.h>
#else
#include <WProgram.h>
#include <pins_arduino.h>
#endif


#include <stdint.h>


#define IR_LED_pin 53


// these control the relay timing so the wireless signal gets sent long enough
#define MIN_PRESS_TIME    100
#define MIN_REPEAT_TIME   600


class ir_controller {
public:
  ir_controller();
  boolean setDevice(int);
  boolean turnOn();
  boolean turnOff();
  boolean toggle();
  boolean getDeviceState();
  
private:
  void do_ir_sequence();
  void power();
  void pulseIR(long);
  int ir_pin;
  boolean ir_state;
  unsigned long last_ir_press;
  
};

#endif
