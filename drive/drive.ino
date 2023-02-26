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
Adafruit_DCMotor* mot_FR = AFMS.getMotor(1);
Adafruit_DCMotor* mot_BR = AFMS.getMotor(2);
Adafruit_DCMotor* mot_BL = AFMS.getMotor(3);
Adafruit_DCMotor* mot_FL = AFMS.getMotor(4);

//use MOTOR_COUNT to store length of array
const int MOTOR_COUNT = 4;
//array of motor pointers to conduct actions on all four
Adafruit_DCMotor* motors[MOTOR_COUNT] = {mot_FL, mot_BL, mot_FR, mot_BR};

void setup() {
  Serial.begin(9600);

  if (!AFMS.begin()) {
    Serial.println("Motor shield not detected");
    while (1);
  }
  Serial.println("Motor Shield detected.");

  //init speed to max, but send no movement signal
  setSpeedAll(MOTOR_COUNT, motors, 255);
  runAll(MOTOR_COUNT, motors, RELEASE);
}

void loop() {
  Serial.println("Testing");
  testMotors();
  delay(6000);
}

/*
 * @debug
 * use to test what backwards or forwards means to a motor
 *
*/
void testDirs() {
  Serial.println("about to go fwd");
  runAll(MOTOR_COUNT, motors, FORWARD);
  delay(3000);

  Serial.println("about to go rev");
  runAll(MOTOR_COUNT, motors, BACKWARD);
  delay(3000);

  Serial.println("Done testing");
  runAll(MOTOR_COUNT, motors, RELEASE);
}

/*
 * @debug
 * use this function for testing all the util functions
*/
void testMotors() {
  const int DRIVE_TIME = 5000;
  const int PAUSE_TIME = 1000;

  //drive forward
  Serial.println("Driving forward");
  runAll(MOTOR_COUNT, motors, FORWARD);
  // comment the above line and remove the following one to test only the RWD
  // runRWD(FORWARD);
  delay(DRIVE_TIME);
  //stop
  Serial.println("Stopping");
  runAll(MOTOR_COUNT, motors, RELEASE);
  delay(PAUSE_TIME);
  
  //drive backward
  Serial.println("Driving backward");
  runRWD(BACKWARD);
  delay(DRIVE_TIME);
  //stop
  Serial.println("Stopping");
  runAll(MOTOR_COUNT, motors, RELEASE);
  delay(PAUSE_TIME);

  //turn left
  Serial.println("Turning left");
  turn("left");
  delay(DRIVE_TIME);
  //stop
  Serial.println("Stopping");
  runAll(MOTOR_COUNT, motors, RELEASE);
  delay(PAUSE_TIME);

  //turn right
  Serial.println("Turning right");
  turn("right");
  delay(DRIVE_TIME);
  //stop
  Serial.println("Stopping");
  runAll(MOTOR_COUNT, motors, RELEASE);
  delay(DRIVE_TIME);
}

void setSpeedAll(int motorCount, Adafruit_DCMotor* motors[], int speed) {
  int motorIndex;
  for (motorIndex=0;motorIndex<motorCount;motorIndex++) {
    motors[motorIndex]->setSpeed(speed);
  }
}

void runAll(int motorCount, Adafruit_DCMotor* motors[], int direction) {
  //Note: M3 and M4 go backwards
  int motorIndex;
  for (motorIndex=0;motorIndex<motorCount;motorIndex++) {
    motors[motorIndex]->run(RELEASE);
    //swap dir for M3 & M4 (index 0 or 1)
    motors[motorIndex]->run(motorIndex < 2 ? getOppDir(direction) : direction);
  }
}

//rear-wheel-drive
void runRWD(int direction) {
  // Adafruit_DCMotor* RWD[] = {mot_BL, mot_BR};
  // runAll(2, RWD, direction);
  mot_BL->run(BACKWARD);
  mot_BR->run(FORWARD);  
}

void turn(const char* direction) {
  Adafruit_DCMotor* right_wheels[] = {mot_FR, mot_BR};
  Adafruit_DCMotor* left_wheels[] = {mot_FL, mot_BL};
  if (strcmp(direction, "left") == 0) {
      runAll(2, right_wheels, FORWARD);
      runAll(2, left_wheels, FORWARD); //this is actually backwards
      delay(1000);
  } else if (strcmp(direction, "right") == 0) {
      runAll(2, right_wheels, BACKWARD);
      runAll(2, left_wheels, BACKWARD); //this is actually forwards
      delay(1000);
  } else { 
      runAll(4, motors, RELEASE);
      Serial.println("Invalid direction");
  }
}

int getOppDir(int direction) {
  if (direction == FORWARD) return BACKWARD; 
  if (direction == BACKWARD) return FORWARD; 
  if (direction == RELEASE) return RELEASE;
}
