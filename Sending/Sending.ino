// Include Libraries
#include <esp_now.h>
#include <WiFi.h>
#include <HardwareSerial.h>
#include <TinyGPS++.h>

// Set the serial port for GPS module
HardwareSerial gpsSerial(2);

// Set the GPS object
TinyGPSPlus gps;
/////////////////////////////////////////////////////////
// Variables for test data
float latB;
float longB;
int satB;
bool bool_value = true;
 
// MAC Address of responder - edit as required
uint8_t broadcastAddress[] = {0xB0, 0xB2, 0x1C, 0xA8, 0xFA, 0x40};
 
// Define a data structure
typedef struct struct_message {
  char a[32];
  float latB;
  float longB;
  int satB;
} struct_message;
 
// Create a structured object
struct_message myData;
 
// Peer info
esp_now_peer_info_t peerInfo;
 
// Callback function called when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) { //Automated function to show data is sent, ignore for now
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
 
void setup() {
  
  // Set up Serial Monitor
  Serial.begin(115200);
  // Start the serial communication with GPS module at a baud rate of 9600
  gpsSerial.begin(9600);
  delay(1000); //Delay for GPS module initialization
  ///////

  // Set ESP32 as a Wi-Fi Station
  WiFi.mode(WIFI_STA);
 
  // Initilize ESP-NOW
  if (esp_now_init() != ESP_OK) {
    Serial.println("Error initializing ESP-NOW");
    return;
  }
 
  // Register the send callback
  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, broadcastAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK){
    Serial.println("Failed to add peer");
    return;
  }
}
 
void loop() {

  // Invert the boolean value
  bool_value = !bool_value;
  
  // Format structured data
  strcpy(myData.a, "Welcome to the Workshop!");//Uselss, ignore for now
  // Obtain sat no.,long,lat
  if (gps.location.isUpdated()) {
  myData.latB = gps.location.lat();
  myData.longB = gps.location.lng();
  }

  if (gps.satellites.isUpdated()) {
  myData.satB = gps.satellites.value();

  }

  // if (gps.date.isUpdated() && gps.time.isUpdated()) {
  // }//Day and time for future use

  //have to wait for GPS signal, otherwise no meaning
  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData)); //Dontt touch broadcast add,
   
  if (result == ESP_OK) {
    Serial.println("Sending confirmed");
    Serial.print("Latitude: ");
    Serial.print(gps.location.lat(), 6);
    Serial.print(" | Longitude: ");
    Serial.println(gps.location.lng(), 6);
    Serial.print("Satellites: ");
    Serial.println(gps.satellites.value());
  }
  else {
    Serial.println("Sending error");
  }
  delay(2000);
}