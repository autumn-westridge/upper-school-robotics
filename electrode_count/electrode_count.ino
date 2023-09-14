#include <Wire.h>
#include <Adafruit_MPR121.h>

#define NUM_SENSORS 12

int LED_PIN = 13;
const int retrig_ms = 100;
long clk[NUM_SENSORS];

Adafruit_MPR121 cap = Adafruit_MPR121();
uint16_t lasttouched  = 0;
uint16_t currtouched  = 0;

int target_order[3] = {0, 11, 11};
int touch_order[3] = {-1, -1, -1};
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
  for (int i = 0; i < 3; i++) {
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
  if (touch_count % 3 == 0) {
    for (int i = 0; i < 3; i++) {
      touch_order[i] = -1;
    }
  }
}

void checkCap() {
  currtouched = cap.touched();
  
  for (int i = 0; i < NUM_SENSORS; i++) {
    if ((currtouched & _BV(i)) && !(lasttouched & _BV(i))) {
      Serial.print(i); Serial.println(" touched");
      clk[i] = millis();

    }
    if (!(currtouched & _BV(i)) && (lasttouched & _BV(i)) ) {
      Serial.print(i); Serial.println(" released");
      if ((millis() - clk[i]) >= retrig_ms) {
        touch_order[touch_count % 3] = i;
        printTouchOrder();
        touch_count++;
      }
      clk[i] = millis();
    }
  }
  
  lasttouched = currtouched;
}

void printTouchOrder() {
  Serial.print("[");
  Serial.print(touch_order[0]);
  Serial.print(", ");
  Serial.print(touch_order[1]);
  Serial.print(", ");
  Serial.print(touch_order[2]);
  Serial.println("]");
}


