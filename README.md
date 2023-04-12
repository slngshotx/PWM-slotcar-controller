# PWM-slotcar-controller
Project to create a basic PWM slotcar (Scalextric) controller using an ESP-32 dev board.

# Pre-requisites
- ESP-32 Devkit board.
- BTS 7960 Dual H-bridge motor controller.
- A Slotcar controller handle to convert.
- Bits and bobs of wiring.
- Basic understanding of uploading code to the dev board - https://randomnerdtutorials.com/getting-started-with-esp32/. The ESP32 has WiFi and bluetooth a good demo for using the onboard webserver is here https://randomnerdtutorials.com/esp32-web-server-arduino-ide/.

# Wiring

![image](https://user-images.githubusercontent.com/58425116/223165165-bd816452-1110-48b5-a136-a6db579d64a1.png)

# ESP32 power
There will need to be an additional circuit using a simple voltage regulator that will take 10+V from the track and convert to 5V to power the board. For testing the board can be powered from the USB cable.

# Controller Trigger
The trigger could be a rotary potentiometer, a resistor board or a modified barrel resistor. If using an old resistor controller then it will need the base band connecting to the rest of the resistor. For example the image below shows a typical Parma style resistor, and the modification to make the variable resistor into a simple potentiometer.

![image](https://user-images.githubusercontent.com/58425116/223164797-4c20bdc1-1c4f-499e-ab69-b8096d39cf82.png)

# Webserver configuration
The Webserver will show up at 192.168.1.1 on the WiFi network slingshot-controller.
All values are saved and will be loaded on power on.


