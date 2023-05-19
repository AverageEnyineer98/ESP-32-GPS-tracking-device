// Include Libraries
#include <esp_now.h>
#include <WiFi.h>
#include <HardwareSerial.h>
#include <TinyGPS++.h>
#include "BluetoothSerial.h"
#include <math.h>

// Set the serial port for the GPS module
HardwareSerial gpsSerial(2);

// Set up Bluetooth
BluetoothSerial SerialBT;

// Set the GPS object
TinyGPSPlus gps;

// Define the GPS structure
typedef struct {
  float x;
  float y;
  float z;
  float longitude;
  float latitude;
  float elevation;
} GPS;

// Create the GPS objects
GPS GPSA;
GPS GPSB;

// Define a data structure
typedef struct struct_message {
  float latB;
  float longB;
  int satB;
} struct_message;

// Create a structured object
struct_message myData;

// Convert latitude, longitude, and elevation to Cartesian coordinates
typedef struct {
  float x;
  float y;
  float z;
} Coordinates;

Coordinates Acoord;
Coordinates Bcoord;

Coordinates convertToCoordinates(GPS gpsData) {
  // Earth radius in meters
  float earthRadius = 6371000.0;

  // Convert latitude, longitude, and elevation to radians
  float latRad = gpsData.latitude * (M_PI / 180.0);
  float lonRad = gpsData.longitude * (M_PI / 180.0);

  // Calculate Cartesian coordinates
  float cosLat = cos(latRad);
  float sinLat = sin(latRad);
  float cosLon = cos(lonRad);
  float sinLon = sin(lonRad);

  float radiusPlusElevation = earthRadius + gpsData.elevation;

  Coordinates coordinates;
  coordinates.x = radiusPlusElevation * cosLat * cosLon;
  coordinates.y = radiusPlusElevation * cosLat * sinLon;
  coordinates.z = radiusPlusElevation * sinLat;

  return coordinates;
}

// Callback function executed when data is received
void OnDataRecv(const uint8_t *mac, const uint8_t *incomingData, int len) {
  memcpy(&myData, incomingData, sizeof(myData));
  // Retrieve Slave ESP32 GPS data
  GPSB.latitude = myData.latB;
  GPSB.longitude = myData.longB;
  GPSB.elevation = 0.0; // Set elevation for GPSB
  //GPSB = convertToCoordinates(GPSB);
  BCoord = convertToCoordinates(GPSB);
}

void setup() {
  // Set up Serial Monitor
  Serial.begin(9600);
  gpsSerial.begin(9600);

  SerialBT.begin("ESP32 GPS master Receiver");                            // Print on BT
  Serial.println("Bluetooth Started! Ready to pair...");  // Print on serial
  delay(1000);
  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);

  // Initialize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Register callback function
  esp_now_register_recv_cb(OnDataRecv);
}

void loop() {
  // Read the data from the GPS module
  while (gpsSerial.available() > 0) {
    gps.encode(gpsSerial.read());
  }

  if (gps.location.isUpdated()) {
    // Update GPSA object with current GPS data
    GPSA.latitude = gps.location.lat();
    GPSA.longitude = gps.location.lng();
    GPSA.elevation = gps.altitude.meters();
    //GPSA = convertToCoordinates(GPSA);
  }

  // Print GPSA data
  SerialBT.print("Longitude of A is: ");
  SerialBT.print(GPSA.longitude);
  SerialBT.print(" || Latitude of A is: ");
  SerialBT.print(GPSA.latitude);
  SerialBT.print(" || Elevation of A is: ");
  SerialBT.print(GPSA.elevation);
  SerialBT.print(" || Cartesian Coordinates of A: (");
  SerialBT.print(GPSA.x);
  SerialBT.print(", ");
  SerialBT.print(GPSA.y);
  SerialBT.print(", ");
  SerialBT.print(GPSA.z);
  SerialBT.println(")");

  // Print GPSB data
  SerialBT.print("Longitude of B is: ");
  SerialBT.print(GPSB.longitude);
  SerialBT.print(" || Latitude of B is: ");
  SerialBT.print(GPSB.latitude);
  SerialBT.print(" || Elevation of B is: ");
  SerialBT.print(GPSB.elevation);
  SerialBT.print(" || Cartesian Coordinates of B: (");
  SerialBT.print(GPSB.x);
  SerialBT.print(", ");
  SerialBT.print(GPSB.y);
  SerialBT.print(", ");
  SerialBT.print(GPSB.z);
  SerialBT.println(")");

  SerialBT.println();
  ACoord = convertToCoordinates(GPSA);
  delay(1000);
}
