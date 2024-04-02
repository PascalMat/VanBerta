// Rotary Encoder Inputs
#define CLK 2
#define ROT_GND 3
#define DT 4
#define SW 7
#define SW_GND 8

const int analogOutPin = 1;

unsigned int counter = 0;
int currentStateCLK;
int lastStateCLK;
String currentDir = "";
unsigned long lastButtonPress = 0;
bool belowThreshold = true;
unsigned int threshold = 5;
unsigned long lastStateChange = 0;

void setup() {
  // Set encoder pins as inputs
  pinMode(CLK, INPUT_PULLUP);
  pinMode(DT, INPUT_PULLUP);
  pinMode(SW, INPUT_PULLUP);
  pinMode(SW_GND, OUTPUT);
  pinMode(ROT_GND, OUTPUT);

  // Setup Serial Monitor
  Serial.begin(9600);

  // Read the initial state of CLK
  lastStateCLK = digitalRead(CLK);
  digitalWrite(SW_GND, 0);
  digitalWrite(ROT_GND, 0);
}

void loop() {
  // Read the current state of CLK
  currentStateCLK = digitalRead(CLK);

  // If last and current state of CLK are different, then pulse occurred
  // React to only 1 state change to avoid double count
  if (currentStateCLK != lastStateCLK && currentStateCLK == 1) {
    // If the DT state is different than the CLK state then
    // the encoder is rotating CCW so decrement
    if (digitalRead(DT) != currentStateCLK) {
      if (counter > 0) {
        counter--;
        currentDir = "CCW";
      }
    } else {
      if (counter < 25) {
        // Encoder is rotating CW so increment
        counter++;
        currentDir = "CW";
      }
    }

    lastStateChange = millis();

    Serial.print("Direction: ");
    Serial.print(currentDir);
    Serial.print(" | Counter: ");
    Serial.println(counter);

    if (counter > threshold) {
      analogWrite(analogOutPin, counter * 10);
    }
  }

  // Remember last CLK state
  lastStateCLK = currentStateCLK;

  // Read the button state
  int btnState = digitalRead(SW);

  // If we detect LOW signal, button is pressed
  if (btnState == LOW) {
    // if 50ms have passed since last LOW pulse, it means that the
    // button has been pressed, released and pressed again
    if (millis() - lastButtonPress > 50) {
      Serial.println("Button pressed!");
      counter = 0;
    }

    // Remember last button press event
    lastButtonPress = millis();
  }

  // Put in a slight delay to help debounce the reading
  delay(10);
}
