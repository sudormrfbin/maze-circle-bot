#include <AFMotor.h>

// IR pins
#define leftMostIR A4
#define leftIR A3
#define middleIR A2
#define rightIR A1
#define rightMostIR A0

#define ON_WHITE 0
#define ON_BLACK 1

AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(2, MOTOR12_1KHZ);

void turn_left() {
  motor1.setSpeed(100);
  motor2.setSpeed(100);
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
}

void turn_right() {
  motor1.setSpeed(100);
  motor2.setSpeed(100);
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
}

void go_straight() {
  motor1.setSpeed(100);
  motor2.setSpeed(100);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
}

void stop_moving() {
  motor1.setSpeed(0);
  motor2.setSpeed(0);
  motor1.run(RELEASE);
  motor2.run(RELEASE);
}

void setup() {
  pinMode(leftMostIR, INPUT);
  pinMode(leftIR, INPUT);
  pinMode(middleIR, INPUT);
  pinMode(rightIR, INPUT);
  pinMode(rightMostIR, INPUT);

  delay(1000);  // 1s delay to do positioning adjustments
}

void loop() {
  int left = digitalRead(leftIR);
  int right = digitalRead(rightIR);
  if (left == ON_WHITE && right == ON_WHITE) {
    go_straight();
  } else if (left == ON_WHITE && right == ON_BLACK) {
    turn_left();
  } else if (left == ON_BLACK && right == ON_WHITE) {
    turn_right();
  } else if (left == ON_BLACK && right == ON_BLACK) {
    stop_moving();
  }
}


