// // Additional Functionalities
void setup();
void loop();
void system();

boolean Start = false;
// #include "OTA.h"
// #include "User_interface.h"
// #include "MPU6050.h"

// Functional Macros
// #include "PS3.h"`
#include "Movement.h"
// #include "User_interface.h"
#include "Esp32_Servo_Library.h"

void setup() {
  // Setup Read Spead and Serial monitor's Baud Rate
  // analogReadResolution(12);
  Serial.begin(115200);

  // // Additional Macro Setups
  // OTA_setup();
  // User_interface_setup();
  // MPU6050_setup();

  // Macro Setups
  // PS3_setup();
  Movement_setup();

  motor(motor_Speed);
  // while (!Ps3.isConnected()) {
  //   motor(motor_Speed);
  //   PS3_move(stick_LX, stick_LY, stick_RX, stick_RY);
  // }
  pinMode(35, INPUT);
  // while(digitalRead(35) ==0);
  PWMsetup(0, 18);  // set pin 18 to be a servo pin, on channel 0
  setPos(0, 180);   // turn to 50 degree
}

void loop() {

  if (Start) {
    Serial.println(Start);

    mecanum_move(50, -90 * PI / 180);
    setPos(0, 180);  // turn to 180 degree
    delay(500);

    mecanum_move(0, -90 * PI / 180);
    delay(1000);

    for (int i = 180; i > 120; i--) {
      setPos(0, i);  // turn to 90 degree
      delay(30);
    }
    delay(3000);
    // mecanum_move(255, -90*PI/180);
    // delay(1000);
    // mecanum_move(0, 0*PI/180);
    // delay(1000);
    // mecanum_move(255, -10*PI/180);
    // delay(1000);
    setPos(0, 180);  // turn to 50 degree
    Start = 0;
  } else {
    mecanum_move(0, 0);
    if (digitalRead(35) == 1)
      Start = 1;
    Serial.println(Start);
  }

  // Serial.println(digitalRead(35));
  // start_button_menu.count(start_button(), &Start, 1);
  // PS3_move(stick_LX, stick_LY, stick_RX, stick_RY);
}
void system() {
}
