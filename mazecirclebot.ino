#include <AFMotor.h>

// IR pins
#define leftMostIR A4
#define leftIR A3
#define middleIR A2
#define rightIR A1
#define rightMostIR A0

AF_DCMotor motor1(1, MOTOR12_1KHZ); 
AF_DCMotor motor2(2, MOTOR12_1KHZ);

void left_turn() {
  motor1.setSpeed(100);
  motor2.setSpeed(100);
  motor1.run(FORWARD);
  motor2.run(BACKWARD);
}

void right_turn() {
  motor1.setSpeed(100);
  motor2.setSpeed(100);
  motor1.run(BACKWARD);
  motor2.run(FORWARD);
}


void setup() {
  pinMode(leftMostIR, INPUT);
  pinMode(leftIR, INPUT);
  pinMode(middleIR, INPUT);
  pinMode(rightIR, INPUT);
  pinMode(rightMostIR, INPUT);

  delay(1000); // 1s delay to do positioning adjustments
}

void loop(){
  //line detected by both
  if(digitalRead(leftIR)==0 && digitalRead(rightIR)==0){
    //Forward
    motor1.run(FORWARD);
    motor1.setSpeed(100);
    motor2.run(FORWARD);
    motor2.setSpeed(100);

  }
  //line detected by left sensor
  else if(digitalRead(leftIR)==0 && !analogRead(rightIR)==0){
    //turn left
    motor1.run(FORWARD);
    motor1.setSpeed(100);
    motor2.run(BACKWARD);
    motor2.setSpeed(100);


  }
  //line detected by right sensor
  else if(!digitalRead(leftIR)==0 && digitalRead(rightIR)==0){
    //turn right
    motor1.run(BACKWARD);
    motor1.setSpeed(100);
    motor2.run(FORWARD);
    motor2.setSpeed(100);

  }
  //line detected by none
  else if(!digitalRead(leftIR)==0 && !digitalRead(rightIR)==0){
    //stop
    motor1.run(RELEASE);
    motor1.setSpeed(0);
    motor2.run(RELEASE);
    motor2.setSpeed(0);

  }

}
