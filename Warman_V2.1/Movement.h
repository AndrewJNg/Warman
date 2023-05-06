// motor control system for TB6612FNG motor driver
// Also includes PS3 to motor control


//  motor control pin layout   [ Left Front, Right Front , Back Left , Back Right]
// const byte motorPin1[] =  {17, 4, 18, 15};
// const byte motorPWM[] = {5,  16, 19, 2};

// //const byte motorPin1[] =  {15,4, 17, 18};
// //const byte motorPWM[] = {2,  16, 5, 19}

// //const byte motorPin2[] = {22, 2, 12, 17};
// const byte motorChannel[] = {6, 7, 8, 9};
// //const byte motorSTBY[] = {27};

const byte motorPin1[] = { 26, 12, 17, 2 };
const byte motorPin2[] = { 27, 14, 16, 4 };
const byte motorPWM[] = { 25, 13, 5, 15 };

// const byte motorChannel[] = { 0, 1, 2, 3 };
const byte motorChannel[] = { 6, 7, 8, 9 };
// const byte motorSTBY[] = { 27 };

int motor_Speed[] = { 0, 0, 0, 0 };
const byte PWM_resolution = 8;             //16;
const int PWM_resolution_max_value = 255;  //65536;

void motor(int Speed[]) {
  system();
  //n Traslate Speed from  -255 to 255 or higher resolution to motor driver
  // for (int x = 0; x < sizeof(motorSTBY); x++) {
  //   digitalWrite(motorSTBY[x], HIGH);
  // }

  for (int x = 0; x < sizeof(motorPin1); x++) {
    if (Speed[x] > PWM_resolution_max_value) Speed[x] = PWM_resolution_max_value;
    else if (Speed[x] < -PWM_resolution_max_value) Speed[x] = -PWM_resolution_max_value;

    if (Speed[x] > 0) {  // Forward
      digitalWrite(motorPin1[x], LOW);
      digitalWrite(motorPin2[x], HIGH);
      ledcWrite(motorChannel[x], abs(Speed[x]));
    } else if (Speed[x] < 0) {  // Reverse
      digitalWrite(motorPin1[x], HIGH);
      digitalWrite(motorPin2[x], LOW);
      ledcWrite(motorChannel[x], abs(Speed[x]));
    } else {  //Stop
      digitalWrite(motorPin1[x], HIGH);
      digitalWrite(motorPin2[x], HIGH);
      ledcWrite(motorChannel[x], 0);
    }
  }
}

void PS3_move(int LX, int LY, int RX, int RY) {
  // angle from 0 to 2*pi
  float angle = PS3_LeftAnalogStickAngle(LX, LY);

  //Speed  (range of 0 to 100)
  float Speed_total_percent = PS3_LeftAnalogStickSpeed(LX, LY);

  // turn speed for rotational movement
  float turn_Speed = 0;

  // filter to ignore inputs below 15 from controller for rotation only
  if (abs(RX) > 15) turn_Speed = map(RX, -128, 127, -PWM_resolution_max_value, PWM_resolution_max_value);



  /*Mecanum wheels programming instructions found on this website
    https://seamonsters-2605.github.io/archive/mecanum
    similar principle is applied, however the direction of motion was changed (clockwise being positive angle)
  */

  motor_Speed[0] = map(sin(angle + (1 * PI) / 4) * Speed_total_percent * 100, -10000, 10000, -PWM_resolution_max_value, PWM_resolution_max_value);
  motor_Speed[1] = map(sin(angle + (3 * PI) / 4) * Speed_total_percent * 100, -10000, 10000, -PWM_resolution_max_value, PWM_resolution_max_value);
  motor_Speed[2] = map(sin(angle + (3 * PI) / 4) * Speed_total_percent * 100, -10000, 10000, -PWM_resolution_max_value, PWM_resolution_max_value);
  motor_Speed[3] = map(sin(angle + (1 * PI) / 4) * Speed_total_percent * 100, -10000, 10000, -PWM_resolution_max_value, PWM_resolution_max_value);

  //apply turn Speed to allow control using right analog stick
  motor_Speed[0] = motor_Speed[0] + turn_Speed;
  motor_Speed[1] = motor_Speed[1] - turn_Speed;
  motor_Speed[2] = motor_Speed[2] + turn_Speed;
  motor_Speed[3] = motor_Speed[3] - turn_Speed;

  // run all motors with according speeds
  motor(motor_Speed);
}

void Movement_setup() {
  // set motor pins as output
  for (int x = 0; x < sizeof(motorPin1); x++) {
    pinMode(motorPin1[x], OUTPUT);
    pinMode(motorPin2[x], OUTPUT);
    ledcSetup(motorChannel[x], 5000, PWM_resolution);  // up to 16 channels
    ledcAttachPin(motorPWM[x], motorChannel[x]);
  }
  
}
void mecanum_move(int Speed, float angle_degree, int turn_Speed) {
  // angle from 0 to 2*pi
  float angle = angle_degree* PI / 180;

  //Speed  (range of 0 to 100)
  float Speed_total_percent = Speed;

  // turn speed for rotational movement
  // float turn_Speed = 0;

  // filter to ignore inputs below 15 from controller for rotation only
  if (abs(RX) > 15) turn_Speed = map(RX, -128, 127, -PWM_resolution_max_value, PWM_resolution_max_value);



  /*Mecanum wheels programming instructions found on this website
    https://seamonsters-2605.github.io/archive/mecanum
    similar principle is applied, however the direction of motion was changed (clockwise being positive angle)
  */

  motor_Speed[0] = map(sin(angle + (1 * PI) / 4) * Speed_total_percent * 100, -10000, 10000, -PWM_resolution_max_value, PWM_resolution_max_value);
  motor_Speed[1] = map(sin(angle + (3 * PI) / 4) * Speed_total_percent * 100, -10000, 10000, -PWM_resolution_max_value, PWM_resolution_max_value);
  motor_Speed[2] = map(sin(angle + (3 * PI) / 4) * Speed_total_percent * 100, -10000, 10000, -PWM_resolution_max_value, PWM_resolution_max_value);
  motor_Speed[3] = map(sin(angle + (1 * PI) / 4) * Speed_total_percent * 100, -10000, 10000, -PWM_resolution_max_value, PWM_resolution_max_value);

  //apply turn Speed to allow control using right analog stick
  motor_Speed[0] = motor_Speed[0] + turn_Speed;
  motor_Speed[1] = motor_Speed[1] - turn_Speed;
  motor_Speed[2] = motor_Speed[2] + turn_Speed;
  motor_Speed[3] = motor_Speed[3] - turn_Speed;

  // run all motors with according speeds
  motor(motor_Speed);
  system();
}
