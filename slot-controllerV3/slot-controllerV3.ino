/*
 * Arduino slot car controller V3
 *
 * Model ESP32 - main module
 * Main module controls loop etc.
 *
 * Copyright 2023 Slingshot Software All rights reserved.
 *
 * PWM controller 3 software for use on a ESP32 chip
 *
 */
#include <WebServer.h>

#define __DEF_PWM_FREQUENCY__ 20000
#define __DEF_BRAKE_SETTING__ 254
#define __DEF_THROTTLE_SETTING__ 100

#define __DEBUG__
//#define __DEBUG_VALUES__

// Pins used
// Controller value analog pin (potentiometer)
const int iControllerPin = 34;

// Pins used to control the motor output
const int iFwdPowerPin = 32;
const int iPowerOnPin = 33;

// Values read in from controls
int iControllerValue = 0;  // value read from the controller pot
int iPowerCurve = 0;       // defines the power curve to use

// Values used in code
bool bWebServerRunning = false;  // If webserver is running
int iLoopIterations = 0;         // Count times we have looped

// Webserver if used
WebServer server(80);

// Setting various properties
int iPWMFrequency = __DEF_PWM_FREQUENCY__;  // Can be changed to any freq, suggest 5000 - 25000
int iBrakeSetting = __DEF_BRAKE_SETTING__;
int iThrottleSetting = __DEF_THROTTLE_SETTING__;
const int iPWMChannel = 0;
const int iPWMResolution = 8;

// the setup routine runs once when you press reset:
void setup() {
#if defined(__DEBUG__) || defined(__DEBUG_VALUES__)
  Serial.begin(115200);
#endif

  // Read the preferences
  getPreferences();

  // Setup I/O pins
  pinMode(iControllerPin, INPUT_PULLUP);
  pinMode(iFwdPowerPin, OUTPUT);
  pinMode(iPowerOnPin, OUTPUT);

  setupPWM();
}

// the loop routine runs over and over again forever:
void loop() {
  readControllerValue();
  applyTrackPower();

  if (bWebServerRunning) {
    server.handleClient();
  }

  if (++iLoopIterations >= 10000) {
    // Only start the webserver when we have time.
    if (iControllerValue == 0 && !bWebServerRunning) {
      bWebServerRunning = true;
      startWebserver();
    }
    iLoopIterations = 0;
    // print out the value you read:
#if defined(__DEBUG_VALUES__)
    printDebugValues();
#endif
  }
}

void setupPWM()
{
  // Setup the PWM output channel and attach it to the output pin
  ledcSetup(iPWMChannel, iPWMFrequency, iPWMResolution);
  ledcAttachPin(iFwdPowerPin, iPWMChannel);
}

/* 
 * ReadControllerValue
 *
 * Read the value of the trigger, analog read will return 0 - 4095
 * This can be converted to duty value of 0 - 254 applying any curve value
 */
void readControllerValue() {
  // read the input on the controller pin
  int iReadValue = analogRead(iControllerPin);

  // Now apply any curve we are using
  switch (iPowerCurve) {
    // Default is to use straight line
    case 0:
    default:
      iControllerValue = map(iReadValue, 0, 4095, 0, 254);
      break;
  }
  // Apply any throttle limit setting so we can reduce top end power
  if (iThrottleSetting < 100)
  {
    // Simple calculation reduce controller value by iThrottleSetting %
    // i.e. 50% power would be a max of 254 * (90 / 100) = 228
    iControllerValue = min(iControllerValue, (int)(iControllerValue * (iThrottleSetting / 100.0)));
  }
}

/*
 * After checking the various controller settings apply the track power
 * at the correct level using PWM
 */
void applyTrackPower() {
  // If controller value > 0 then apply power if it's 0 then brake
  ledcWrite(iPWMChannel, iControllerValue);
  if (iControllerValue > 0) {
    analogWrite(iPowerOnPin, 254);
  } else {
    // Vary the brakes using the supplied setting, this will PWM the on/off pin
    analogWrite(iPowerOnPin, iBrakeSetting);
  }
}
/* 
 * Only used when __DEBUG__ is enabled to display
 */
#if defined(__DEBUG_VALUES__)
void printDebugValues() {
  // print the results to the serial monitor:
  Serial.print("Input: ");
  Serial.println(iControllerValue);
}
#endif
