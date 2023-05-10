#include "Adafruit_VL53L0X.h"

Adafruit_VL53L0X lox = Adafruit_VL53L0X();

void tof_setup() {
  // Serial.begin(115200);

  // wait until serial port opens for native USB devices
  // while (! Serial) {
  //   delay(1);
  // }

  Serial.println("Adafruit VL53L0X test.");
  if (!lox.begin()) {
    Serial.println(F("Failed to boot VL53L0X"));
    while(1);
  }
  
    Serial.println(F("Connected to tof "));
  // power
  // Serial.println(F("VL53L0X API Continuous Ranging example\n\n"));

  // start continuous ranging
  lox.startRangeContinuous();
}

void tof_loop() {
  if (lox.isRangeComplete()) {
    int reading = lox.readRange();
    // if(reading<= 5000)
      tof_distance = reading;
    // Serial.println(tof_distance);
  }
}