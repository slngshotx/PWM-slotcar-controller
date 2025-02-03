/*
 * Arduino slot car controller V3
 *
 * Model ESP32 - Webserver module 
 * Configure and run a local webserver to change config values. 
 *
 * Copyright 2023 Slingshot Software All rights reserved.
 *
 * PWM controller 3 software for use on a ESP32 chip
 *
 */
#include <WiFi.h>

/* Put your SSID & Password */
const char* ssid = "slingshot-controller";
const char* password = "slingshot";

/* Put IP Address details */
IPAddress local_ip(192, 168, 1, 1);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

/* 
 * startWebserver
 *
 * If required startup the webserver as an access point.
 * Connect a device to ssid/password and browse to local_ip for access to controller settings
 */
void startWebserver() {
#if defined(__DEBUG__)
  Serial.print("Starting webserver: ");
  Serial.println(ssid);
#endif
  WiFi.softAP(ssid, password);
  WiFi.softAPConfig(local_ip, gateway, subnet);
  delay(100);
#if defined(__DEBUG__)
  Serial.print("Webserver started: ");
  Serial.println(local_ip);
#endif

  server.on("/", handleOnConnect);
  server.on("/put", handleSubmit);
  server.on("/preset", handlePresetSelect);
  server.onNotFound(handleNotFound);

  server.begin();
}

void handlePresetSelect() {
    iSelectedPreset = server.arg(0).toInt();
    getPreferences();
    server.send(200, "text/html", SendHTML());
  }

/* 
 * handleSubmit
 *
 * Process webserver data from a submit
 */
void handleSubmit() {
  String sPWMFrequency = server.arg("sPWMFreq");
  String sBrakeSetting = server.arg("sBrake");
  String sThrottleSetting = server.arg("sThrottle");
  String sTCSSetting = server.arg("sTCS");
  String sTCSStartSetting = server.arg("sTCSStart");
  String sTCSStopSetting = server.arg("sTCSStop");
  String sCoastSetting = server.arg("sCoast");
  String sCurveVal = server.arg("sCurveVal");

  if (sPWMFrequency)
    iPWMFrequency = sPWMFrequency.toInt();
  if (sBrakeSetting)
    iBrakeSetting = sBrakeSetting.toInt();
  if (sThrottleSetting)
    iThrottleSetting = sThrottleSetting.toInt();
  if (sTCSSetting)
    iTCSSetting = sTCSSetting.toInt();
  if (sTCSStartSetting)
    iTCSStartSetting = sTCSStartSetting.toInt();
  if (sTCSStopSetting)
    iTCSStopSetting = sTCSStopSetting.toInt();
  if (sTCSStopSetting)
    iCoastSetting = sCoastSetting.toInt();
  if (sCurveVal)
    fCurveVal = sCurveVal.toFloat();

#if defined(__DEBUG__)
  Serial.print("Number of params ");
  Serial.println(server.args());
  Serial.print("PWMFrequency : ");
  Serial.println(iPWMFrequency);
  Serial.print("BrakeSetting : ");
  Serial.println(iBrakeSetting);
  Serial.print("ThrottleSetting : ");
  Serial.println(iThrottleSetting);
  Serial.print("TCSSetting : ");
  Serial.println(iTCSSetting);
  Serial.print("TCSStartSetting : ");
  Serial.println(iTCSStartSetting);
  Serial.print("TCSStopSetting : ");
  Serial.println(iTCSStopSetting);
  Serial.print("Curve Value : ");
  Serial.println(fCurveVal);
#endif

  // Update the preferences
  putPreferences();
  server.send(200, "text/html", SendHTML());
}

/* 
 * handleOnConnect
 *
 * Display the default web page for controller config.
 */
void handleOnConnect() {
#if defined(__DEBUG__)
  Serial.println("Sending HTML");
#endif
  server.send(200, "text/html", SendHTML());
}

void handleNotFound() {
  server.send(404, "text/plain", "Controller page Not found");
}

String SendHTML() {
  String ptr = "<!DOCTYPE html> <html>\n";
  ptr += "<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0, user-scalable=no\">\n";
  ptr += "<title>Slingshot Controller Config</title>\n";
  ptr += "<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}\n";
  ptr += "body{margin-top: 50px;} h1 {color: #444444;margin: 50px auto 30px;} h3 {color: #444444;margin-bottom: 50px;}\n";
  ptr += "p {font-size: 14px;color: #888;margin-bottom: 10px;}\n";
  ptr += "</style>\n";
  ptr += "</head>\n";
  ptr += "<body>\n";
  ptr += "<h2>Slingshot Controller V3.1 Config</h2>\n";
  ptr += "<form action=\"/put\">";
 
  ptr += "Preset 1<input type=\"radio\" value=\"1\" onclick=\"window.location='/preset?selectedPreset=1';\"";
  ptr += iSelectedPreset == 1 ?  "checked>" : ">";
  ptr += "Preset 2<input type=\"radio\" value=\"2\" onclick=\"window.location='/preset?selectedPreset=2';\"";
  ptr += iSelectedPreset == 2 ?  "checked>" : ">";
  ptr += "Preset 3<input type=\"radio\" value=\"3\" onclick=\"window.location='/preset?selectedPreset=3';\"";
  ptr += iSelectedPreset == 3 ?  "checked>" : ">";
  ptr += "<hr><br>";

  ptr += "<label> PWM Freq(Hz) (200-25000) </label>";
  ptr += "<input type=\"range\" min=\"200\" max=\"25000\" step=\"100\" name=\"sPWMFreq\" oninput=\"this.nextElementSibling.value = this.value\" value=\"";
  ptr += String(iPWMFrequency);
  ptr += "\"> <output>";
  ptr += String(iPWMFrequency);
  ptr += "</output><br>";

  ptr += "<label> Brakes (0-255) </label>";
#ifdef __USE_BRAKE_POT__
  ptr += "<label> Brakes (0-255) (brake pot enabled)</label>";
  ptr += "<input disabled type=\"range\" min=\"0\" max=\"255\" name=\"sBrake\"";
#else
  ptr += "<input type=\"range\" min=\"0\" max=\"255\" name=\"sBrake\" oninput=\"this.nextElementSibling.value = this.value\" value=\"";
  ptr += String(iBrakeSetting);
  ptr += "\"> <output>";
  ptr += String(iBrakeSetting);
#endif
  ptr += "</output><br>";

  ptr += "<label> Throttle(%) (0-100) </label>";
  ptr += "<input type=\"range\" min=\"0\" max=\"100\" name=\"sThrottle\" oninput=\"this.nextElementSibling.value = this.value\" value=\"";
  ptr += String(iThrottleSetting);
  ptr += "\"> <output>";
  ptr += String(iThrottleSetting);
  ptr += "</output><br><hr>";

  ptr += "<br>==== BELOW NOT CURRENTLY IMPLEMENTED ===<br>";

  ptr += "<label> Min Speed (0-255) </label>";
  ptr += "<input type=\"range\" min=\"0\" max=\"255\" name=\"sCoast\" oninput=\"this.nextElementSibling.value = this.value\" value=\"";
  ptr += String(iCoastSetting);
  ptr += "\"> <output>";
  ptr += String(iCoastSetting);
  ptr += "</output><br><hr>";

  ptr += "<label> Curve exp (0.5->1<-2) 1 is flat</label>";
  ptr += "<input type=\"text\" min=\"0.5\" max=\"2\" id=\"sCurveVal\" name=\"sCurveVal\" value=\"";
  ptr += String(fCurveVal);
  ptr += "\"><br>";

  ptr += "<label> TCS (0-100) </label>";
  ptr += "<input type=\"range\" min=\"0\" max=\"100\" name=\"sTCS\" oninput=\"this.nextElementSibling.value = this.value\" value=\"";
  ptr += String(iTCSSetting);
  ptr += "\"> <output>";
  ptr += String(iTCSSetting);
  ptr += "</output><br>";

  ptr += "<label> Start TCS (0-255) </label>";
  ptr += "<input type=\"range\" min=\"0\" max=\"255\" name=\"sTCSStart\" oninput=\"this.nextElementSibling.value = this.value\" value=\"";
  ptr += String(iTCSStartSetting);
  ptr += "\"> <output>";
  ptr += String(iTCSStartSetting);
  ptr += "</output><br>";

  ptr += "<label> End TCS (0-255) </label>";
  ptr += "<input type=\"range\" min=\"0\" max=\"255\" name=\"sTCSStop\" oninput=\"this.nextElementSibling.value = this.value\" value=\"";
  ptr += String(iTCSStopSetting);
  ptr += "\"> <output>";
  ptr += String(iTCSStopSetting);
  ptr += "</output><br>";

  ptr += "<br><input type=\"submit\" value=\"Submit\"></form>";
  ptr += "</body>\n";
  ptr += "</html>\n";

  return ptr;
}