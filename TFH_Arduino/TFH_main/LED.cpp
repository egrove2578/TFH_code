/*
 18745 RPCS, S'13
 TFH Group
 
 Implementation file for the LED color control
 */


#include "LED.h"

/********************************************************************************
 constructor
   -- Initialize LED pins and ground pins, and set them as outputs
   -- Initialize color array and configurations for each color
   -- Turn all colors off and set initial current_color to BLACK
********************************************************************************/
LED_controller::LED_controller() {
  
   COLORS[0][0] = ON;    COLORS[0][1] = OFF;  COLORS[0][2] = OFF; // {ON, OFF, OFF}; /// RED
   COLORS[1][0] = OFF;   COLORS[1][1] = ON;   COLORS[1][2] = OFF; // {OFF, ON, OFF}; /// GREEN
   COLORS[2][0] = OFF;   COLORS[2][1] = OFF;  COLORS[2][2] = ON;  // {OFF, OFF, ON}; /// BLUE
   COLORS[3][0] = ON;    COLORS[3][1] = ON;   COLORS[3][2] = OFF; // {ON, ON, OFF}; /// YELLOW
   COLORS[4][0] = OFF;   COLORS[4][1] = ON;   COLORS[4][2] = ON;  // {OFF, ON, ON}; /// CYAN
   COLORS[5][0] = ON;    COLORS[5][1] = OFF;  COLORS[5][2] = ON;  // {ON, OFF, ON}; /// MAGENTA
   COLORS[6][0] = ON;    COLORS[6][1] = ON;   COLORS[6][2] = ON;  // {ON, ON, ON}; /// WHITE
   COLORS[7][0] = OFF;   COLORS[7][1] = OFF;  COLORS[7][2] = OFF; // {OFF, OFF, OFF}; /// BLACK

 
   LEDs[0][0] = RED1;
   LEDs[1][0] = RED2;
   LEDs[2][0] = RED3;
   
   LEDs[0][1] = GREEN1;
   LEDs[1][1] = GREEN2;
   LEDs[2][1] = GREEN3;
   
   LEDs[0][2] = BLUE1;
   LEDs[1][2] = BLUE2;
   LEDs[2][2] = BLUE3;
  
   GND1 = GROUND_1;
   GND2 = GROUND_2;
   GND3 = GROUND_3;
   
   current_color = BLACK_I;
   
   pinMode(RED1, OUTPUT);
   pinMode(RED2, OUTPUT);
   pinMode(RED3, OUTPUT);
   pinMode(GREEN1, OUTPUT);
   pinMode(GREEN2, OUTPUT);
   pinMode(GREEN3, OUTPUT);
   pinMode(BLUE1, OUTPUT);
   pinMode(BLUE2, OUTPUT);
   pinMode(BLUE3, OUTPUT);
   
   pinMode(GND1, OUTPUT);
   pinMode(GND2, OUTPUT);
   pinMode(GND3, OUTPUT);
   
   digitalWrite(RED1, OFF);
   digitalWrite(RED2, OFF);
   digitalWrite(RED3, OFF);
   digitalWrite(GREEN1, OFF);
   digitalWrite(GREEN2, OFF);
   digitalWrite(GREEN3, OFF);
   digitalWrite(BLUE1, OFF);   
   digitalWrite(BLUE2, OFF);   
   digitalWrite(BLUE3, OFF);
   
   digitalWrite(GND1, HIGH);
   digitalWrite(GND2, HIGH);
   digitalWrite(GND3, HIGH);

}


/***************************************************************** 
  int get_current_color - returns the color currently showing
*****************************************************************/
int LED_controller::get_current_color() {
  return current_color;
}


/***************************************************************** 
  void set_color - takes a color index and sets all RGB LEDs in 
  the group to that color, then sets current_color to that index
*****************************************************************/
void LED_controller::set_color(int color_index) { 
  
   bool* colors = COLORS[color_index];
   
   int* led;
   
   for(int i = 0; i < NUM_RGB_LEDS; i++) { 
     led = LEDs[i]; 
     for (int j = 0; j < 3; j++) {     
       digitalWrite(led[j], colors[j]); 
     }
   }
   
   current_color = color_index;
}

