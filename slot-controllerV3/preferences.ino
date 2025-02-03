/*
 * Arduino slot car controller V3
 *
 * Model ESP32 - Preferences module 
 * Read and write config preferences to flash memory using 
 *
 * Copyright 2023 Slingshot Software All rights reserved.
 *
 * PWM controller 3 software for use on a ESP32 chip
 *
 */
#include <Preferences.h>

/* 
 * getPreferences
 *
 * Read any stored preferences from flash. Defaults will be used if not set.
 */
void getPreferences() {
  Preferences controllerPrefs;

  // Save the presets being used
  controllerPrefs.begin("controlPrefs", false);
  // After boot get the last saved preset
  if (iSelectedPreset < 0) {
    iSelectedPreset = controllerPrefs.getInt("sSelectedPreset", 1);
  }

  controllerPrefs.putInt("sSelectedPreset", iSelectedPreset);
  controllerPrefs.end();

  String prefs = "preset" + iSelectedPreset;
  controllerPrefs.begin(prefs.c_str(), true);
  iPWMFrequency = controllerPrefs.getInt("sPWMFreq", __DEF_PWM_FREQUENCY__);
  iBrakeSetting = controllerPrefs.getInt("sBrake", __DEF_BRAKE_SETTING__);
  iThrottleSetting = controllerPrefs.getInt("sThrottle", __DEF_THROTTLE_SETTING__);
  iTCSSetting = controllerPrefs.getInt("sTCS", __DEF_TCS_SETTING__);
  iTCSStartSetting = controllerPrefs.getInt("sTCSStart", __DEF_TCS_START_SETTING__);
  iTCSStopSetting = controllerPrefs.getInt("sTCSStop", __DEF_TCS_STOP_SETTING__);
  iCoastSetting = controllerPrefs.getInt("sCoast", __DEF_COAST_SETTING__);
  fCurveVal = controllerPrefs.getFloat("sCurveVal", __DEF_CURVEVAL__);


#if defined(__DEBUG__)
  Serial.println("Read preferences");
  printPrefValues();
#endif
}

/* 
 * putPreferences
 *
 * Save the preferences to persistent store for next time.
 */
void putPreferences() {
  Preferences controllerPrefs;

  // Get value from appropriate preset
  controllerPrefs.begin("controlPrefs", true);
  iSelectedPreset = controllerPrefs.getInt("sSelectedPreset", 1);
  controllerPrefs.end();

  String prefs = "preset" + iSelectedPreset;
  controllerPrefs.begin(prefs.c_str(), false);

  controllerPrefs.putInt("sPWMFreq", iPWMFrequency);
  controllerPrefs.putInt("sBrake", iBrakeSetting);
  controllerPrefs.putInt("sThrottle", iThrottleSetting);
  controllerPrefs.putInt("sTCS", iTCSSetting);
  controllerPrefs.putInt("sTCSStart", iTCSStartSetting);
  controllerPrefs.putInt("sTCSStop", iTCSStopSetting);
  controllerPrefs.putInt("sCoast", iCoastSetting);
  controllerPrefs.putFloat("sCurveVal", fCurveVal);
  controllerPrefs.putInt("sSelectedPreset", iSelectedPreset);

  controllerPrefs.end();

#if defined(__DEBUG__)
  Serial.println("Saved preferences");
  printPrefValues();
#endif

  getPreferences();
  // Reset the PWM values if they have changed
  setupPWM();
}

/* 
 * Only used when __DEBUG__ is enabled to display
 */
#if defined(__DEBUG__)
void printPrefValues() {
  // print the results to the serial monitor:
  Serial.print("PWMFrequency: ");
  Serial.println(iPWMFrequency);
  Serial.print("BrakeSetting: ");
  Serial.println(iBrakeSetting);
  Serial.print("ThrottleSetting: ");
  Serial.println(iThrottleSetting);
  Serial.print("TCSSetting: ");
  Serial.println(iTCSSetting);
  Serial.print("TCSStartSetting: ");
  Serial.println(iTCSStartSetting);
  Serial.print("TCSStopSetting: ");
  Serial.println(iTCSStopSetting);
  Serial.print("CurveVal: ");
  Serial.println(fCurveVal);
}
#endif
