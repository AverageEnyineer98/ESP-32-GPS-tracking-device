// Include Libraries
#include <esp_now.h>
#include <WiFi.h>
#include <HardwareSerial.h>
#include <TinyGPS++.h>
#include "BluetoothSerial.h"

// Set the serial port for GPS module
HardwareSerial gpsSerial(2);

// Set up Bluetooth
BluetoothSerial SerialBT;

// Set the GPS object
TinyGPSPlus gps;
/////////////////////////////////////////////////////////
// Variables for test data
float latA;
float longA;
int satA;

float latB;
float longB;
int satB;
// Define a data structure
typedef struct struct_message {
  //char a[32];
  float latB;
  float longB;
  int satB;
} struct_message;

// Create a structured object
struct_message myData;


// Callback function executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  // Serial.print("Data received length: ");
  // Serial.println(len);  //dont care, updated automatically
  // Serial.print("Latitude of B ESP32:");
  // Serial.println(myData.latB);
  // Serial.print("Longitude of B ESP32:");
  // Serial.println(myData.longB);
  // Serial.print("No. Of Satellite of B ESP32 ");
  // Serial.println(myData.satB);
  // Serial.println();
  latB = myData.latB;
  longB = myData.longB;
  satB = myData.satB;

  if (gps.location.isUpdated()) {
    latA = gps.location.lat();
    longA = gps.location.lng();
  }

  if (gps.satellites.isUpdated()) {
    satA = gps.satellites.value();
  }
  // Serial.println("Latitude of A ESP32:: ");
  // Serial.print(latA, 6);
  // Serial.print("Longitude of A ESP32: ");
  // Serial.println(longA, 6);
  // Serial.print("No. Of Satellite of A ESP32: ");
  // Serial.println(satA);
  // if (gps.date.isUpdated() && gps.time.isUpdated()) {
  // }//Day and time for future use
}

int

void setup() {
  // Set up Serial Monitor
  Serial.begin(115200);
  gpsSerial.begin(9600);

  SerialBT.begin("ESP32 GPS");                            //Print on BT
  Serial.println("Bluetooth Started! Ready to pair...");  //Print on serial
  delay(1000);
  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Initilize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register callback function
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  /////////////Print to bt serial
  SerialBT.print("Longitude of A is: ");
  SerialBT.print(longA);
  SerialBT.print(" | Longitude of B is: ");
  SerialBT.println(longB);
  SerialBT.print("Latitude of A is: ");
  SerialBT.print(latA);
  SerialBT.print(" | Latitude of B is: ");
  SerialBT.println(latB);
  SerialBT.print("No. of Satellites of A is: ");
  SerialBT.print(satA);
  SerialBT.print(" | No. of Satellites of B is: ");
  SerialBT.println(satB);
  ///////////////
}