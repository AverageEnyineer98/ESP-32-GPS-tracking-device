#include <HardwareSerial.h>
#include <TinyGPS++.h>
#include "BluetoothSerial.h"

// Set the serial port for GPS module
HardwareSerial gpsSerial(2);

// Set the GPS object
TinyGPSPlus gps;

// Set up Bluetooth
BluetoothSerial SerialBT;

void setup() {
  // Start the serial communication with a baud rate of 9600
  Serial.begin(9600);

  // Start the serial communication with GPS module at a baud rate of 9600
  gpsSerial.begin(9600);

  // Start Bluetooth
  SerialBT.begin("ESP32 GPS");
  Serial.println("Bluetooth Started! Ready to pair...");

  // Wait for the GPS module to initialize
  delay(1000);
}

void loop() {
  // Read the data from the GPS module
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }

  // Print the location data to the serial monitor and Bluetooth
  if (gps.location.isUpdated()) {
    Serial.print("Latitude: ");
    Serial.print(gps.location.lat(), 6);
    Serial.print(" | Longitude: ");
    Serial.println(gps.location.lng(), 6);
    SerialBT.print("Latitude: ");
    SerialBT.print(gps.location.lat(), 6);
    SerialBT.print(" | Longitude: ");
    SerialBT.println(gps.location.lng(), 6);
  }
  
  // Print the number of satellites being tracked
  if (gps.satellites.isUpdated()) {
    Serial.print("Satellites: ");
    Serial.println(gps.satellites.value());
    SerialBT.print("Satellites: ");
    SerialBT.println(gps.satellites.value());
  }
  
  // Print the date and time
  if (gps.date.isUpdated() && gps.time.isUpdated()) {
    Serial.print("Date: ");
    Serial.print(gps.date.month());
    Serial.print("/");
    Serial.print(gps.date.day());
    Serial.print("/");
    Serial.print(gps.date.year());
    Serial.print(" | Time: ");
    Serial.print(gps.time.hour());
    Serial.print(":");
    Serial.print(gps.time.minute());
    Serial.print(":");
    Serial.println(gps.time.second());
    SerialBT.print("Date: ");
    SerialBT.print(gps.date.month());
    SerialBT.print("/");
    SerialBT.print(gps.date.day());
    SerialBT.print("/");
    SerialBT.print(gps.date.year());
    SerialBT.print(" | Time: ");
    SerialBT.print(gps.time.hour());
    SerialBT.print(":");
    SerialBT.print(gps.time.minute());
    SerialBT.print(":");
    SerialBT.println(gps.time.second());
  }

  // Read from Bluetooth and send to serial
  if (SerialBT.available()) {
    Serial.write(SerialBT.read());
  }

  // Read from serial and send to Bluetooth
  if (Serial.available()) {
    SerialBT.write(Serial.read());
  }

  delay(20);
}
