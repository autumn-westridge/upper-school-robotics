#include <Wire.h>
#include <Adafruit_MPR121.h>

#define NUM_SENSORS     12
#define SEQUENCE_LENGTH 3

int LED_PIN = 13;
const int retrig_ms = 100;
long clk[NUM_SENSORS];

Adafruit_MPR121 cap = Adafruit_MPR121();
uint16_t lasttouched  = 0;
uint16_t currtouched  = 0;

int target_order[SEQUENCE_LENGTH] = {0, 11, 11};
int touch_order[SEQUENCE_LENGTH] = {-1, -1, -1};
int touch_count = 0;

void setup() {
  Serial.begin(115200);

  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 found!");

  pinMode(LED_PIN, OUTPUT);
  for (int i = 0; i < NUM_SENSORS; i++) {
    clk[i] = millis();
  }
}

void loop() {
  checkCap();

  // Check if the user's touch order matches the target order
  bool match = true;
  for (int i = 0; i < SEQUENCE_LENGTH; i++) {
    if (target_order[i] != touch_order[i]) {
      match = false;
    }
  }
  if (match) {
    digitalWrite(LED_PIN, HIGH);
    delay(2000); // stop everything with the LED on
  }
  else {
    digitalWrite(LED_PIN, LOW);
  }
 
  // If the user has touched three sensors, reset
  if (touch_count % SEQUENCE_LENGTH == 0) {
    for (int i = 0; i < SEQUENCE_LENGTH; i++) {
      touch_order[i] = -1;
    }
  }
}

void checkCap() {
  currtouched = cap.touched();
 
  for (int i = 0; i < NUM_SENSORS; i++) {
    if ((millis() - clk[i]) >= retrig_ms) {
      if ((currtouched & _BV(i)) && !(lasttouched & _BV(i))) {
          Serial.print(i); Serial.println(" touched");
          touch_order[touch_count % SEQUENCE_LENGTH] = i;
          printTouchOrder();
          touch_count++;
          clk[i] = millis();
      }
      if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
          Serial.print(i); Serial.println(" released");
          clk[i] = millis();
      }
    }
  }
 
  lasttouched = currtouched;
}

void printTouchOrder() {
  Serial.print("[");
  for (int i = 0; i < SEQUENCE_LENGTH - 1; i++) {
    Serial.print(touch_order[i]);
    Serial.print(", ");
  }
  Serial.print(touch_order[SEQUENCE_LENGTH - 1]);
  Serial.println("]");
}
