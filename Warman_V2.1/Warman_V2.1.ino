void setup();
void loop();
void system();

float tof_distance = 0;
boolean Start = false;
boolean Start2 = false;
boolean Start3 = false;
unsigned long currentMillis = 0;

//////////////////////////////////////////////////////////////////////////////
double startPoint = 0;
double endPoint = 0;
int Mode = 1;  // set mode 1 as default
unsigned long StartTimer = 0;

double KpLeft = 0, KiLeft = 10.0, KdLeft = 0.00;
double KpRight = 0, KiRight = 10.0, KdRight = 0.00;

double rpmSetLeft, InputRpmLeft, OutputLeftMotor;
double rpmSetRight, InputRpmRight, OutputRightMotor;
double turnSetpoint, turnInput, turnOutput;
double straightSetpoint, straightInput, straightOutput;

int currSpeedEncLeft = 0;
int currSpeedEncRight = 0;

// int averageSpeed = 28800;  // set average speed of the mechanism to 2700
int averageSpeed = 0;  // set average speed of the mechanism to 2700
int speedDiff = 0;     // set different to be 500 , positive means backSpin, negative means topSpin
// int averageSpeed = 0;  //180;  // set average speed of the mechanism to 2700
// int speedDiff = 0;     // set different to be 500 , positive means backSpin, negative means topSpin

//////////////////////////////////////////////////////////////////////////////

#include "lowPass.h"
#include "tof.h"
#include "MPU6050.h"
#include "PS3.h"`

#include "AS5600.h"
#include "Movement.h"
#include "Flywheel.h"
#include "PID.h"
#include "Speed_profile.h"
#include "Esp32_Servo_Library.h"
#include "OLED.h"

void setup() {
  // Setup Read Spead and Serial monitor's Baud Rate
  Serial.begin(115200);

  // // Additional Macro Setups

  // Macro Setups
  PS3_setup();
  Enc_setup();
  PID_setup();
  // Gyro_setup() ;

  Movement_setup();
  flywheelMotor_setup();
  tof_setup();
  OLED_setup();

  motor(motor_Speed);
  pinMode(35, INPUT);

  // setup ball collector servo
  PWMsetup(0, 19);  // set pin 19 to be a servo pin, on channel 0
  setPos(0, 180);   // turn to 180 degree

  PWMsetup(1, 18);  // set pin 18 to be a servo pin, on channel 0
  setPos(1, 175);   // turn to 0 degree

  // for (int i = 0; i < 1000; i++) {
  // OLED_display_stats();
  // rpmMove((i * (averageSpeed - speedDiff / 2)) / 1000, (i * (averageSpeed + speedDiff / 2)) / 1000);
  // rpmMove((i * (averageSpeed - speedDiff / 2)) / 1000, 0);
  // flywheelMotor(averageSpeed - speedDiff / 2, averageSpeed + speedDiff / 2);
  // delay(5);  // takes 5 seconds to spin up
  // }
}

void loop() {
  system();
  if (Start) {

    // boolean driveLeft, boolean leftSilo, boolean leaveSilo
    // time_drive_on_wall(boolean driveLeft, int time)
    // reverse(int speed, int time);

    averageSpeed = 1200;                   // set average speed of the mechanism to 1180
    tof_drive_on_wall(true, true, false);  // drive left until it hit silo
    time_drive_on_wall(true, 1000);
    ball_gripper();  // pickup first ball

    tof_drive_on_wall(true, true, true);  // drive left until it leave silo
    time_drive_on_wall(true, 1500);

    // lift arm up halfway
    setPos(0, 135);
    stop_delay(500);

    tof_drive_on_wall(false, true, false);  // drive right until it hit silo

    shoot();         // Shoot
    ball_gripper();  // pickup second ball
    shoot();         // Shoot

    time_drive_on_wall(true, 3000);
    ball_gripper();                         // pickup third ball

    tof_drive_on_wall(false, true, false);  // drive right until it hit silo

    shoot();
    
    averageSpeed = 2150;                   // set average speed of the mechanism to 2150
    time_drive_on_wall(true, 4000);

    // lift arm up
    setPos(0, 85);
    stop_delay(500);


    forward(-100, 1000);  // reverse
    turn180(150, 1050);   // turn 180
    forward(100, 750);   // forward

    forward(100, 500);   // forward
    tof_drive_on_wall(false, false, true);  // drive right until it leave silo

    // lift arm down    
    setPos(0, 180);
    stop_delay(500);


    tof_drive_on_wall(true, true, false);  // drive left until it hit silo
    time_drive_on_wall(true, 1000);
    ball_gripper();  // pickup first tennis ball

    // tof_drive_on_wall(false, true, false);  // drive right until it hit silo
    
    // stop_delay(5000);
    // shoot();         // Shoot

    time_drive_on_wall(true, 3000);


    // lift arm up
    setPos(0, 85);   // pickup 2nd & 3rd tennis ball
    stop_delay(500);

    tof_drive_on_wall(false, true, false);  // drive right until it hit silo

    stop_delay(5000);
    shoot();         // Shoot
    stop_delay(5000);
    shoot();         // Shoot
    stop_delay(5000);
    shoot();         // Shoot
    stop_delay(2000);
    
    averageSpeed = 0;                   // set average speed of the mechanism to 2150
    mecanum_move(150, -100, 0);
    delay(1500);














    Start = 0;
  } else if (Start2) {
    ball_gripper();
    Start2 = 0;
  } else if (Start3) {
    shoot();
    Start3 = 0;
  }

  else {
    // mecanum_move(0, 0,0);
    PS3_move(stick_LX, stick_LY, stick_RX, stick_RY);
    if (digitalRead(35) == 1)
      Start = 1;
  }
}

void shoot() {
  setPos(1, 120);  // turn to 50 degree
  currentMillis = millis();
  while ((millis() - currentMillis) < 500) {
    system();
  }
  setPos(1, 175);  // turn to 0 degree
  mecanum_move(0, 0, 0);
  stop_delay(500);
}

void ball_gripper() {
  setPos(0, 85);
  currentMillis = millis();
  while ((millis() - currentMillis) < 3000) {
    system();
    PS3_move(stick_LX, stick_LY, stick_RX, stick_RY);
  }
  setPos(0, 180);
  mecanum_move(0, 0, 0);
  stop_delay(500);
}

void stop_delay(int time_interval) {
  currentMillis = millis();
  while ((millis() - currentMillis) < time_interval) {
    system();
  }
}

void tof_drive_on_wall(boolean driveLeft, boolean leftSilo, boolean leaveSilo) {
  bool condition = 0;

  do {
    if (driveLeft)
      mecanum_move(100, -60, 0);

    else
      mecanum_move(100, 60, 0);

    if (leaveSilo)
      condition = tof_distance <= 600;
    else condition = tof_distance >= 600;
    system();

  } while (condition);


  mecanum_move(0, 0, 0);
  stop_delay(500);
}

void time_drive_on_wall(boolean driveLeft, int time) {
  currentMillis = millis();
  while ((millis() - currentMillis) < time) {
    if (driveLeft) mecanum_move(100, -60, 0);
    else mecanum_move(100, 60, 0);
    system();
  }
  mecanum_move(0, 0, 0);
  stop_delay(200);
}

void forward(int speed, int time) {

  currentMillis = millis();
  while ((millis() - currentMillis) < time) {
    if (speed < 0)
      mecanum_move(abs(speed), -180, 0);
    else
      mecanum_move(abs(speed), 0, 0);

    system();
  }
  mecanum_move(0, 0, 0);
  stop_delay(500);
}

void turn180(int speed, int time) {

  currentMillis = millis();
  while ((millis() - currentMillis) < time) {
    mecanum_move(0, 0, speed);
    system();
  }
  mecanum_move(0, 0, 0);
  stop_delay(500);
}

void system() {
  tof_loop();
  // Gyro_update();
  rpmMove(averageSpeed - speedDiff / 2, averageSpeed + speedDiff / 2);
  // rpmMove(averageSpeed - speedDiff / 2,0);
  // flywheelMotor(averageSpeed - speedDiff / 2, averageSpeed + speedDiff / 2);
  OLED_display_stats();
}
