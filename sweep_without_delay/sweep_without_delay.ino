#include <Servo.h>  // Servo class library

// Motor variables
Servo s;
int servo_pin = 2;  // Which pin is the servo on?
int min_pos = 0;    // The minimum position we'll be moving to within the motor's ROM
int max_pos = 180;  // The maximum position
int curr_pos = 90;  // The motor's current position
bool dir = true;    // True = increasing, false = decreasing

// Timing variables
long clk;           // Clock
int pause_ms = 20;  // How long to pause -- 20ms

void setup() {
  // Set up the motor
  s.attach(servo_pin);
  
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
    clk = millis(); // Reset the clock
  }
}
