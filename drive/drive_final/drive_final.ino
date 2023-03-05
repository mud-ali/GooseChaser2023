/*
 * @author Mudasir Ali
 * Contact: mudali25@bergen.org
 * Created at 2/23/2023
 * Modified 3/03/2023
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
  if (!AFMS.begin()) {
    while (1);
  }

  //init speed to max, but send no movement signal
  setSpeedAll(255);
  runAll(RELEASE);
}

void loop() {
  setSpeedAll(255);
  runAll(RELEASE);
  delay(1000);

  display();
  delay(4000);
  runAll(RELEASE);

}

/*
 * @debug
 * use to display working drive
 *
*/
void display() {
  runRWD(FORWARD);
  delay(2000);
  
  runAll(RELEASE);
  delay(2000);

  runRWD(BACKWARD);
  delay(2000);
  runAll(RELEASE);

}

/*
 * @brief simulated patrolling of a field
 * @param xTime - time to go across length of field
 * @param turnTime - the amount of time needed to turn 90deg
 * @param rows - number of times to go across length of field
 *
*/
void patrolField(int xTime, int turnTime, int rows) {
  for (int i=0;i<rows;i++) {
    runAll(FORWARD);
    delay(xTime);
    runAll(RELEASE);
    
    turn("right");
    delay(turnTime);
    runAll(FORWARD);
    delay(turnTime);
    runAll(RELEASE);

    turn("right");
    delay(turnTime);
    runAll(RELEASE);
  }
}

/*
 * @util
 * @brief sets speed of all motors to the same value
*/
void setSpeedAll(int speed) {
  mot_FL->setSpeed(speed);
  mot_BR->setSpeed(speed);
  mot_BL->setSpeed(speed);
  mot_FR->setSpeed(speed);
}

/*
 * @util
 * @brief runs all motors in the same direction
*/
void runAll(int direction) {
  mot_FL->setSpeed(255);
  mot_FL->run(direction);

  mot_BR->setSpeed(255);
  mot_BR->run(direction);

  mot_BL->setSpeed(255);
  mot_BL->run(direction);

  mot_FR->setSpeed(255);
  mot_FR->run(direction);

  delay(500); //minimum delay
}

/*
 * @util
 * @brief runs back motors in the same direction
*/
void runRWD(int direction) {
  mot_BL->setSpeed(255);
  mot_BR->setSpeed(255);

  mot_BL->run(direction);
  mot_BR->run(direction);  
}

//requires ~18 volts to run all motors
void turn(const char* direction) {
  if (strcmp(direction, "left") == 0) {
      mot_FL->setSpeed(255);
      mot_FL->run(BACKWARD);

      mot_BR->setSpeed(255);
      mot_BR->run(FORWARD);

      // mot_BL->setSpeed(255);
      // mot_BL->run(BACKWARD);

      // mot_FR->setSpeed(255);
      // mot_FR->run(FORWARD);

  } else if (strcmp(direction, "right") == 0) {
      mot_FL->setSpeed(255);
      mot_FL->run(FORWARD);

      mot_BR->setSpeed(255);
      mot_BR->run(BACKWARD);

      // mot_BL->setSpeed(255);
      // mot_BL->run(FORWARD);

      // mot_FR->setSpeed(255);
      // mot_FR->run(BACKWARD);
  } else { 
      runAll(RELEASE);
  }
}
