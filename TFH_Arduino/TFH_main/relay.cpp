/*
 18540 RPCS, S'13
 TFH Group
 
 Implementation file for the relay functionality
 */

#include "relay.h"

/********************************************************************************
 constructor fun
********************************************************************************/
relay_controller::relay_controller()
{
  relayPins[0] = relay1;
  relayPins[1] = relay2;
  relayPins[2] = relay3;
  relayPins[3] = relay4;
  relayPins[4] = relay5;
  relayPins[5] = relay6;
  
  for(int i = 0; i < 6; i++) {
    pinMode(relayPins[i],OUTPUT);
    digitalWrite(relayPins[i],HIGH);
    relay_state[i] = false;
    last_relay_press[i] = millis();
  }

  numberOfRelays = 6;
}

/********************************************************************************
 constructor fun, use this if you aren't using all the relays
********************************************************************************/
relay_controller::relay_controller(int howManyRelays)
{
  if(howManyRelays < 0) {
    howManyRelays = 0;
  }
  if(howManyRelays > 6) {
    howManyRelays = 6;
  }
    
  relayPins[0] = relay1;
  relayPins[1] = relay2;
  relayPins[2] = relay3;
  relayPins[3] = relay4;
  relayPins[4] = relay5;
  relayPins[5] = relay6;
  
  for(int i = 0; i < howManyRelays; i++) {
    pinMode(relayPins[i],OUTPUT);
    digitalWrite(relayPins[i],HIGH);
    relay_state[i] = false;
    last_relay_press[i] = millis();
  }
  
  numberOfRelays = howManyRelays;
}

/********************************************************************************
 returns false if the device setting was unsuccessful, else returns true
 reason why the function will fail
 - there wasn't enough time for the last command to finish
 - the sent relay isn't in the range of 1 to 6
********************************************************************************/
boolean relay_controller::setDevice(int theRelay, int theState)
{
  // fail check for out of range values
  if(theRelay < 0 || theRelay > numberOfRelays-1) {
    return false;
  }
  
  if(theState > 0 && relay_state[theRelay]) {
    return true;
  }
  if(theState == 0 && relay_state[theRelay] == false) {
    return true;
  }
  
  if(millis() - last_relay_press[theRelay] > MIN_REPEAT_TIME) {
    digitalWrite(relayPins[theRelay],LOW);
    delay(MIN_PRESS_TIME);
    digitalWrite(relayPins[theRelay],HIGH);
    last_relay_press[theRelay] = millis();
    relay_state[theRelay] = theState;
    return true;
  }
  return false;
}

/********************************************************************************
 set a device's state. just in case you need to force it
********************************************************************************/
void relay_controller::setDeviceState(int theRelay, int theState)
{
  // fail check for out of range values
  if(theRelay < 0 || theRelay > numberOfRelays-1) {
    return;
  }
  
  relay_state[theRelay] = theState;
}


/********************************************************************************
 
********************************************************************************/
boolean relay_controller::turnOnDevice(int theRelay)
{
  // fail check for out of range values
  if(theRelay < 0 || theRelay > numberOfRelays-1) {
    return false;
  }
  
  if(relay_state[theRelay] > 0) {
    return true;
  }
  
  if(millis() - last_relay_press[theRelay] > MIN_REPEAT_TIME) {
    digitalWrite(relayPins[theRelay],LOW);
    delay(MIN_PRESS_TIME);
    digitalWrite(relayPins[theRelay],HIGH);
    last_relay_press[theRelay] = millis();
    relay_state[theRelay] = true;
    return true;
  }
  return false;
}

/********************************************************************************

********************************************************************************/
boolean relay_controller::turnOffDevice(int theRelay)
{
  // fail check for out of range values
  if(theRelay < 0 || theRelay > numberOfRelays-1) {
    return false;
  }
  
  if(relay_state[theRelay] == 0) {
    return true;
  }
  
  if(millis() - last_relay_press[theRelay] > MIN_REPEAT_TIME) {
    digitalWrite(relayPins[theRelay],LOW);
    delay(MIN_PRESS_TIME);
    digitalWrite(relayPins[theRelay],HIGH);
    last_relay_press[theRelay] = millis();
    relay_state[theRelay] = false;
    return true;
  }
  return false;
}

/********************************************************************************
 
********************************************************************************/
boolean relay_controller::toggleDevice(int theRelay)
{
  // fail check for out of range values
  if(theRelay < 0 || theRelay > numberOfRelays-1) {
    return false;
  }
  
  if(millis() - last_relay_press[theRelay] > MIN_REPEAT_TIME) {
    digitalWrite(relayPins[theRelay],LOW);
    delay(MIN_PRESS_TIME);
    digitalWrite(relayPins[theRelay],HIGH);
    last_relay_press[theRelay] = millis();
    relay_state[theRelay] = !relay_state[theRelay];
    return true;
  }
  return false;
}

/********************************************************************************
 
********************************************************************************/
boolean relay_controller::getDeviceState(int theRelay)
{
  // fail check for out of range values
  if(theRelay < 0 || theRelay > numberOfRelays-1) {
    return false;
  }
  
  return relay_state[theRelay];
}


