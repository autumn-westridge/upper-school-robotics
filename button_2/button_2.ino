int button_pin = 3;
int last_button_val;
int curr_button_val;

long clk;
int retrig_ms = 200;

void setup() {
  Serial.begin(115200);               // Initialize Serial
  pinMode(button_pin, INPUT_PULLUP);  // Use the pullup resistor

  // Initialize both values to the starting oin value
  last_button_val = curr_button_val = digitalRead(button_pin);

  clk = millis();
}

void loop() {
  curr_button_val = digitalRead(button_pin);  // Get the new value
  /*
   * IF: our current value is not the same as the last value
   * AND it has been more than retrig_ms milliseconds
   * Then consider this a valid button press.
   */
  if (curr_button_val != last_button_val && millis() - clk >= retrig_ms) {
    // Report the change
    if (curr_button_val == HIGH) {
      Serial.println("Button pressed");
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
