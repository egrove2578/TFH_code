/*
 18745 RPCS, S'13
 TFH Group
 
 Main functionality of TFH system control.  Includes control of bubble column LEDs, IR remote, and relay-controlled devices: Fan, Aromatherapy Dispenser
 */

#include "LED.h"
#include "ir_remote.h"
#include "relay.h"
#include "TFH_main.h"

/******************************************************************
 * GLOBALS
 *******************************************************************/

/// controllers for each device
LED_controller LED_control;
ir_controller ir_control;
relay_controller relay_control;

/// buffer for reading from serial input
char buffer[BYTES_PER_MESSAGE];

/// state variables for devices
int LED_current_color;
int LED_desired_color;
byte relay_data;
bool aroma_current;
bool aroma_desired;
bool boxfan_current;
bool boxfan_desired;
bool projector_current;
bool projector_desired;
bool bubble_current;
bool bubble_desired;

/******************************************************************
 * void setup
 * -- Construct controllers for each device
 * -- Start serial communication
 *******************************************************************/
void setup() {
  Serial.begin(9600);
}

/******************************************************************
 * void loop
 * -- read from the serial port, parse desired device states from 
 * the data.  If the data is different from what is already set,
 * set the device to the desired state.
 * 
 * Assumes data is in the following form:
 * byte 0: 'r' (the header)
 * byte 1: a value from 0 to 6 representing the bubble column color
 * byte 2: relay states, broken into:
 * bit 0-1 - bubble column current power state, desired power state (0 is off, 1 is on)
 * bit 2-3 - aromatherapy device current power state, desired power state
 * bit 4-5 - box fan current power state, desired power state
 * bit 6-7 - state of IR remote
 * byte 3: 'p' (the footer)
 *******************************************************************/
void loop() {

  if (DEBUG) {
    test();
  }
  else {
    char header_buf;

    if (Serial.available() > 0) {
      header_buf = Serial.read();

      /// recognize the header
      if (HEAD == header_buf) {
        for (int i = 0; i < BYTES_PER_MESSAGE; i++) {

          if (Serial.available()) {
            /// read in the message
            buffer[i] = Serial.read();
          }
          else {
            Serial.println("error: not enough bytes in message!");
            break;  
          }
        }
        
        if (Serial.available()) {
          /// read the footer
          header_buf = Serial.read();
          if (FOOT == header_buf) {
            process_commands(buffer);
          }
          else {
            Serial.println("error: no footer at the end of message!");
          }
        }  
      }
    }
  }
}


void test() {
  for (int i = 0; i < NUM_COLORS; i++) {
    LED_control.set_color(i);
    LED_current_color = i;
    Serial.println(LED_control.get_current_color());
    delay(10000);
    
    ir_control.toggle();
    
    relay_control.setDevice(i%3, 1);
    relay_control.setDevice((i+1)%3, 0);
    relay_control.setDevice((i+2)%3, 0);
  }
  
}


void process_commands(char* buffer) {
  
  /// parse commands from the message
  LED_desired_color = buffer[DESIRED_COLOR_INDEX];
  
  relay_data = buffer[RELAY_DATA_INDEX];
  aroma_current = (relay_data & CURRENT_AROMA_MASK);
  aroma_desired = (relay_data & DESIRED_AROMA_MASK);
  boxfan_current = (relay_data & CURRENT_FAN_MASK);
  boxfan_desired = (relay_data & DESIRED_FAN_MASK);
  bubble_current = (relay_data & CURRENT_BUBBLE_MASK);
  bubble_desired = (relay_data & DESIRED_BUBBLE_MASK);
  projector_current = (relay_data & CURRENT_IR_MASK);
  projector_desired = (relay_data & DESIRED_IR_MASK);
  
  /// update the color of the bubble column
  if (LED_desired_color != LED_current_color) {
    LED_control.set_color(LED_desired_color);
  }
  
  /// update the state of the aromatherapy diffuser
  if (aroma_current != aroma_desired) {
    if (relay_control.setDevice(RELAY_AROMA_1, aroma_desired)) {
      aroma_current = aroma_desired;
    } 
  }
  
  /// update the state of the box fan
  if (boxfan_current != boxfan_desired) {
    if (relay_control.setDevice(RELAY_BOXFAN, boxfan_desired)) {
      boxfan_current = boxfan_desired;
    }
  }
  
  /// update the state of the bubble column motor
  if (bubble_current != bubble_desired) {
    if (relay_control.setDevice(RELAY_BUBBLE, bubble_desired)) {
      bubble_current = bubble_desired;
    }
  }
  
  /// update the state of the projector
  if (projector_current != projector_desired) {
    if (ir_control.setDevice(projector_desired)) {
      projector_current = projector_desired;
    }
  }
}


