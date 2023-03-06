# PWM-slotcar-controller
Project to create a basic PWM slotcar (Scalextric) controller using an ESP-32 dev board

# Pre-requisites
ESP-32 Devkit board.
BTS 7960 Dual H-bridge motor controller.
A Slotcar controller handle to convert.
Bits and bobs of wiring.
Basic understanding of uploading code to the dev board - https://randomnerdtutorials.com/getting-started-with-esp32/. The ESP32 has WiFi and bluetooth a good demo for using the onboard webserver is here https://randomnerdtutorials.com/esp32-web-server-arduino-ide/.

# Wiring
![image](https://user-images.githubusercontent.com/58425116/223160588-e5de5309-8a2f-4e70-9643-8534811728b4.png)

# Controller Trigger
The trigger could be a rotary potentiometer, a resistor board or a modified barrel resistor. If using an old resistor controller then it will need the base band connecting to the rest of the resistor. For example the image below shows a typical Parma style resistor, and the modification to make the variable resistor into a simple potentiometer.

![image](https://user-images.githubusercontent.com/58425116/223164797-4c20bdc1-1c4f-499e-ab69-b8096d39cf82.png)

