// flywheelMotor control system for TB6612FNG flywheelMotor driver
// uses variables from encoder to allow better forward and backward movement
extern bool flywheelMotorForward[2];


// const byte flywheelMotorPin1[] = { 18, 1 };
// const byte flywheelMotorPin2[] = {2, 17};

//  motor control pin layout   [ Top, bottom]
const byte flywheelMotorPWM[] = { 33, 32 };



//const byte flywheelMotorSTBY[] = {27};

//ESP32 settings for pwm channel
const byte flywheelMotorChannel[] = { 10, 11 };
const byte flywheel_PWM_resolution = 16;              //16;
const int flywheel_PWM_resolution_max_value = 65535;  //65535;


void flywheelMotor(int speedLeft, int speedRight) {
  system();
  if (speedLeft > flywheel_PWM_resolution_max_value) speedLeft = flywheel_PWM_resolution_max_value;
  else if (speedLeft < -flywheel_PWM_resolution_max_value) speedLeft = -flywheel_PWM_resolution_max_value;
  if (speedRight > flywheel_PWM_resolution_max_value) speedRight = flywheel_PWM_resolution_max_value;
  else if (speedRight < -flywheel_PWM_resolution_max_value) speedRight = -flywheel_PWM_resolution_max_value;

  // invert flywheelMotors when needed
  int Speed[2] = { speedLeft, speedRight };
  // Serial.print(speedLeft);
  // Serial.print("");
  // Serial.println(speedRight);
  // for loop to run each flywheelMotors individually
  for (int x = 0; x < sizeof(flywheelMotorPWM); x++) {

    if (Speed[x] > 0) {
      // Forward
      // digitalWrite(flywheelMotorPin1[x], HIGH);
      // digitalWrite(flywheelMotorPin2[x], LOW);
      ledcWrite(flywheelMotorChannel[x], abs(Speed[x]));
    } else if (Speed[x] < 0) {
      // Reverse
      // digitalWrite(flywheelMotorPin1[x], LOW);
      // digitalWrite(flywheelMotorPin2[x], HIGH);
      ledcWrite(flywheelMotorChannel[x], 0);
    } else {
      //Stop
      // digitalWrite(flywheelMotorPin1[x], HIGH);
      // digitalWrite(flywheelMotorPin2[x], HIGH);
      ledcWrite(flywheelMotorChannel[x], 0);
    }
  }
}



void flywheelMotor_setup() {

  for (int x = 0; x < sizeof(flywheelMotorPWM); x++) {
    ledcSetup(flywheelMotorChannel[x], 5000, flywheel_PWM_resolution);  // 50 Hz, 16-bit width
    ledcAttachPin(flywheelMotorPWM[x], flywheelMotorChannel[x]);

    // pinMode(flywheelMotorPin1[x], OUTPUT);
  }
  // activate Standby pin
  //  for (int x = 0; x < sizeof(flywheelMotorSTBY); x++)
  //  {
  //    pinMode(flywheelMotorSTBY[x], OUTPUT);
  //  }

  // for (int x = 0; x < sizeof(flywheelMotorPin1); x++)
  // {
  //   ledcSetup(flywheelMotorChannel[x], 5000, flywheel_PWM_resolution);
  //   ledcAttachPin(flywheelMotorPWM[x] , flywheelMotorChannel[x] );
  //   ledcWrite(flywheelMotorChannel[x] , 0);

  //   pinMode(flywheelMotorPin1[x], OUTPUT);


  /*
    // set direction pins
    pinMode(flywheelMotorPin1[x], OUTPUT);
    pinMode(flywheelMotorPin2[x], OUTPUT);

    //set pwm pins
    ledcSetup(flywheelMotorChannel[x], 5000, flywheel_PWM_resolution);
    ledcAttachPin(flywheelMotorPWM[x] , flywheelMotorChannel[x] );

    digitalWrite(flywheelMotorPin1[x], HIGH);
    digitalWrite(flywheelMotorPin2[x], HIGH);
    ledcWrite(flywheelMotorChannel[x] , 0);
    */
  // }
}
