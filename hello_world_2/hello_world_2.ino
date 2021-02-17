int delay_ms = 500;
int counter = 0;

void setup() {
  Serial.begin(115200);
  Serial.println((String) "Hello World! Delay time: " + delay_ms + "; counter starts at " + counter);
}

void loop() {
  Serial.println((String) "Hello " + counter);
  counter++;

  delay(delay_ms);
}
