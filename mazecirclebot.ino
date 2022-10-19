#include <AFMotor.h>

// IR pins
#define leftMostIR A4
#define leftIR A3
#define middleIR A2
#define rightIR A1
#define rightMostIR A0

#define ON_WHITE 0
#define ON_BLACK 1

#define SPEED 100

AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(2, MOTOR12_1KHZ);

void curve_left() {
  motor1.setSpeed(SPEED);
  motor2.setSpeed(SPEED);
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
}

void curve_right() {
  motor1.setSpeed(SPEED);
  motor2.setSpeed(SPEED);
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
}

void go_straight() {
  motor1.setSpeed(SPEED);
  motor2.setSpeed(SPEED);
  motor1.run(FORWARD);
  motor2.run(FORWARD);
}

#define GO_FORWARD_ADJUST_TIME 300
#define TURN_LEFT 1
#define TURN_RIGHT 2
#define FORWARD_ADJUST true
#define NO_FORWARD_ADJUST false

void turn(int left_or_right, bool forward_adjust) {
  if (forward_adjust) {
    unsigned long startMillis = millis();
    while (millis() - startMillis < GO_FORWARD_ADJUST_TIME) {
      go_straight();
    }
  }
  stop_moving();
  delay(200);
  while (digitalRead(middleIR) == ON_WHITE) {
    if (left_or_right == TURN_LEFT) {
      curve_left();
    } else {
      curve_right();
    }
  }
}

void turnaround() {
  turn(TURN_LEFT, NO_FORWARD_ADJUST);
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

/* #define IR(lm, l, m, r, rm) leftMost == lm &&\ */
/*                             left == l && \ */
/*                             middle == m && \ */
/*                             right == r && \ */
/*                             rightMost == rm */
/* #define W ON_WHITE */
/* #define B ON_BLACK */

void loop() {
  int leftMost = digitalRead(leftMostIR);
  int left = digitalRead(leftIR);
  int middle = digitalRead(middleIR);
  int right = digitalRead(rightIR);
  int rightMost = digitalRead(rightMostIR);

  if (leftMost == ON_WHITE && // straight line (1)
      left == ON_WHITE &&
      middle == ON_BLACK &&
      right == ON_WHITE &&
      rightMost == ON_WHITE) {
    go_straight();
  } else if ( // left L or left T (3, 5)
      leftMost == ON_BLACK &&
      left == ON_BLACK &&
      middle == ON_BLACK &&
      right == ON_WHITE &&
      rightMost == ON_WHITE) {
    turn(TURN_LEFT, FORWARD_ADJUST);
  } else if ( // T or 4 way intersection (4, 8)
      leftMost == ON_BLACK &&
      left == ON_BLACK &&
      middle == ON_BLACK &&
      right == ON_BLACK &&
      rightMost == ON_BLACK) {
    turn(TURN_LEFT, FORWARD_ADJUST);
  } else if ( // left curve (9)
      leftMost == ON_WHITE &&
      left == ON_BLACK &&
      middle == ON_BLACK &&
      right == ON_WHITE &&
      rightMost == ON_WHITE) {
    turn(TURN_LEFT, NO_FORWARD_ADJUST);
  } else if ( // left curve (9)
      leftMost == ON_WHITE &&
      left == ON_BLACK &&
      middle == ON_WHITE &&
      right == ON_WHITE &&
      rightMost == ON_WHITE) {
    turn(TURN_LEFT, NO_FORWARD_ADJUST);
  } else if ( // right curve (10)
      leftMost == ON_WHITE &&
      left == ON_WHITE &&
      middle == ON_BLACK &&
      right == ON_BLACK &&
      rightMost == ON_WHITE) {
    turn(TURN_RIGHT, NO_FORWARD_ADJUST);
  } else if ( // right curve (10)
      leftMost == ON_WHITE &&
      left == ON_WHITE &&
      middle == ON_WHITE &&
      right == ON_BLACK &&
      rightMost == ON_WHITE) {
    turn(TURN_RIGHT, NO_FORWARD_ADJUST);
  } else if ( // deadend (7)
      leftMost == ON_WHITE &&
      left == ON_WHITE &&
      middle == ON_WHITE &&
      right == ON_WHITE &&
      rightMost == ON_WHITE) {
    turnaround();
  }
}


