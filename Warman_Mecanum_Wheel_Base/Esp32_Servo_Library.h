

static const int freq = 50; // pwn duty cycle at 50Hz 

static const int minDeg = 0;
static const int maxDeg = 180;

const int bitWidth = 16;
static const int minBit = 1638; // corresponding to 0 deg
static const int maxBit = 7864; // corresponding to 180 deg

//////////////////////////////////////////////////////////////////////////////////////////////////////////////


// setup servo pins for one servo pin
void PWMsetup(int channel, int pin) {
  ledcSetup(channel, freq,  bitWidth); // 50 Hz, 16-bit width
  ledcAttachPin(pin, channel);   // GPIO pin assigned to channel
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////


int servoPulse;

// Creates a PWM signal by mapping the position of the servo in degrees to 
void setPos(int channel, int deg) {
  servoPulse = map(deg, minDeg, maxDeg, minBit, maxBit);
  ledcWrite(channel, servoPulse);   
}