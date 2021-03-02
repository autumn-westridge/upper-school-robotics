int button_pin = 3;

void setup() {
  Serial.begin(115200);               // Initialize Serial
  pinMode(button_pin, INPUT_PULLUP);  // Use the pullup resistor
}

void loop() {
  int i = digitalRead(button_pin);    // Read the button value
  Serial.println(i);                  // Print it out
  delay(20);                          // The Bad Delay
}
