#include <Servo.h>  // Servo class library

// Motor variables
Servo s;
int servo_pin = 2;      // Which pin is the servo on?
int min_pos = 0;       // The minimum position we'll be moving to within the motor's ROM
int max_pos = 180;      // The maximum position
int curr_pos;           // The motor's current position
int longpause_pos = 90; // Where to do a long pause
bool dir = true;        // True = increasing, false = decreasing

// Timing variables
long clk;                 // Clock
int pause_ms = 20;        // How long to pause -- 20ms
int shortpause_ms = 20;   // Short pause
int longpause_ms = 2000;  // Long pause

void setup() {
  // Set up the motor
  s.attach(servo_pin);

  // Start the current position one greater than the minimum
  curr_pos = min_pos + 1;
  
  // Start the clock
  clk = millis();
}

void loop() {
  // Is it time to take a step?
  if (millis() - clk >= pause_ms) {
    // If we've reached one of the extremes, reverse direction
    if (curr_pos == max_pos || curr_pos == min_pos) {
      dir = !dir; // Negates a boolean. If it's true, becomes false; if false, becomes true
    }

    // Increment or decrement curr_pos
    if (dir) {
      curr_pos++;
    }
    else {
      curr_pos--;
    }

    // Move the motor
    s.write(curr_pos);

    // Check if we're doing a long pause here
    updatePause();
    
    clk = millis(); // Reset the clock
  }
}

/*  This function can be updated to control the speed of your motor however you want.
 *  For example, it could go fast when dir is true but slow when it's false,
 *  faster when it's closer to 0 and slower when it's closer to 180, and so on.
 */
void updatePause() {
  // If this is where we're pausing, change the pause_ms to the longpause amount
  if (curr_pos == longpause_pos) {
    pause_ms = longpause_ms;
  }
  // Else, a short pause
  else {
    pause_ms = shortpause_ms;
  }
}
