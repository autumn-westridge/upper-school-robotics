#include <Servo.h>

// Servo variables
const int NUM_SERVOS = 2;
// Declare your servos
Servo s0;
Servo s1;
// Put the memory address of the servos into an array
Servo *servos[NUM_SERVOS] = {&s0, &s1};

// Position/movement arrays for each servo
int servo_pins[NUM_SERVOS] = {2, 3};
int curr_pos[NUM_SERVOS] = {0, 0};   // The current position of the motor
int goal_pos[NUM_SERVOS] = {180, 180};   // The goal we're working towards
int min_pos[NUM_SERVOS] = {0, 0};    // The minimum position we'll allow the motor to move to
int max_pos[NUM_SERVOS] = {180, 180};  // The maximum position

// Timing variables
long servo_clock;
long pause_ms = 20;

void setup() {
  Serial.begin(115200);
  
  //Attach the servos from the array
  for (int i = 0; i < NUM_SERVOS; i++) {
    servos[i]->attach(servo_pins[i]);
    servos[i]->write(curr_pos[i]);
  }

  servo_clock = millis();
}

void loop() {
  writeMotorPosition();
}

void writeMotorPosition() {
  // This should be familiar by now...
  if (millis() - servo_clock >= pause_ms) {
    for (int i = 0; i < NUM_SERVOS; i++) {
      // Move towards our goal
      if (curr_pos[i] < goal_pos[i]) {
        curr_pos[i]++;
      }
      else if (curr_pos[i] > goal_pos[i]) {
        curr_pos[i]--;
      }
      
      if (curr_pos == goal_pos) {
        if (goal_pos == 180) {
          goal_pos = 0;
        }
        else {
          goal_pos = 180;
        }
      }
      
      servos[i]->write(curr_pos[i]);
      servo_clock = millis();
    }
  }
}
