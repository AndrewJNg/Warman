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

int squashSpeed = 510;
int squashDiff = 0;
int tennisSpeed = 710;
int tennisDiff = -90;

// int squashSpeed = 1210;
// int squashDiff = -5;
// int tennisSpeed = 2200;
// int tennisDiff = -50;


int averageSpeed = 0;  // set average flywheel speed
int speedDiff = 0;     // set flywheel difference, positive means backSpin, negative means topSpin

//////////////////////////////////////////////////////////////////////////////

#include "Memory.h"
#include "lowPass.h"
#include "tof.h"
#include "MPU6050.h"
#include "PS3.h"

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

  // Library Setups
  PS3_setup();
  Enc_setup();
  PID_setup();
  // Gyro_setup() ;

  Movement_setup();
  flywheelMotor_setup();
  tof_setup();
  OLED_setup();


  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

  // turn motors off
  motor(motor_Speed);
  pinMode(35, INPUT);


  // setup ball collector servo
  PWMsetup(0, 19);  // set pin 19 to channel 0
  setPos(0, 180);

  // set launch arm servo
  PWMsetup(1, 18);  // set pin 18 to channel 0
  setPos(1, 175);

    // store_memory();
  read_memory();
}

void loop() {
  system();
  if (Start) {

    // tof_drive_on_wall(boolean driveLeft, boolean leftSilo, boolean leaveSilo);
    // time_drive_on_wall(boolean driveLeft, int time);
    // reverse(int speed, int time);

    averageSpeed = squashSpeed;  // set squash speed for flywheel
    speedDiff = squashDiff;

    tof_drive_on_wall(true, true, false);  // drive left until it hit silo
    time_drive_on_wall(true, 1000);
    ball_gripper(2000);  // pickup first ball

    tof_drive_on_wall(true, true, true);  // drive left until it leave silo
    time_drive_on_wall(true, 1500);

    // lift arm up halfway
    setPos(0, 135);
    stop_delay(500);

    tof_drive_on_wall(false, true, false);  // drive right until it hit silo

    shoot();         // Shoot
    ball_gripper(2000);  // pickup second ball
    shoot();         // Shoot

    time_drive_on_wall(true, 3000);
    ball_gripper(2000);  // pickup third ball
    tof_drive_on_wall(false, true, false);  // drive right until it hit silo
    shoot();

    averageSpeed = tennisSpeed;  // set tennis speed for flywheel
    speedDiff = tennisDiff;

    time_drive_on_wall(true, 4000);

    // lift arm up
    setPos(0, 85);
    stop_delay(500);

    /////////////////////////////////////////

    forward(-100, 1000);  // reverse
    turn180(150, 1100);   // turn 180
    forward(100, 750);    // forward

    forward(100, 500);                      // forward
    tof_drive_on_wall(false, false, true);  // drive right until it leave silo

    // lift arm down
    setPos(0, 180);
    stop_delay(500);


    tof_drive_on_wall(true, true, false);  // drive left until it hit silo
    time_drive_on_wall(true, 1300);
    ball_gripper(2000);  // pickup first tennis ball

    time_drive_on_wall(true, 3000);


    // lift arm up
    setPos(0, 85);  // pickup 2nd & 3rd tennis ball

    
    stop_delay(500);

    tof_drive_on_wall(false, true, false);  // drive right until it hit silo
    time_drive_on_wall(false, 800);

    stop_delay(5000);
    shoot();  // Shoot
    
    // lift arm up
    // setPos(0, 175);  // pickup 2nd & 3rd tennis ball
    // setPos(0, 180);  // pickup 2nd & 3rd tennis ball

    stop_delay(5000);
    shoot();  // Shoot
    // ball_gripper(2000);  // pickup third ball
    stop_delay(5000);
    
    // lift arm up
    // setPos(0, 175);  // pickup 2nd & 3rd tennis ball
    // setPos(0, 180);  // pickup 2nd & 3rd tennis ball

    shoot();  // Shoot
    // ball_gripper(2000);  // pickup third ball
    stop_delay(2000);
    // lift arm up
    // setPos(0, 85);  // pickup 2nd & 3rd tennis ball
    // stop_delay(500);

    averageSpeed = 0;  // turn flywheel off
    speedDiff = 0;
    mecanum_move(150, -100, 0);
    delay(1300);














    Start = 0;
  } else if (Start2) {
    ball_gripper(2000);
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
  setPos(1, 120);  // turn launch arm up to shoot
  currentMillis = millis();
  while ((millis() - currentMillis) < 500) {
    system();
  }
  setPos(1, 175);  // set launch arm down
  mecanum_move(0, 0, 0);
  stop_delay(500);
}

void ball_gripper(int timeInterval) {
  setPos(0, 85);
  currentMillis = millis();
  while ((millis() - currentMillis) < timeInterval) {
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
  OLED_display_stats();
}
