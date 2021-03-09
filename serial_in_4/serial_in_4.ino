#include <Servo.h>

// To read from serial:
const byte MAX_CHARS = 6;       // Read more chars to allow for longer pauses
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
int pause_ms = 20;
int min_pause_ms = 5;
int max_pause_ms = 1000;

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
      ndx = 0;
      
      // A string beginning with a 'p' sets the pause duration
      if (in_buffer[0] == 'p') {
        int new_pause_ms = atoi(in_buffer + 2); // Pointer arithmetic!
        if (new_pause_ms <= max_pause_ms && new_pause_ms >= min_pause_ms) {
          pause_ms = new_pause_ms;
          Serial.println((String) "Now pausing for " + pause_ms + " milliseconds");
        }
      }
      
      // Otherwise treat it the same as before
      else {
        int new_pos = atoi(in_buffer);
        // Error handling
        if (new_pos <= max_pos && new_pos >= min_pos) {
          goal_pos = new_pos;
          Serial.println((String) "New servo goal: " + goal_pos);
        }
      }
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
