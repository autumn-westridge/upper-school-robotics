#include <Servo.h>

// Button variables
int button_pin = 3;
int last_button_val;
int curr_button_val;
bool button_go = false;

// Servo variables
Servo s;
int servo_pin = 2;
int min_pos = 0;    // The minimum position we'll be moving to within the motor's ROM
int max_pos = 180;  // The maximum position
int curr_pos;       // The motor's current position
bool dir = true;    // True = increasing, false = decreasing

// Timing variables
long button_clock;
long servo_clock;
int retrig_ms = 200;
int pause_ms = 20;

void setup() {
  Serial.begin(115200);               // Initialize Serial
  pinMode(button_pin, INPUT_PULLUP);  // Use the pullup resistor
  last_button_val = curr_button_val = digitalRead(button_pin); // Initialize both values to the starting oin value

  //Attach the servo on pin 2
  s.attach(servo_pin);
  curr_pos = min_pos + 1;

  button_clock = servo_clock = millis();
}

void loop() {
  checkButton();
  updateServo();
}

void checkButton() {  
  curr_button_val = digitalRead(button_pin);  // Get the new value
  /*
   * IF: our current value is not the same as the last value
   * AND it has been more than retrig_ms milliseconds
   * Then consider this a valid button press.
   */
  if (curr_button_val != last_button_val && millis() - button_clock >= retrig_ms) {
    // Report the change
    if (curr_button_val == HIGH) {
      Serial.println("Button pressed");
      button_go = !button_go;
    }
    else {
      Serial.println("Button released");
    }
    
    // Reset the clock
    clk = millis();
    /*
     * We COULD put this outside of the if statement, but that
     * might cause us to fail to report that the button had been released
     * if the button press is very fast. Why?
     */
    last_button_val = curr_button_val;
  }
}

void updateServo() {
  // Is it time to take a step?
  if (button_go && millis() - servo_clock >= pause_ms) {
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
    servo_clock = millis(); // Reset the clock
  }  
}
