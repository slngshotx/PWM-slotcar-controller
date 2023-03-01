/*
 * Arduino slot car controller V3
 *
 * Model ESP 32
 *
 * Copyright 2023 Slingshot Software All rights reserved.
 *
 * PWM controller 3 software for use on a ESP 32 chip
 *
 */

// Various USB debug settings
#define __DEBUG__
//#define __DEBUG_ALL_VALUES__

// Pins used
// Controller value analog pin (potentiometer)
const int             iControllerPin = 34;

// Pins used to control the motor output
const int             iFwdPowerPin = 32;
const int             iPowerOnPin = 33;

// Values read in from controls
int                   iControllerValue = 0;           // value read from the controller pot
int                   iPowerCurve = 0;                // defines the power curve to use

// Values used in code
int                   iLoopIterations = 0;            // Count times we have looped

// Setting PWM properties
const int             freq = 20000;
const int             pwmChannel = 0;
const int             resolution = 8;

// the setup routine runs once when you press reset:
void setup() {
#if defined(__DEBUG__) || defined(__DEBUG_ALL_VALUES__)
  Serial.begin(115200);
#endif

  pinMode(iControllerPin, INPUT_PULLUP);
  pinMode(iFwdPowerPin, OUTPUT);
  pinMode(iPowerOnPin, OUTPUT);

  // Setup the PWM output channel and attach it to the output pin
  ledcSetup(pwmChannel, freq, resolution);
  ledcAttachPin(iFwdPowerPin, pwmChannel);
}

// the loop routine runs over and over again forever:
void loop() {
  readControllerValue();
  applyTrackPower();

  if (++iLoopIterations >= 10000) {
    iLoopIterations = 0;
    // print out the value you read:
#if defined(__DEBUG__)
    printDebugValues();
#endif
  }
}

/* 
 * ReadControllerValue
 *
 * Read the value of the trigger, analog read will return 0 - 4095
 * This can be converted to duty value of 0 - 255 applying any curve value
 */
void readControllerValue()
{
   // read the input on the controller pin
  int iReadValue = analogRead(iControllerPin);

  // Now apply any curve we are using
  switch (iPowerCurve) {
    // Default is to use straight line
    case 0:
    default:
      iControllerValue = map(iReadValue, 0, 4095, 0, 255);  
      break;
  }
}

/*
 * After checking the various controller settings apply the track power
 * at the correct level using PWM
 */
void applyTrackPower()
{
  // If controller value > 0 then apply power if it's 0 then turn power off
  if (iControllerValue > 0) {
    digitalWrite(iPowerOnPin, HIGH);
    ledcWrite(pwmChannel, iControllerValue);   
  } else
  {
    digitalWrite(iPowerOnPin, LOW); 
  }
}
/* 
/*
 * Only used when __DEBUG__ is enabled to display
 */
#if defined(__DEBUG__)
void printDebugValues()
{
  // print the results to the serial monitor:
  Serial.print("Input: " );
  Serial.println(iControllerValue);
}
#endif
