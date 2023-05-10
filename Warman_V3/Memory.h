#include <Preferences.h>
Preferences preferences;

// int IRVal[4] = { 0, 0, 0, 0 };

// int squashSpeed = 1200;
// int squashDiff = 0;
// int tennisSpeed = 2150;
// int tennisDiff = 0;

void read_memory() {
  preferences.begin("FlywheelData", false);

  // retrieve saved data
  squashSpeed = preferences.getUInt("squashSpeed", 0);
  squashDiff = preferences.getUInt("squashDiff", 0);
  tennisSpeed = preferences.getUInt("tennisSpeed", 0);
  tennisDiff = preferences.getUInt("tennisDiff", 0);
  preferences.end();

}

void store_memory()
{
  preferences.begin("FlywheelData", false);
  preferences.putUInt("squashSpeed", squashSpeed);
  preferences.putUInt("squashDiff", squashDiff);
  preferences.putUInt("tennisSpeed", tennisSpeed);
  preferences.putUInt("tennisDiff", tennisDiff);
  preferences.end();
}
