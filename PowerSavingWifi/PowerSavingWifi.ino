
/*
 *  ESP32
 *  Modem Sleep and wake up
 *  by Mischianti Renzo <https://www.mischianti.org>
 *
 *  https://www.mischianti.org/category/tutorial/esp32-tutorial/esp32-practical-power-saving/
 *
 */
 
#include <WiFi.h>
#include <BluetoothSerial.h>
#include "driver/adc.h"
#include <esp_bt.h>
 
#define STA_SSID "Group7Cam"
#define STA_PASS "123"
 
BluetoothSerial SerialBT;
 
void setModemSleep();
void wakeModemSleep();
 
void setup() {
    Serial.begin(115200);
 
    while(!Serial){delay(500);}
 

 
    Serial.println("START WIFI");
    WiFi.softAP(STA_SSID, STA_PASS);
 
 
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.softAPIP());
 
    setModemSleep();
    Serial.println("MODEM SLEEP ENABLED FOR 5secs");
}
 
//void loop() {}
unsigned long startLoop = millis();
bool started = false;
void loop() {
    if (!started && startLoop+20000<millis()){
        // Not use delay It has the own policy
        wakeModemSleep();
        Serial.println("MODEM SLEEP DISABLED");
        started = true;
        startLoop = millis();
    }else if(startLoop+20000<millis()){
        setModemSleep();
        Serial.println("MODEM SLEEP enabled");
        started = false;
        startLoop = millis();
    }
}
 
void disableWiFi(){
    
    WiFi.disconnect(true);  // Disconnect from the network
    WiFi.mode(WIFI_OFF);    // Switch WiFi off
    Serial.println("");
    Serial.println("WiFi disconnected!");
}

 
void setModemSleep() {
    disableWiFi();

    setCpuFrequencyMhz(40);
    // Use this if 40Mhz is not supported
    // setCpuFrequencyMhz(80);
}
 
void enableWiFi(){
   // adc_power_on();
    delay(200);
 
    WiFi.disconnect(false);  // Reconnect the network
    WiFi.mode(WIFI_STA);    // Switch WiFi off
 
    delay(200);
 
    Serial.println("START WIFI");
    WiFi.softAP(STA_SSID, STA_PASS);
  
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.softAPIP());
}
 
void wakeModemSleep() {
    setCpuFrequencyMhz(240);
    enableWiFi();
}