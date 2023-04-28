
void setup();
void loop();
void system();

double startPoint = 0;
double endPoint = 0;
int Mode = 1;  // set mode 1 as default
boolean Start = false;
unsigned long StartTimer = 0;

//double Kp = 1.8, Ki = 1.8, Kd = 0.12;
//double Kp = 5, Ki = 4.5, Kd = 0.03;
double KpLeft = 1.15, KiLeft = 0.5, KdLeft = 0.002;
double KpRight = 1.15, KiRight = 0.5, KdRight = 0.002;

//double turnKp = 2, turnKi = 0, turnKd = 0.000;
//double turnKp = 1, turnKi = 2, turnKd = 0.001;
// double turnKp = 1, turnKi = 2, turnKd = 0.0001;

// double straightKp = 1, straightKi = 0, straightKd = 0;

double rpmSetLeft, InputRpmLeft, OutputLeftMotor;
double rpmSetRight, InputRpmRight, OutputRightMotor;
double turnSetpoint, turnInput, turnOutput;
double straightSetpoint, straightInput, straightOutput;

int currSpeedEncLeft = 0; 
int currSpeedEncRight = 0;

int averageSpeed = 2700; // set average speed of the mechanism to 2700
int speedDiff = 500; // set different to be 500 , positive means backSpin, negative means topSpin

#include "PS3.h"
#include "AS5600.h"
#include "Movement.h"
#include "PID.h"
#include "Speed_profile.h"
#include "OLED.h"


// int averageSpeed = 1000; // set average speed of the mechanism to 2750
// int speedDiff = 0; // set different to be 500 , positive means backSpin, negative means topSpin
void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  PS3_setup();
  Enc_setup();
  PID_setup();
  flywheelMotor_setup();
  OLED_setup();
  

  for (int i = 0; i < 1000; i++) {
    OLED_display_stats();
    rpmMove((i*(averageSpeed-speedDiff/2))/1000,(i*(averageSpeed+speedDiff/2))/1000);
    delay(5); // takes 5 seconds to spin up
  }
  // motor(50,50);
}

void loop() {
  system();
  rpmMove(averageSpeed-speedDiff/2, averageSpeed+speedDiff/2);
  OLED_display_stats();
}

void system() {
  //system functions, important to keep different time sensitive functions working
  // Gyro_update();
  // notify();
}