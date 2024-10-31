# PWM-slotcar-controller
Project to create a basic PWM slotcar (Scalextric) controller using an ESP-32 dev board.

# Pre-requisites
- ESP-32 Devkit board.
- BTS 7960 Dual H-bridge motor controller.
- A Slotcar controller handle to convert.
- Bits and bobs of wiring.
- Basic understanding of uploading code to the dev board - https://randomnerdtutorials.com/getting-started-with-esp32/. The ESP32 has WiFi and bluetooth a good demo for using the onboard webserver is here https://randomnerdtutorials.com/esp32-web-server-arduino-ide/.

# Wiring

![image](https://user-images.githubusercontent.com/58425116/233952739-11b20489-bafc-4a8d-982e-e2664df13e9d.png)

# ESP32 power
There will need to be an additional circuit using a simple voltage regulator that will take 10+V from the track and convert to 5V to power the board. For testing the board can be powered from the USB cable.

# Controller Trigger
The trigger could be a rotary potentiometer, a resistor board or a modified barrel resistor. If using an old resistor controller then it will need the base band connecting to the rest of the resistor. For example the image below shows a typical Parma style resistor, and the modification to make the variable resistor into a simple potentiometer.

![image](https://user-images.githubusercontent.com/58425116/223164797-4c20bdc1-1c4f-499e-ab69-b8096d39cf82.png)

# Webserver configuration
The Webserver will show up at 192.168.1.1 on the WiFi network slingshot-controller.
All values are saved and will be loaded on power on.

# Download and getting started
For those not used to Github, probably the simplest way to get started is as follows. This assumes you have already installed the Arduino IDE, there are lots of articles on installing and using the Arduino IDE i.e. https://lastminuteengineers.com/getting-started-with-esp32/
I'd suggest you scan the above article and get the blink example running if you are new to Arduino programming.

- Download a zip file containing the source code from here https://github.com/slngshotx/PWM-slotcar-controller/releases.
- Extract or uncompress the zip file somewhere on your local disk.
- Open the uncompressed folder and double click the file slot-controllerv3.ino. This should open all 3 files in the Arduino IDE.
- In the Arduino IDE select the COM port and board type (probably "ESP32 Devkit") and compile and upload the code to your ESP32.

