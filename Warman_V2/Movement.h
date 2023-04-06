// motor control system for TB6612FNG motor driver
// uses variables from encoder to allow better forward and backward movement
extern bool motorForward[2];


// const byte motorPin1[] = { 18, 1 };
// const byte motorPin2[] = {2, 17};

const byte motorPWM[] = { 32, 33 };



//const byte motorSTBY[] = {27};

//ESP32 settings for pwm channel
const byte motorChannel[] = { 0, 1 };
const byte PWM_resolution = 8;             //16;
const int PWM_resolution_max_value = 255;  //65535;


void motor(int speedLeft, int speedRight) {
  system();
  if (speedLeft > 255) speedLeft = 255;
  else if (speedLeft < -255) speedLeft = -255;
  if (speedRight > 255) speedRight = 255;
  else if (speedRight < -255) speedRight = -255;

  // invert motors when needed
  int Speed[2] = { speedLeft, speedRight };
  // Serial.print(speedLeft);
  // Serial.print("");
  // Serial.println(speedRight);
  // for loop to run each motors individually
  for (int x = 0; x < sizeof(motorPWM); x++) {

    if (Speed[x] > 0) {
      // Forward
      // digitalWrite(motorPin1[x], HIGH);
      // digitalWrite(motorPin2[x], LOW);
      ledcWrite(motorChannel[x], abs(Speed[x]));
    } else if (Speed[x] < 0) {
      // Reverse
      // digitalWrite(motorPin1[x], LOW);
      // digitalWrite(motorPin2[x], HIGH);
      ledcWrite(motorChannel[x], 0);
    } else {
      //Stop
      // digitalWrite(motorPin1[x], HIGH);
      // digitalWrite(motorPin2[x], HIGH);
      ledcWrite(motorChannel[x], 0);
    }
  }
}



void Motor_setup() {

  for (int x = 0; x < sizeof(motorPWM); x++) {
    ledcSetup(motorChannel[x], 5000, PWM_resolution);  // 50 Hz, 16-bit width
    ledcAttachPin(motorPWM[x], motorChannel[x]);

    // pinMode(motorPin1[x], OUTPUT);
  }
  // activate Standby pin
  //  for (int x = 0; x < sizeof(motorSTBY); x++)
  //  {
  //    pinMode(motorSTBY[x], OUTPUT);
  //  }

  // for (int x = 0; x < sizeof(motorPin1); x++)
  // {
  //   ledcSetup(motorChannel[x], 5000, PWM_resolution);
  //   ledcAttachPin(motorPWM[x] , motorChannel[x] );
  //   ledcWrite(motorChannel[x] , 0);

  //   pinMode(motorPin1[x], OUTPUT);


  /*
    // set direction pins
    pinMode(motorPin1[x], OUTPUT);
    pinMode(motorPin2[x], OUTPUT);

    //set pwm pins
    ledcSetup(motorChannel[x], 5000, PWM_resolution);
    ledcAttachPin(motorPWM[x] , motorChannel[x] );

    digitalWrite(motorPin1[x], HIGH);
    digitalWrite(motorPin2[x], HIGH);
    ledcWrite(motorChannel[x] , 0);
    */
  // }
}
