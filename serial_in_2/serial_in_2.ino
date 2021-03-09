#include <Servo.h>

// To read from serial:
const byte MAX_CHARS = 3;       // Only read up to three characters
char in_buffer[MAX_CHARS + 1];  // +1 to allow for null terminator

// Servo variables
Servo s;
int servo_pin = 2;
int curr_pos = 0;

void setup() {
  Serial.begin(115200);
  
  //Attach the servo on pin 2, as usual
  s.attach(servo_pin);
  s.write(curr_pos);
}

void loop() {
  readFromSerial();
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
      writeMotorPosition();
      ndx = 0;
    }
  }
}

void writeMotorPosition() {
  curr_pos = atoi(in_buffer);   // Convert the input buffer into an nit
  Serial.println(curr_pos);     // Print it out
  s.write(curr_pos);            // Write it to the motor
}
