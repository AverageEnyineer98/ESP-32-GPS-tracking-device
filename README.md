# ESP-32-GPS-tracking-device
This project was inspired when I was skiing with my friend.
When you wanna do cool tricks while skiing at high speed but you do not want your friend to hold the camera/phone during high speed.
This is the device to work.
The function of this device is to always make one (Master) esp32 that controls the robot arm to always point at the another ESP-32 (slave).
The user can attach a GoPro or phone on the robot arm, and the robot arm will always point the Master ESP-32 to the slave.
![Notes](https://github.com/AverageEnyineer98/ESP-32-GPS-tracking-device/assets/121141873/2fd76a68-60a3-440b-b124-c7c2d52f9bba)

--------------------------------------------------------------------------------------------------------------

I have divided the project to different phase/parts:

Phase 1 : Enable GPS module and receive coordinates/ GPS data

Phase 2 : Allow communication between two ESP-32 using ESP NOW

Phase 3 : Master ESP32 will calculate the relative position of the slave ESP-32

Phase 4 : Master ESP32 will automate the robot arm to always point it to the position of the Slave ESP-32

Phase 5 : Make CAD drawing and print prototype for both devices

Phase 6 : Add power source, in this case it will be a duo 18650 Battery (~2x3.67V), and the internal voltage regulator of the ESP-32 will regulate the voltage. (No more than 12V)

--------------------------------------------------------------------------------------------------------------
Phase 0 : Preparation 
When you load the code, make sure you hold on the "BOOT" button on the ESP-32 in Arduino while it shows "connecting" on the terminal, release when data writing is initiated
After that, press "EN" on the ESP-32 to enable bluetooth. Now you can pair your device with ESP-32 using bluetooth.
Further reference can be found in this link : https://www.electronicshub.org/esp32-bluetooth-tutorial/

Phase 1 : 
I have decided to use 2 ATGM332D GPS module, it is capable of receiving satellites with NMEA signals

To test the GPS, I have used my PC wifi card's antenna, and bluetooth the serial bus to my tablet. So I can see the information provided by sattelites outdoor.

The test code is BLEGPS.ino

Following is the setup and the satellite info I retrieved on an open ground
![346158488_195429856706999_6012077779530714286_n](https://github.com/AverageEnyineer98/ESP-32-GPS-tracking-device/assets/121141873/43f59e30-697f-42d1-b886-e6af99e15318)
![346106530_210601011727845_3340513990749274137_n](https://github.com/AverageEnyineer98/ESP-32-GPS-tracking-device/assets/121141873/b6e7ce7a-8f1a-43c8-a1a0-6bd347f3c096)
--------------------------------------------------------------------------------------------------------------
Phase 2 : Intercommunication for ESP-32
The intercom for the two ESP-32 is to allow the slave ESP-32 (transmitter) to send its GPS signal to the Master ESP-32 (Receiver)
