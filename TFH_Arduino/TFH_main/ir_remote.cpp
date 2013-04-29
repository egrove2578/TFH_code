/*
 18540 RPCS, S'13
 TFH Group
 
 Implementation file for the ir remote functionality
 */

#include "ir_remote.h"

// Signals for the led
int off[] = { 
  3740 ,4460 ,560 ,560 ,560 ,1660 ,560 ,560 ,1680 ,560 ,540 ,560 ,540 ,540 ,1680 ,560 ,1660 ,540 
    ,560 ,1660 ,560 ,560 ,540 ,560 ,540 ,540 ,1680 ,540 ,1680 ,1660 ,1660 ,1660 ,1680 ,1660 ,61460 ,4460 ,540 
    ,540 ,540 ,1680 ,560 ,560 ,1660 ,560 ,560 ,540 ,560 ,540 ,1660 ,560 ,1660 ,560 ,560 ,1660 ,540 ,560 ,560 
    ,540 ,560 ,560 ,1660 ,560 ,1660 ,1680 ,1660 ,1660 ,1660 ,1680};

int on[] = {  
  8820, 580, 560, 580, 580, 580, 580, 560, 580, 580, 580, 580, 600, 580, 560, 580, 600, 580, 
  580, 580, 580, 580, 580, 580, 600, 580, 580, 580, 580, 600, 580, 580, 580, 580, 8820, 600, 580, 600, 580, 
  560, 580, 580, 580, 580, 580, 580, 580, 600, 580, 580, 580, 580, 580, 580, 580, 580, 580, 580, 580, 580, 
  580, 580, 580, 580, 600, 580, 580, 580};

/********************************************************************************
 constructor fun
********************************************************************************/
ir_controller::ir_controller()
{
  ir_pin = IR_LED_pin;
  
  pinMode(ir_pin,OUTPUT);
  digitalWrite(ir_pin,LOW);
  ir_state = false; // assume off
  last_ir_press = millis();
}


/********************************************************************************
 returns false if the device setting was unsuccessful, else returns true
 reason why the function will fail
 - there wasn't enough time for the last command to finish
 - the sent relay isn't in the range of 1 to 6
********************************************************************************/
boolean ir_controller::setDevice(int theState)
{
  if(theState > 0 && ir_state) {
    return true;
  }
  if(theState == 0 && ir_state == false) {
    return true;
  }
  
  if(millis() - last_ir_press > MIN_REPEAT_TIME) {
    do_ir_sequence();
    last_ir_press = millis();
    ir_state = theState;
    return true;
  }
  return false;
}


/********************************************************************************
 
********************************************************************************/
boolean ir_controller::turnOn()
{
  if(ir_state > 0) {
    return true;
  }
  
  if(millis() - last_ir_press > MIN_REPEAT_TIME) {
    do_ir_sequence();
    last_ir_press = millis();
    ir_state = true;
    return true;
  }
  return false;
}

/********************************************************************************
 
********************************************************************************/
boolean ir_controller::turnOff()
{
  if(ir_state == 0) {
    return true;
  }
  
  if(millis() - last_ir_press > MIN_REPEAT_TIME) {
    do_ir_sequence();
    last_ir_press = millis();
    ir_state = false;
    return true;
  }
  return false;
}

/********************************************************************************
 
********************************************************************************/
boolean ir_controller::toggle()
{
  if(millis() - last_ir_press > MIN_REPEAT_TIME) {
    do_ir_sequence();
    last_ir_press = millis();
    ir_state = !ir_state;
    return true;
  }
  return false;
}

/********************************************************************************
 
********************************************************************************/
boolean ir_controller::getDeviceState()
{
  return ir_state;
}

/********************************************************************************
 
********************************************************************************/
void ir_controller::do_ir_sequence() {
  // still need to get the control sequence for the projector
  // TODO: get IR control sequence to toggle projector power
  Serial.println("toggling power");
  power();  
}
  
  
      // This procedure sends a 38KHz pulse to the IRledPin
// for a certain # of microseconds. We'll use this whenever we need to send codes
void ir_controller::pulseIR(long microsecs) {
  // we'll count down from the number of microseconds we are told to wait
  cli(); // this turns off any background interrupts
  while (microsecs > 0) {
    // 38 kHz is about 13 microseconds high and 13 microseconds low
    digitalWrite(ir_pin, HIGH); // this takes about 3 microseconds to happen
    delayMicroseconds(10); // hang out for 10 microseconds
    digitalWrite(ir_pin, LOW); // this also takes about 3 microseconds
    delayMicroseconds(10); // hang out for 10 microseconds
    // so 26 microseconds altogether
    microsecs -= 26;
  }
  sei(); // this turns them back on
}

void ir_controller::power() {
  for (int i = 0; i  < 68; i++) {
    delayMicroseconds(off[i]);
    pulseIR(on[i]);
  }
}

