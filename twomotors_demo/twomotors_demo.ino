#include <Stepper.h>
#include <Servo.h>

const int stepsPerRevolution = 200;                       // This variable doesn't really matter for this dumb sketch
Stepper stepper_motor(stepsPerRevolution, 8, 9, 10, 11);  // Initialize the stepper library on pins 8 through 11
int stepper_delay = 5;                                    // How long do we wait until we take the next step?
long stepper_clock;                                       // This implements the delay

Servo servo_motor;      // Initialize the Servo library -- will attach to pin 7
int servo_pos = 0;      // Initial position
int servo_delay = 20;   // Servo delay
bool servo_dir = true;  // Increasing or decreasing
long servo_clock;       // This implements the delay

// Setup -- this code runs once
void setup() {
  Serial.begin(9600);
  servo_motor.attach(7);      // Attach the servo
  
  stepper_clock = millis();   // Grab the current time
  servo_clock = millis();
}

void loop() {
  long ms = millis();

  moveStepper(ms);
  
  moveServo(ms);
}

void moveStepper(long ms) {
  // If enough time has passed to increment either motor...
  if (ms - stepper_clock > stepper_delay) {
    stepper_motor.step(1);  // Take a step
    stepper_clock = ms;     // Reset the clock
  }
}

void moveServo(long ms) {
  if (ms - servo_clock > servo_delay) {
    // Either increment or decrement depending on our direction
    if (servo_dir) {
      servo_pos++;
    }
    else {
      servo_pos--;
    }
    Serial.println(servo_pos);
    
    // If we've reached the edges of the servo's range of motion...
    if (servo_pos >= 179 || servo_pos <= 0) {
      servo_dir = !servo_dir; // Reset 
    }
    
    servo_motor.write(servo_pos);
    servo_clock = ms;
  }
}
