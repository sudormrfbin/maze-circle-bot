#include <AFMotor.h>

// IR pins
#define leftMostIR A4
#define leftIR A3
#define middleIR A2
#define rightIR A1
#define rightMostIR A0

#define ON_WHITE 0
#define ON_BLACK 1

#define SPEED 90

AF_DCMotor motor1(1, MOTOR12_1KHZ);
AF_DCMotor motor2(2, MOTOR12_1KHZ);

void curve_right() {
  motor1.setSpeed(SPEED);
  motor2.setSpeed(SPEED);
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
}

void curve_left() {
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
  /* motor1.setSpeed(80); */
  /* motor2.setSpeed(80); */

  if (forward_adjust) {
    unsigned long startMillis = millis();
    while (millis() - startMillis < GO_FORWARD_ADJUST_TIME) {
      go_straight();
    }
  }
  while (digitalRead(middleIR) == ON_WHITE) {
    if (left_or_right == TURN_LEFT) {
      curve_left();
    } else {
      curve_right();
    }
  }

  /* motor1.setSpeed(SPEED); */
  /* motor2.setSpeed(SPEED); */
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

bool finished_maze = false;

void loop() {
  if (finished_maze) {
    return;
  }

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
    turn(TURN_LEFT, NO_FORWARD_ADJUST);
  } else if ( // T or 4 way intersection (4, 8)
      leftMost == ON_BLACK &&
      left == ON_BLACK &&
      middle == ON_BLACK &&
      right == ON_BLACK &&
      rightMost == ON_BLACK) {
    unsigned long startMillis = millis();
    while (millis() - startMillis < 150) {
      go_straight();
    }
    // Go forward a bit and check if there is black line. If there is
    // we have reached the end of the maze (big black box) so stop. If
    // not this was a T or 4 way junction, so turn left.
    if (digitalRead(middleIR) == ON_BLACK) {
      stop_moving();
      finished_maze = true;
    } else {
      turn(TURN_LEFT, NO_FORWARD_ADJUST);
    }
  } else if ( // right L or right T (2, 6)
      leftMost == ON_WHITE &&
      left == ON_WHITE &&
      middle == ON_BLACK &&
      right == ON_BLACK &&
      rightMost == ON_BLACK) {
    unsigned long startMillis = millis();
    while (millis() - startMillis < 200) {
      go_straight();
    }
    // Go forward for 200ms and check if there is black line. If there isn't
    // turn right else go keep going straight.
    if (digitalRead(middleIR) == ON_WHITE) {
      turn(TURN_RIGHT, NO_FORWARD_ADJUST);
    }
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
    unsigned long startMillis = millis();
    while (millis() - startMillis < 150) {
      go_straight();
    }

    int leftMost = digitalRead(leftMostIR);
    int left = digitalRead(leftIR);
    int middle = digitalRead(middleIR);
    int right = digitalRead(rightIR);
    int rightMost = digitalRead(rightMostIR);
    // Sometimes all the sensors may detect white momentarily as it
    // goes along a curve, triggering the deadend condition. To prevent
    // false positives, go forward a bit more and check if all sensors
    // are still on white. If so this is actually a deadend.
    if (leftMost == ON_WHITE &&
        left == ON_WHITE &&
        middle == ON_WHITE &&
        right == ON_WHITE &&
        rightMost == ON_WHITE) {
      turnaround();
    }

  }
}
