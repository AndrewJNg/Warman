/********************************************************
   PID Basic Example
   Reading analog input 0 to control analog PWM output 3
 ********************************************************/
//https://github.com/br3ttb/Arduino-PID-Library/
#include <PID_v1.h>

//#define PIN_INPUT 0
//#define PIN_OUTPUT 3

//Define Variables we'll be connecting to

//Specify the links and initial tuning parameters
PID leftMotorVel(&InputRpmLeft, &OutputLeftMotor, &rpmSetLeft, KpLeft, KiLeft, KdLeft, DIRECT);
PID rightMotorVel(&InputRpmRight, &OutputRightMotor, &rpmSetRight, KpRight, KiRight, KdRight, DIRECT);

// PID turnPID(&turnInput, &turnOutput, &turnSetpoint, turnKp, turnKi, turnKd, DIRECT);
// PID straightPID(&straightInput, &straightOutput, &straightSetpoint, straightKp, straightKi, straightKd, DIRECT);

void PID_setup()
{
  //initialize the variables we're linked to
  //  Input = analogRead(PIN_INPUT);
  //  Setpoint = 100;

  leftMotorVel.SetMode(AUTOMATIC);
  leftMotorVel.SetSampleTime(10);
  leftMotorVel.SetOutputLimits(-65536, 65536);

  rightMotorVel.SetMode(AUTOMATIC);
  rightMotorVel.SetSampleTime(10);
  rightMotorVel.SetOutputLimits(-65536, 65536);

  // turnPID.SetMode(AUTOMATIC);
  // turnPID.SetSampleTime(1);
  // turnPID.SetOutputLimits(-100, 100);  // turn speed

  // straightPID.SetMode(AUTOMATIC);
  // straightPID.SetSampleTime(1);
  // straightPID.SetOutputLimits(-255, 255);  // turn speed
}
