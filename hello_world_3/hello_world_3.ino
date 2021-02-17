int delay_ms = 2000;

void setup() {
  Serial.begin(115200);
}

void loop() {
  for (int i = 0; i < 10; i++) {
    Serial.print((String) i + " ");
    
    if (i == 9) {
      Serial.print("Goodbye");
    }
  }
  Serial.println();
  
  delay(delay_ms);
}
