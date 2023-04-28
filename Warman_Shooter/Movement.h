// flywheelMotor control system for TB6612FNG flywheelMotor driver
// uses variables from encoder to allow better forward and backward movement
extern bool flywheelMotorForward[2];


// const byte flywheelMotorPin1[] = { 18, 1 };
// const byte flywheelMotorPin2[] = {2, 17};

const byte flywheelMotorPWM[] = { 32, 33 };



//const byte flywheelMotorSTBY[] = {27};

//ESP32 settings for pwm channel
const byte flywheelMotorChannel[] = { 0, 1 };
const byte PWM_resolution = 8;             //16;
const int PWM_resolution_max_value = 255;  //65535;


void flywheelMotor(int speedLeft, int speedRight) {
  system();
  if (speedLeft > 255) speedLeft = 255;
  else if (speedLeft < -255) speedLeft = -255;
  if (speedRight > 255) speedRight = 255;
  else if (speedRight < -255) speedRight = -255;

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
    ledcSetup(flywheelMotorChannel[x], 5000, PWM_resolution);  // 50 Hz, 16-bit width
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
  //   ledcSetup(flywheelMotorChannel[x], 5000, PWM_resolution);
  //   ledcAttachPin(flywheelMotorPWM[x] , flywheelMotorChannel[x] );
  //   ledcWrite(flywheelMotorChannel[x] , 0);

  //   pinMode(flywheelMotorPin1[x], OUTPUT);


  /*
    // set direction pins
    pinMode(flywheelMotorPin1[x], OUTPUT);
    pinMode(flywheelMotorPin2[x], OUTPUT);

    //set pwm pins
    ledcSetup(flywheelMotorChannel[x], 5000, PWM_resolution);
    ledcAttachPin(flywheelMotorPWM[x] , flywheelMotorChannel[x] );

    digitalWrite(flywheelMotorPin1[x], HIGH);
    digitalWrite(flywheelMotorPin2[x], HIGH);
    ledcWrite(flywheelMotorChannel[x] , 0);
    */
  // }
}
