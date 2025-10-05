// plotterberry.ino - minimal serial command parser
#include <Arduino.h>

// contoh stepper control (ganti dengan driver nyata)
const int stepPinX = 2;
const int dirPinX = 3;
const int stepPinY = 4;
const int dirPinY = 5;

long curX = 0, curY = 0;
float steps_per_unit = 10.0; // contoh: 10 steps = 1 unit

void setup() {
  Serial.begin(115200);
  pinMode(stepPinX, OUTPUT);
  pinMode(dirPinX, OUTPUT);
  pinMode(stepPinY, OUTPUT);
  pinMode(dirPinY, OUTPUT);
  Serial.println("READY");
}

void stepMotor(int stepPin, int dirPin, long steps, bool dir) {
  digitalWrite(dirPin, dir ? HIGH : LOW);
  for (long i = 0; i < steps; i++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(800);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(800);
  }
}

void moveTo(float x, float y) {
  long tx = lround(x * steps_per_unit);
  long ty = lround(y * steps_per_unit);

  long dx = tx - curX;
  long dy = ty - curY;

  // simple: move X then Y (not optimized / no Bresenham)
  if (dx != 0) stepMotor(stepPinX, dirPinX, abs(dx), dx > 0);
  if (dy != 0) stepMotor(stepPinY, dirPinY, abs(dy), dy > 0);

  curX = tx; curY = ty;
}

void loop() {
  if (Serial.available() > 0) {
    String line = Serial.readStringUntil('\n');
    line.trim();
    if (line.length() == 0) return;

    if (line.startsWith("MOVE")) {
      float x=0,y=0;
      sscanf(line.c_str(), "MOVE %f %f", &x, &y);
      moveTo(x,y);
      Serial.println("ACK");
    } else if (line == "PENUP") {
      // implement servo up
      Serial.println("ACK");
    } else if (line == "PENDOWN") {
      // implement servo down
      Serial.println("ACK");
    } else if (line.startsWith("SPEED")) {
      // parse speed
      Serial.println("ACK");
    } else {
      Serial.println("ERR");
    }
  }
}
