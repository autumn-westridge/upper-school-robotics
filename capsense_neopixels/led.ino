/****
 ****
 **** BEGIN LED HELPER CODE
 ****
 ****/

void setDefaultLEDs() {
  /**** 
  ****  
  **** CHANGE ME
  ****
  ****/
  uint32_t ambient_led_color = strip.Color(0, 180, 180); // nice blue-green
  uint32_t response_led_color = strip.Color(0, 0, 0); // by default, response LEDs are off
  /**** 
  ****  
  **** END
  ****
  ****/
  
  // By default, set ambient LEDs to ambient_led_color
  for (int i = 0; i < AMBIENT_LED_COUNT; i++) {
    strip.setPixelColor(ambient_leds[i], ambient_led_color);
  }
  // By default, set response LEDs to response_led_color
  for(int i=0; i < RESPONSE_LED_COUNT; i++) {
    strip.setPixelColor(response_leds[i], response_led_color);
  }
  strip.show();
}

/**** 
 ****  
 **** OPTIONAL: CHANGE ME
 ****
 ****/
void ledError() {
  // blink red twice
  respond(strip.Color(255, 0, 0)); // turn the response LEDs red
  delay (200);
  respond(strip.Color(0, 0, 0)); // turn the response LEDs off
  delay (150);
  respond(strip.Color(255, 0, 0));
  delay (200);
  respond(strip.Color(0, 0, 0));
}

void ledCorrect() {
  respond(strip.Color(0, 255, 0)); // turn the response LEDs green
  delay(400);
  respond(strip.Color(0, 0, 0)); // turn off
}
/**** 
 ****  
 **** OPTIONAL: END
 ****
 ****/

// applies only to response LEDs
void respond(uint32_t color) {
  for(int i=0; i < RESPONSE_LED_COUNT; i++) { // For each pixel in strip...
    strip.setPixelColor(response_leds[i], color);         //  Set pixel's color (in RAM)
  }
  strip.show();                          //  Update strip to match
}

// applies to entire strip
void rainbow() {
  for(long firstPixelHue = 0; firstPixelHue < 65536; firstPixelHue += 256) {
    strip.rainbow(firstPixelHue);
    strip.show(); // Update strip with new contents
    delay(10);  // Pause for a moment
  }
}