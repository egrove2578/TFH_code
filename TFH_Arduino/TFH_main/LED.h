/*
 18745 RPCS, S'13
 TFH Group
 
 Header for the LED control
 
 */

#ifndef LED_H
#define LED_H

#include <Arduino.h>

/***************************************************************
 * PIN ASSIGNMENTS
 ****************************************************************/
#define RED1 30
#define GREEN1 31
#define BLUE1 32
#define GROUND_1 33

#define RED2 42
#define GREEN2 43
#define BLUE2 44
#define GROUND_2 45

#define RED3 38
#define GREEN3 39
#define BLUE3 40
#define GROUND_3 41

/***************************************************************
 * CONSTANTS
 ****************************************************************/
#define ON LOW
#define OFF HIGH

#define NUM_RGB_LEDS 3

#define NUM_COLORS 8

#define RED_I 0
#define GREEN_I 1 
#define BLUE_I 2
#define YELLOW_I 3
#define CYAN_I 4
#define MAGENTA_I 5
#define WHITE_I 6
#define BLACK_I 7


/***************************************************************
 * CONTROL FUNCTIONS
 ****************************************************************/

class LED_controller {
public:
  LED_controller();
  void set_color(int color_index);
  int get_current_color();

private:
  int LEDs[NUM_RGB_LEDS][3];
  bool COLORS[NUM_COLORS][3];
  int GND1, GND2, GND3;
  int current_color;
};

#endif

