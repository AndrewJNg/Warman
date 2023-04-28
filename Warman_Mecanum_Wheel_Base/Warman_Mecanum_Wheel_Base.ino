void setup();
void loop();
void system();

float tof_distance = 0;
boolean Start = false;

//////////////////////////////////////////////////////////////////////////////
double startPoint = 0;
double endPoint = 0;
int Mode = 1;  // set mode 1 as default
unsigned long StartTimer = 0;

double KpLeft = 1.15, KiLeft = 0.5, KdLeft = 0.002;
double KpRight = 1.15, KiRight = 0.5, KdRight = 0.002;

double rpmSetLeft, InputRpmLeft, OutputLeftMotor;
double rpmSetRight, InputRpmRight, OutputRightMotor;
double turnSetpoint, turnInput, turnOutput;
double straightSetpoint, straightInput, straightOutput;

int currSpeedEncLeft = 0;
int currSpeedEncRight = 0;

int averageSpeed = 2700;  // set average speed of the mechanism to 2700
int speedDiff = 500;      // set different to be 500 , positive means backSpin, negative means topSpin

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
  // tof_setup();
  OLED_setup();

  motor(motor_Speed);
  pinMode(35, INPUT);

  // setup ball collector servo
  PWMsetup(0, 19);  // set pin 19 to be a servo pin, on channel 0
  setPos(0, 180);   // turn to 180 degree

  PWMsetup(1, 18);  // set pin 19 to be a servo pin, on channel 0
  setPos(1, 0);     // turn to 0 degree
  
  for (int i = 0; i < 1000; i++) {
    OLED_display_stats();
    rpmMove((i*(averageSpeed-speedDiff/2))/1000,(i*(averageSpeed+speedDiff/2))/1000);
    delay(5); // takes 5 seconds to spin up
  }
}

void loop() {
  rpmMove(averageSpeed-speedDiff/2, averageSpeed+speedDiff/2);

  if (Start) {
    Serial.println(Start);
    // do {
    //   mecanum_move(100, 88);  // speed 100, at 80 degree
    // } while (tof_distance >= 600);
    mecanum_move(0, 0);  // stop


    setPos(1, 30);  // turn to 50 degree
    delay(500);
    setPos(1, 0);  // turn to 0 degree

    /*
    mecanum_move(50, -90);
    setPos(0, 180);  // turn to 180 degree
    delay(500);

    mecanum_move(0, -90);
    delay(1000);

    for (int i = 180; i > 120; i--) {
      setPos(0, i);  // slowly lift servo motor
      delay(30);
    }
    delay(3000);

    setPos(0, 180);  // turn servo motor back down
    */
    Start = 0;

  } else {
    mecanum_move(0, 0);
    if (digitalRead(35) == 1)
      Start = 1;
    // Serial.println(Start);
  }

  // Serial.println(digitalRead(35));
  // start_button_menu.count(start_button(), &Start, 1);
  // PS3_move(stick_LX, stick_LY, stick_RX, stick_RY);
}
void system() {
  // tof_loop();
  OLED_display_stats();
}
