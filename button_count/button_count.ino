const int button_pin = 2;
const int retrig_ms = 100;

int last_button_val;
int curr_button_val;

long clk;
int times_pressed = 0;

void setup() {
  Serial.begin(9600);                 // Initialize Serial -- let us see output
  pinMode(button_pin, INPUT_PULLUP);  // Use the pullup resistor

  // Initialize both values
  last_button_val = curr_button_val = digitalRead(button_pin);
  // Initialize the clock
  clk = millis();
}

void loop() {
  curr_button_val = digitalRead(button_pin);  // Get the new value

  /*
   * IF: our current value is not the same as the last value
   * AND it has been more than retrig_ms milliseconds
   * Then this is a valid button press.
   */
  if (curr_button_val != last_button_val && (millis() - clk) >= retrig_ms) {
    // If the value is LOW, the button has been pressed.
    if (curr_button_val == LOW) {
      times_pressed++;
      Serial.println(times_pressed);
    }
    
    // Reset the clock
    clk = millis();
    
  }
  
  // The current value becomes last_button_val for the next time through the loop
  last_button_val = curr_button_val;
}

















