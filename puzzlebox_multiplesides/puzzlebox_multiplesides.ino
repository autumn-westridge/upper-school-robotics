#include <Wire.h>
#include <Adafruit_MPR121.h>
#include <Adafruit_NeoPixel.h>

// Set to 1 for serial monitor printout
#define DEBUG           0

/**** 
 ****  
 **** CHANGE ME
 ****
 ****/
// LED details
#define LED_PIN             6
#define LED_COUNT           10
#define MAX_SEQUENCE_LENGTH 5

int AMBIENT_LED_COUNT = 4;
int RESPONSE_LED_COUNT = 6;
int response_leds[6] = {0, 3, 4, 5, 8, 9};

// Capacitive sensor details
int NUM_SENSORS = 12;
int SEQUENCE_LENGTH = 3;

int target_order[MAX_SEQUENCE_LENGTH] = {0, 11, 11, -1, -1};
int touch_order[MAX_SEQUENCE_LENGTH] = {-1, -1, -1, -1, -1};
/**** 
 ****  
 **** END
 ****
 ****/

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

const int retrig_ms = 100;
long clk[12];

Adafruit_MPR121 cap = Adafruit_MPR121();
uint16_t lasttouched  = 0;
uint16_t currtouched  = 0;
int touch_count = 0;


void setup() {
  Serial.begin(115200);

  if (!cap.begin(0x5A)) {
    Serial.println("MPR121 not found, check wiring?");
    while (1);
  }
  Serial.println("MPR121 found!");

  for (int i = 0; i < NUM_SENSORS; i++) {
    clk[i] = millis();
  }

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)

  setDefaultLEDs();
}

void loop() {
  checkCap();
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
          checkTouchOrder();
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

void checkTouchOrder() {
  // Check if the most recent touch matches the target.
  // Have to subtract 1 because we already added to touch_count in checkCap
  // This if statement is true if the user made an error
  if (target_order[touch_count - 1] != touch_order[touch_count - 1]) {
    // Display an error visually via the response LEDs
    ledError();
    // Restart the sequence
    restartSequence();
  }
  // If this wasn't an error, it was correct
  else {
    // If this isn't the end of the sequence, show an encouraging message
    if (touch_count < SEQUENCE_LENGTH) {
      ledCorrect();
    }
    // If this is the end of the sequence, do something special
    else {
      rainbow();
      // move on to the next side
      setDefaultLEDs();
      advanceSide();
      restartSequence();
    }
  }
}

void restartSequence() {
  // Restart at the beginning of the sequence
  touch_count = 0;
  for (int i = 0; i < SEQUENCE_LENGTH; i++) {
    touch_order[i] = -1;
  }
}

/****
 ****
 **** BEGIN DEBUG PRINTOUT CODE
 ****
 ****/
void printTouchOrder() {
  if (DEBUG == 1) {
    Serial.print("[");
    for (int i = 0; i < SEQUENCE_LENGTH - 1; i++) {
      Serial.print(touch_order[i]);
      Serial.print(", ");
    }
    Serial.print(touch_order[SEQUENCE_LENGTH - 1]);
    Serial.println("]");
  }
}