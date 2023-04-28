
float v1Filt = 0;
float v1Prev = 0;
float v2Filt = 0;
float v2Prev = 0;

int prevCountLeft = 0;
int prevCountRight = 0;
// unsigned long prevMicro = 0;
unsigned long EncprevMicro = 0;

void rpmMove(float target_rpm_left, float target_rpm_right) {

  enc_motor_update();
  int interval = 1000;
  unsigned long timeDIff = micros() - EncprevMicro;


  if (timeDIff >= interval) {

    float rpmLeft = (float)((currAngle[0] - prevCountLeft) * (1.0 / ((float)timeDIff * 1e-6)) * 60) / 4096;
    float rpmRight = (float)((currAngle[1] - prevCountRight) * (1.0 / ((float)timeDIff * 1e-6)) * 60) / 4096;

    // Serial.print(currAngle[0] - prevCountLeft);
    // Serial.print("  ");
    // Serial.print(currAngle[1] - prevCountRight);
    // Serial.print("  ");

    prevCountLeft = currAngle[0];
    prevCountRight = currAngle[1];
    EncprevMicro = micros();

    float v1 = rpmLeft;

    //        v1Filt =0.99937188*v1Filt + 0.00031406*v1+0.00031406*v1Prev;  //0.1 hz
    //        v1Filt =0.99686333*v1Filt + 0.00156833*v1+0.00156833*v1Prev;  //0.5 hz
    v1Filt = 0.98751209 * v1Filt + 0.00624395 * v1 + 0.00624395 * v1Prev;  //2 hz
    //        v1Filt = 0.854*v1Filt + 0.0728*v1+0.0828*v1Prev;            //5 hz
    v1Prev = v1;

    float v2 = rpmRight;
    v2Filt = 0.98751209 * v2Filt + 0.00624395 * v2 + 0.00624395 * v2Prev;
    v2Prev = v2;

    InputRpmLeft = v1Filt;
    rpmSetLeft = target_rpm_left;
    leftMotorVel.SetTunings(KpLeft, KiLeft, KdLeft);
    leftMotorVel.Compute();
    int SpeedLeft = OutputLeftMotor;

    InputRpmRight = v2Filt;
    rpmSetRight = target_rpm_right;
    rightMotorVel.SetTunings(KpRight, KiRight, KdRight);
    rightMotorVel.Compute();
    int SpeedRight = OutputRightMotor;

    currSpeedEncLeft = InputRpmLeft;
    currSpeedEncRight = InputRpmRight;

    flywheelMotor(SpeedLeft, SpeedRight);
    // motor(0, 0);


    // Serial.print(SpeedLeft, 0);
    // Serial.print("\t");
    // Serial.print(SpeedRight, 0);
    // Serial.print("\t");

    // Serial.print(currAngle[0], 0);
    // Serial.print("\t");
    // Serial.print(currAngle[1], 0);
    // Serial.print("\t");
    // Serial.print(rpmLeft, 0);
    // Serial.print("\t");
    // Serial.print(rpmRight, 0);
    // Serial.print("\t");
    // Serial.print(target_rpm_left, 0);
    // Serial.print("\t");
    // Serial.print(target_rpm_right, 0);
    // Serial.print("\t");
    // Serial.print(0, 0);
    // Serial.print("\t");

    // Serial.print(currAngle[0]);
    // Serial.print("\t");

    // Serial.print("KpLeft: ");
    // Serial.print(KpLeft);
    // Serial.print("\t");
    // Serial.print("KiLeft: ");
    // Serial.print(KiLeft);
    // Serial.print("\t");
    // Serial.print("KdLeft: ");
    // Serial.print(KdLeft,5);

    // Serial.print("\t");


    
    // Serial.print( InputRpmLeft, 0);
    // Serial.print("\t");
    // Serial.print( InputRpmRight, 0);
    // Serial.print("\t");
    // Serial.println();

    
  }
}

// void straight(int Speed, long targetDistance) {
//   double initLeftDist = encDistance(0);
//   double initRightDist = encDistance(1);
//   double currLeftDist = 0;
//   double currRightDist = 0;

//   //  Serial.print(initLeftDist);
//   //  Serial.print("  ");
//   //  Serial.print(initRightDist );
//   //  Serial.print("  ");
//   //  Serial.print(currLeftDist);
//   //  Serial.print("  ");
//   //  Serial.print(currRightDist);
//   //  error_old = 0;

//   currLeftDist = encDistance(0) - initLeftDist;
//   currRightDist = encDistance(1) - initRightDist;


//   while ((((currLeftDist + currRightDist) / 2) < targetDistance) && Start == true && (IRVal[0] < 3500 && IRVal[5] < 3500)) {
//     currLeftDist = encDistance(0) - initLeftDist;
//     currRightDist = encDistance(1) - initRightDist;


//     // both walls are present
//     if (IRVal[2] >= 1500 && IRVal[3] >= 1500) {
//       system();
//       //    int diff = IRVal[4] - IRVal[1];
//       int diff = (leftWall - IRVal[2]) + (IRVal[3] - rightWall);
//       //      int diff = (IRVal[3] -rightWall)- (leftWall - IRVal[2]);
//       if (diff < -1500) diff = -1500;
//       else if (diff > 1500) diff = 1500;
//       int speedDiff = map(diff, -1500, 1500, -70, 70);
//       int Speed = 100;
//       motor(Speed - speedDiff, Speed + speedDiff);
//     } else if (IRVal[2] >= 1500 && IRVal[3] < 1500) {
//       system();
//       //    int diff = IRVal[4] - IRVal[1];
//       int diff = (leftWall - IRVal[2]);
//       //      int diff = (leftWall - IRVal[2]) +  (IRVal[3] - rightWall);
//       //      int diff = (IRVal[3] -rightWall)- (leftWall - IRVal[2]);
//       if (diff < -1500) diff = -1500;
//       else if (diff > 1500) diff = 1500;
//       int speedDiff = map(diff, -1500, 1500, -70, 70);
//       int Speed = 100;
//       motor(Speed - speedDiff, Speed + speedDiff);
//     } else if (IRVal[2] < 1500 && IRVal[3] >= 1500) {
//       system();
//       //    int diff = IRVal[4] - IRVal[1];
//       int diff = (IRVal[3] - rightWall);
//       //      int diff = (leftWall - IRVal[2]) +  (IRVal[3] - rightWall);
//       //      int diff = (IRVal[3] -rightWall)- (leftWall - IRVal[2]);
//       if (diff < -1500) diff = -1500;
//       else if (diff > 1500) diff = 1500;
//       int speedDiff = map(diff, -1500, 1500, -70, 70);
//       int Speed = 100;
//       motor(Speed - speedDiff, Speed + speedDiff);
//     } else {
//       //    double error =  currLeftDist - currRightDist ;
//       //    double corr_value =  kp * error  + kd * (error - error_old); //PD control
//       //    error_old = error;

//       straightInput = currRightDist - currLeftDist;
//       straightSetpoint = 0;
//       straightPID.SetTunings(straightKp, straightKi, straightKd);
//       straightPID.Compute();
//       int corr_value = straightOutput;
//       //    rpmMove(Speed, -Speed);

//       //    rpmMove(Speed - corr_value, Speed + corr_value);
//       motor(Speed - corr_value, Speed + corr_value);
//       system();
//     }

//     /*
//     straightInput =  currRightDist - currLeftDist ;
//       straightSetpoint =  0;
//       straightPID.SetTunings(straightKp, straightKi, straightKd);
//       straightPID.Compute();
//       int corr_value = straightOutput;
//       motor(Speed - corr_value, Speed + corr_value);
//       system();
//       */
//   }
// }
