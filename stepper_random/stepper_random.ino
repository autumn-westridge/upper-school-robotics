#include <Stepper.h>

#define STEPS 2038

Stepper stepper(STEPS, 8, 9, 10, 11);
int steps_taken = 0;
int max_steps_total = STEPS * 3;

void setup() {
  randomSeed(analogRead(A0));
  stepper.setSpeed(6);
  stepper_clock = millis();
  Serial.begin(115200);
}

void loop() {
  int min_steps = 0 - steps_taken; // We can go back as far as we've gone forward
  int max_steps = max_steps_total - steps_taken; // We can go the rest of the way forward
  int steps_to_take = random(min_steps, STEPS);
  takeSteps(steps_to_take);
  steps_taken += steps_to_take;
  Serial.println(steps_taken);
}

void takeSteps(int steps_to_take) {
  int increment = steps_to_take < 0 ? -1 : 1;
  for (int i = 0; i < abs(steps_to_take); i++) {
    stepper.step(increment);
  }
}
