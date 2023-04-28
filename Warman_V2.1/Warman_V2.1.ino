void setup();
void loop();
void system();

float tof_distance = 0;
boolean Start = false;
boolean Start2 = false;
boolean Start3 = false;
unsigned long currentMillis =0;

//////////////////////////////////////////////////////////////////////////////
double startPoint = 0;
double endPoint = 0;
int Mode = 1;  // set mode 1 as default
unsigned long StartTimer = 0;

double KpLeft = 0, KiLeft = 8.0, KdLeft = 2.00;
double KpRight = 0, KiRight = 8.0, KdRight = 2.00;

double rpmSetLeft, InputRpmLeft, OutputLeftMotor;
double rpmSetRight, InputRpmRight, OutputRightMotor;
double turnSetpoint, turnInput, turnOutput;
double straightSetpoint, straightInput, straightOutput;

int currSpeedEncLeft = 0;
int currSpeedEncRight = 0;

int averageSpeed = 0;  // set average speed of the mechanism to 2700
int speedDiff = 0;     // set different to be 500 , positive means backSpin, negative means topSpin
// int averageSpeed = 0;  //180;  // set average speed of the mechanism to 2700
// int speedDiff = 0;     // set different to be 500 , positive means backSpin, negative means topSpin

//////////////////////////////////////////////////////////////////////////////

#include "tof.h"
// #include "MPU6050.h"
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
  // OTA_setup();
  // User_interface_setup();
  // MPU6050_setup();

  // Macro Setups
  PS3_setup();
  Enc_setup();
  PID_setup();

  Movement_setup();
  flywheelMotor_setup();
  tof_setup();
  OLED_setup();

  motor(motor_Speed);
  pinMode(35, INPUT);

  // setup ball collector servo
  PWMsetup(0, 19);  // set pin 19 to be a servo pin, on channel 0
  setPos(0, 0);     // turn to 180 degree

  PWMsetup(1, 18);  // set pin 19 to be a servo pin, on channel 0
  setPos(1, 180);   // turn to 0 degree

  // for (int i = 0; i < 1000; i++) {
  // OLED_display_stats();
  // rpmMove((i * (averageSpeed - speedDiff / 2)) / 1000, (i * (averageSpeed + speedDiff / 2)) / 1000);
  // rpmMove((i * (averageSpeed - speedDiff / 2)) / 1000, 0);
  // flywheelMotor(averageSpeed - speedDiff / 2, averageSpeed + speedDiff / 2);
  // delay(5);  // takes 5 seconds to spin up
  // }
}

void loop() {

  if (Start) {
    // Serial.println(Start);
    do {
      mecanum_move(100, 80);  // speed 100, at 80 degree
    } while (tof_distance >= 600);
    mecanum_move(0, 0);  // stop

    stop_delay(1000);
    launch_arm();






    Start = 0;
  } else if (Start2) {
    ball_gripper();
    Start2 = 0;
  } else if (Start3) {
    launch_arm();
    Start3 = 0;
  }

  else {
    // mecanum_move(0, 0);
    PS3_move(stick_LX, stick_LY, stick_RX, stick_RY);
    if (digitalRead(35) == 1)
      Start = 1;
  }
}

void launch_arm() {
  setPos(1, 130);  // turn to 50 degree
  currentMillis = millis();
  while ((millis() - currentMillis) < 500) {
    system();
  }
  setPos(1, 180);  // turn to 0 degree
}

void ball_gripper()
{
    setPos(0, 90);
    currentMillis = millis();
    while ((millis() - currentMillis) < 3000) {
      system();
      PS3_move(stick_LX, stick_LY, stick_RX, stick_RY);
    }
    setPos(0, 0);
}

void stop_delay(int time_interval)
{
    currentMillis = millis();
    while ((millis() - currentMillis) < time_interval) {
      system();
    }
}



void system() {
  tof_loop();
  rpmMove(averageSpeed - speedDiff / 2, averageSpeed + speedDiff / 2);
  // rpmMove(averageSpeed - speedDiff / 2,0);
  // flywheelMotor(averageSpeed - speedDiff / 2, averageSpeed + speedDiff / 2);
  OLED_display_stats();
}
