/*
 * @author Mudasir Ali
 * Contact: mudali25@bergen.org
 * Created at 2/23/2023
 * Some portions modified from Adafruit 
    MotorShield Example Code
 * 
 * @brief controls the drive train for the goose chaser
*/

#include <Adafruit_MotorShield.h>

Adafruit_MotorShield AFMS = Adafruit_MotorShield();

//motor references 
Adafruit_DCMotor* mot1 = AFMS.getMotor(1);
Adafruit_DCMotor* mot2 = AFMS.getMotor(2);
Adafruit_DCMotor* mot3 = AFMS.getMotor(3);
Adafruit_DCMotor* mot4 = AFMS.getMotor(4);

//array of motor pointers to conduct actions on all four
Adafruit_DCMotor* motors = [mot1, mot2, mot3, mot4];
const int MOTOR_COUNT = 4;

void setup() {
  Serial.begin(9600);

  if (!AFMS.begin()) {
    Serial.println("Motor shield not detected");
    while (1);
  }
  Serial.println("Motor Shield detected.");

  //init speed to max, but send no power
  setSpeedAll(MOTOR_COUNT, motors, 255);
  runAll(MOTOR_COUNT, motors, RELEASE);
}

void loop() {
  //drive forward
  Serial.println("Driving forward");
  runRWD(FORWARD);
  delay(1000);
  
  //drive backward
  Serial.println("Driving backward");
  runRWD(BACKWARD);
  delay(1000);

  //turn left
  Serial.println("Turning left");
  turn("left");
  delay(1000);

  //turn right
  Serial.println("Turning right");
  turn("right");
  delay(1000);

  //stop
  Serial.println("Stopping");
  runAll(MOTOR_COUNT, motors, RELEASE);
  delay(1000);
}

void setSpeedAll(int motorCount, Adafruit_DCMotor* motors[], int speed) {
  int motorIndex;
  for (motorIndex=0;motorIndex<motorCount;motorIndex++) {
    motors[motorIndex]->setSpeed(speed);
  }
}

void runAll(int motorCount, Adafruit_DCMotor* motors[], int direction) {
  int motorIndex;
  for (motorIndex=0;motorIndex<motorCount;motorIndex++) {
    motors[motorIndex]->run(direction);
  }
}

//rear-wheel-drive
void runRWD(int direction) {
  runAll(2, {mot1, mot2}, direction);
}

void turn(char[] direction) {
  switch (direction) {
    case "left":
      runAll(2, {mot1, mot2}, FORWARD);
      runAll(2, {mot4, mot3}, BACKWARD);
      break;
    case "right":
      runAll(2, {mot1, mot2}, BACKWARD);
      runAll(2, {mot4, mot3}, FORWARD);
      break;
    default:
      runAll(4, motors, RELEASE);
      Serial.println("Invalid direction");
      break;
  }
}