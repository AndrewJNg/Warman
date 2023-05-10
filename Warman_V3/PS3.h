// Allows control using a PS3 controller by using the github library by jvpernis
//https://github.com/jvpernis/esp32-ps3

// esp32 board manager - V1.0.6
// ps3 controller host by Jeffrey van Pernis - V1.1.0
#include <Ps3Controller.h>

// PS3 Status Variables
int player = 2;
int battery = 0;

// PS3 Control Variables (Movement)
int stick_LX;
int stick_LY;
int stick_RX;
int stick_RY;

// PS3 Control Variables (Servos)
int Triangle = 0;  // 0 = not pressed (hold position), 1 = pressed (Request Scoop)
int Square = 0;    // 0 = not pressed (hold position), 1 = pressed (Request Ball Release)
int L1 = 0;        // 0 = not pressed (hold position), 1 = pressed (Tighten Gripper)
int R1 = 0;        // 0 = not pressed (hold position), 1 = pressed (Loosen Gripper)

// PS3 Control Variables (Stepper)
int L2 = 0;  // 0 = not pressed (hold position), 1 = pressed (Move Gripper Up)
int R2 = 0;  // 0 = not pressed (hold position), 1 = pressed (Move Gripper Down)

// Internal Variable Changes when PS3 Control changes
void notify() {

  // Movement
  if (abs(Ps3.event.analog_changed.stick.lx) + abs(Ps3.event.analog_changed.stick.ly) > 2) {
    stick_LX = Ps3.data.analog.stick.lx;
    stick_LY = -Ps3.data.analog.stick.ly;
  }
  if (abs(Ps3.event.analog_changed.stick.rx) + abs(Ps3.event.analog_changed.stick.ry) > 2) {
    stick_RX = Ps3.data.analog.stick.rx;
    stick_RY = -Ps3.data.analog.stick.ry;
  }

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // PID tuning
  /*
  if ( Ps3.event.button_down.up )
  {
    KpLeft = KpLeft + 10;
    KpRight = KpRight + 10;
  }
  if ( Ps3.event.button_down.down )
  {
    KpLeft = KpLeft - 10;
    KpRight = KpRight - 10;
  }
 

  if ( Ps3.event.button_down.right ) 
  {
    KiLeft = KiLeft + 1;
    KiRight = KiRight + 1;
  }
  if ( Ps3.event.button_down.left ) 
  {
    KiLeft = KiLeft - 1;
    KiRight = KiRight - 1;
  }
 
  if ( Ps3.event.button_down.circle )
  {
    KdLeft = KdLeft + 1;
    KdRight = KdRight + 1;
  }
  if ( Ps3.event.button_down.square )
  {
    KdLeft = KdLeft - 1;
    KdRight = KdRight - 1;
  }
  */


  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // current speed modifiers
  // if ( Ps3.event.button_down.up )speedDiff = speedDiff + 5;
  // if ( Ps3.event.button_down.down )speedDiff = speedDiff - 5;

  // if (Ps3.event.button_down.r1) averageSpeed = averageSpeed + 5;
  // if (Ps3.event.button_down.l1) averageSpeed = averageSpeed - 5;

  if (Ps3.event.button_down.l3) {
    averageSpeed = squashSpeed;  // set squash speed for flywheel
    speedDiff = squashDiff;
  }
  if (Ps3.event.button_down.r3) {
    averageSpeed = tennisSpeed;  // set tennis speed for flywheel
    speedDiff = tennisDiff;
  }


  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // tennis and squash settings
  if (Ps3.event.button_down.up) {
    squashSpeed = squashSpeed + 10;

    averageSpeed = squashSpeed;  // set squash speed for flywheel
    speedDiff = squashDiff;
    store_memory();
  }
  if (Ps3.event.button_down.down) {
    squashSpeed = squashSpeed - 10;
    
    averageSpeed = squashSpeed;  // set squash speed for flywheel
    speedDiff = squashDiff;
    store_memory();
  }
  if (Ps3.event.button_down.left) {
    squashDiff = squashDiff - 10;
    
    averageSpeed = squashSpeed;  // set squash speed for flywheel
    speedDiff = squashDiff;
    store_memory();
  }
  if (Ps3.event.button_down.right) {
    squashDiff = squashDiff + 10;
    
    averageSpeed = squashSpeed;  // set squash speed for flywheel
    speedDiff = squashDiff;
    store_memory();
  }


  if (Ps3.event.button_down.triangle) {
    tennisSpeed = tennisSpeed + 10;
    
    averageSpeed = tennisSpeed;  // set tennis speed for flywheel
    speedDiff = tennisDiff;
    store_memory();
  }
  if (Ps3.event.button_down.cross) {
    tennisSpeed = tennisSpeed - 10;
    
    averageSpeed = tennisSpeed;  // set tennis speed for flywheel
    speedDiff = tennisDiff;
    store_memory();
  }

  if (Ps3.event.button_down.square) {
    tennisDiff = tennisDiff - 10;
    
    averageSpeed = tennisSpeed;  // set tennis speed for flywheel
    speedDiff = tennisDiff;
    store_memory();
  }
  if (Ps3.event.button_down.circle) {
    tennisDiff = tennisDiff + 10;
    
    averageSpeed = tennisSpeed;  // set tennis speed for flywheel
    speedDiff = tennisDiff;
    store_memory();
  }
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // Trigger event buttons
  if (Ps3.event.button_down.start) Start = 1;
  if (Ps3.event.button_down.l1) Start2 = 1;
  if (Ps3.event.button_down.r1) Start3 = 1;

  if (Ps3.event.button_down.select) {
    averageSpeed = 0;

    // lift arm down
    // setPos(0, 180);
  }
}

void onConnect() {
  Serial.println("Connected.");
  Ps3.setPlayer(2);
}

void PS3_setup() {
  Ps3.attach(notify);
  Ps3.attachOnConnect(onConnect);
  // Ps3.begin("03:02:03:04:05:06");
  // Ps3.begin("aa:aa:aa:aa:aa:aa");
  // Ps3.begin("bb:bb:bb:bb:bb:bb");
  Ps3.begin("cc:cc:cc:cc:cc:cc");
  // Ps3.begin("dd:dd:dd:dd:dd:dd");
}

// Helper Function to Calculate Analog Stick Angle
float PS3_LeftAnalogStickAngle(int LX, int LY) {
  float LX_vector = map(LX, -128, 127, -10000, 10000) / 100;
  float LY_vector = map(LY, -127, 128, -10000, 10000) / 100;
  float angle;
  if (LY_vector == 0 && LX_vector > 0) angle = PI / 2;
  else if (LY_vector == 0 && LX_vector < 0) angle = 3 * PI / 2;
  else if (LY_vector == 0 && LX_vector == 0) angle = 0;
  else angle = atan(abs(LX_vector) / abs(LY_vector));

  if (LX_vector > 0 && LY_vector > 0) angle = angle;
  else if (LX_vector > 0 && LY_vector < 0) angle = PI - angle;
  else if (LX_vector < 0 && LY_vector < 0) angle = PI + angle;
  else if (LX_vector < 0 && LY_vector > 0) angle = 2 * PI - angle;

  return angle;
}

// Helper Function to Calculate Analog Stick Speed
float PS3_LeftAnalogStickSpeed(int LX, int LY) {
  float LX_vector = 0;
  float LY_vector = 0;
  if (abs(LX) > 15) LX_vector = map(LX, -128, 127, -10000, 10000) / 100;
  //  else LX_vector = 0;
  if (abs(LY) > 15) LY_vector = map(LY, -127, 128, -10000, 10000) / 100;
  //  else LY_vector = 0;
  float Speed = sqrt(LX_vector * LX_vector + LY_vector * LY_vector);
  if (Speed > 100) Speed = 100;

  return Speed;
}
