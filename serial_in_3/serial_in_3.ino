#include <Servo.h>

// To read from serial:
const byte MAX_CHARS = 3;       // Only read up to three characters
char in_buffer[MAX_CHARS + 1];  // +1 to allow for null terminator
bool new_data = false;          // Set to true if we have new data

// Servo variables
Servo s;
int servo_pin = 2;
int curr_pos = 0;   // The current position of the motor
int goal_pos = 0;   // The goal we're working towards
int min_pos = 0;    // The minimum position we'll allow the motor to move to
int max_pos = 180;  // The maximum position

// Timing variables
long servo_clock;
long pause_ms = 20;

void setup() {
  Serial.begin(115200);
  
  //Attach the servo on pin 2, as usual
  s.attach(servo_pin);
  s.write(curr_pos);

  servo_clock = millis();
}

void loop() {
  readFromSerial();
  writeMotorPosition();
}

void readFromSerial() {
  static int ndx = 0;             // The index of the input buffer to place the next character at
  
  while (Serial.available()) {
    // Read each character sequentially. If it isn't a newline, put it in the buffer
    char c = Serial.read();
    if (c != '\n') {
      in_buffer[ndx] = c;
      /*  If we start to overflow the buffer, just stop incrementing the index.
       *  This way we'll overwrite the last character of the buffer over and over
       *  with the extraneous characters, and then finally overwrite it with \0
       */
      if (ndx < MAX_CHARS) {
        ndx++;
      }
    }
    else {
      in_buffer[ndx] = '\0';  // This is a "null terminator" that indicates the end of a string
      int new_pos = atoi(in_buffer);
      // Error handling
      if (new_pos <= max_pos && new_pos >= min_pos) {
        goal_pos = new_pos;
        Serial.println(goal_pos);
      }
      ndx = 0;
    }
  }
}

void writeMotorPosition() {
  // This should be familiar by now...
  if (millis() - servo_clock >= pause_ms) {
    // Move towards our goal
    if (curr_pos < goal_pos) {
      curr_pos++;
    }
    else if (curr_pos > goal_pos) {
      curr_pos--;
    }
    s.write(curr_pos);
    servo_clock = millis();
  }
}
