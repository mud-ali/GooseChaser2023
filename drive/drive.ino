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
Adafruit_DCMotor* mot_BR = AFMS.getMotor(1);
Adafruit_DCMotor* mot_FR = AFMS.getMotor(2);
Adafruit_DCMotor* mot_FL = AFMS.getMotor(3);
Adafruit_DCMotor* mot_BL = AFMS.getMotor(4);

const int MOTOR_COUNT = 4;
//array of motors to conduct actions on all four
Adafruit_DCMotor* motors[MOTOR_COUNT] = {mot_FL, mot_BL, mot_FR, mot_BR};
Adafruit_DCMotor* right_wheels[2] = {mot_FR, mot_BR};
Adafruit_DCMotor* left_wheels[2] = {mot_FL, mot_BL};

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
  setSpeedAll(4, motors, 255);
  runAll(MOTOR_COUNT, motors, RELEASE);

  testMotors();
  delay(3000);
  runAll(MOTOR_COUNT, motors, RELEASE);
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
  // runAll(MOTOR_COUNT, motors, FORWARD);
  // comment the above line and remove the following one to test only the RWD
  runRWD(FORWARD);
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

void patrolField(int xTime, int turnTime, int rows) {
  for (int i=0;i<rows;i++) {
    runAll(MOTOR_COUNT, motors, FORWARD);
    delay(xTime);
    runAll(MOTOR_COUNT, motors, RELEASE);
    
    turn("right");
    delay(turnTime);
    runAll(MOTOR_COUNT, motors, FORWARD);
    delay(turnTime);
    runAll(MOTOR_COUNT, motors, RELEASE);

    turn("right");
    delay(turnTime);
    runAll(MOTOR_COUNT, motors, RELEASE);
  }
}

void setSpeedAll(int motorCount, Adafruit_DCMotor* motors[], int speed) {
  mot_FL->setSpeed(speed);
  mot_BR->setSpeed(speed);
  mot_BL->setSpeed(speed);
  mot_FR->setSpeed(speed);
}

void runAll(int motorCount, Adafruit_DCMotor* motors[], int direction) {
  mot_FL->setSpeed(255);
  mot_BR->setSpeed(255);
  mot_BL->setSpeed(255);
  mot_FR->setSpeed(255);

  mot_FR->run(direction);
  mot_BR->run(direction);
  mot_BL->run(direction);
  mot_FL->run(direction);
}

void runRWD(int direction) {
  mot_BL->run(direction);
  mot_BR->run(direction);  
}

void turn(const char* direction) {
  if (strcmp(direction, "left") == 0) {
      mot_FR->run(FORWARD); //problems
      mot_BL->run(BACKWARD);
      mot_BR->run(FORWARD); 
      mot_FL->run(BACKWARD);
  } else if (strcmp(direction, "right") == 0) {
      mot_FR->run(BACKWARD); //problems
      mot_BL->run(FORWARD);
      mot_BR->run(BACKWARD); 
      mot_FL->run(FORWARD);
  } else { 
      runAll(4, motors, RELEASE);
      Serial.println("Invalid direction");
  }
}

int getOppDir(int direction) {
  if (direction == 1) return 2; 
  if (direction == 2) return 1;
  return 3;
}
