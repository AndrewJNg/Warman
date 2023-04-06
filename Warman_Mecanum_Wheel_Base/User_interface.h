
bool start_button();

class button {
public:
  unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
  boolean lastButtonState = false;     // the previous reading from the input pin
  int buttonState = 0;                 // the current reading from the input pin
  unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

  //    int counter = 0;
  int count(bool reading, int* subject, int increment);
};

//  button function to allow debouncing of inputs and incrementing to a value
// expample,
// (mode = subject=0, increment=1)
// it will take the input value from a button (either 0 or 1), filter the result and return it by modying the passed in mode pointer
int button::count(bool reading, int* subject, int increment) {
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }
  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;

      if (buttonState == HIGH) {
        *subject = *subject + increment;
      }
    }
  }
  lastButtonState = reading;
  return *subject;
}


button start_button_menu;

bool start_button() {
  // int sum = 0;
  // for (int i = 0; i < 3; i++)
  // {
  //   if (IRVal[i] > 3000) sum++;

  // }
  //  Serial.print(sum);
  //  Serial.print("  ");
  // if (sum == 3)return true;
  // else return false;
  if (digitalRead(35) == 1)
    return true;

  else
    return false;
}