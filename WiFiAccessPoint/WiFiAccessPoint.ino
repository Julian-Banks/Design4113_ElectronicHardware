#include <WiFi.h>

const char* ssid = "Group7Cam";
const char* password = "123";


void setup() {
  Serial.begin(9600);

  // Set up softAP with SSID and password
  WiFi.softAP(ssid, password);

  // Print the softAP IP address so you can connect to it from another device
  Serial.print("SoftAP 'Group7Cam' IP address: ");
  Serial.println(WiFi.softAPIP());
  WiFi.setSleep(true);
}

void loop() {
}
