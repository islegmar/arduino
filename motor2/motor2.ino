/**
   Servo
*/


int step_number = 0;
int pins[4] = {
  9, 10, 11, 12
};

int steps[4][4] = {
  {HIGH, LOW, LOW, LOW},
  {LOW, HIGH, LOW, LOW},
  {LOW, LOW, HIGH, LOW},
  {LOW, LOW, LOW, HIGH}
};
bool spin = true;

void setup() {
  Serial.begin(9600);
  Serial.println("LOW : " + String(LOW));
  Serial.println("HIGH : " + String(HIGH));
  for (unsigned int ind = 0; ind < 4; ind++) {
    pinMode(pins[ind], OUTPUT);
  }
}

void loop() {
  if (spin) {
    Serial.print("Turn clockwise, speed=5");
    for (unsigned int ind = 0; ind < 100; ++ind) {
      OneStep(true, 5);
    }
    Serial.print("Turn non-clockwise, speed=5");
    for (unsigned int ind = 0; ind < 100; ++ind) {
      OneStep(false, 5);
    }
    for (unsigned int ind = 0; ind < 10; ++ind) {
      OneStep(true, 15);
    }
    for (unsigned int ind = 0; ind < 10; ++ind) {
      OneStep(false, 15);
    }
    spin = true;
  }
}

void OneStep(bool clockwise, int speed) {
  int first_step = clockwise ? 3 : 0;
  int last_step = clockwise ? -1 : 4;
  int inc_step = clockwise ? -1 : 1;

  /*
  Serial.print("OneStep(clockwise=");
  Serial.print(clockwise);
  Serial.print(", speed=");
  Serial.print(speed);
  Serial.println(")");
  */
  
  for (unsigned int indStep = first_step; indStep != last_step; indStep += inc_step) {
    // Serial.print("[" + String(indStep) + "] : ");
    // Serial.print("(");
    for (unsigned int indPin = 0; indPin < 4; ++indPin) {
      digitalWrite(pins[indPin], steps[indStep][indPin]);
      // Serial.print(String(steps[indStep][indPin]) + ", ");
    }
    // Serial.println(")");
    delay(speed);
  }

}
