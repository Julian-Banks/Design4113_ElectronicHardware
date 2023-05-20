#include "esp_camera.h"
#include "SD_MMC.h"
#include <EEPROM.h>            // read and write from flash memory

// define the number of bytes you want to access
#define EEPROM_SIZE 1


#define uS_TO_S_FACTOR 1000000ULL  /* Conversion factor for micro seconds to seconds */
#define TIME_TO_SLEEP  5        /* Time ESP32 will go to sleep (in seconds) */

RTC_DATA_ATTR int bootCount = 0;

// Pin assignments for SD card module


// Camera model
#define CAMERA_MODEL_AI_THINKER

// Camera pins
#define CAM_PIN_PWDN 32
#define CAM_PIN_RESET -1
#define CAM_PIN_XCLK 0
#define CAM_PIN_SIOD 26
#define CAM_PIN_SIOC 27
#define CAM_PIN_D7 35
#define CAM_PIN_D6 34
#define CAM_PIN_D5 39
#define CAM_PIN_D4 36
#define CAM_PIN_D3 21
#define CAM_PIN_D2 19
#define CAM_PIN_D1 18
#define CAM_PIN_D0 5
#define CAM_PIN_VSYNC 25
#define CAM_PIN_HREF 23
#define CAM_PIN_PCLK 22

// Interval between taking pictures (in milliseconds)
#define INTERVAL 1000
int pictureNumber = 0;
// Initialize SD card
void initSDCard() {
  
    
}

void setup() {
  Serial.begin(115200);

  // Initialize SD card
  
 if(!SD_MMC.begin("/sdcard", true)){
        Serial.println("Failed to mount card");
        return;
    }
    uint8_t cardType = SD_MMC.cardType();
 
    Serial.print("\nCard Type: ");
     
    if(cardType == CARD_NONE){
        Serial.println("No card attached");
        return;
    }
    if(cardType == CARD_MMC){
        Serial.println("MMC");
    } else if(cardType == CARD_SD){
        Serial.println("SDSC");
    } else if(cardType == CARD_SDHC){
        Serial.println("SDHC");
    } else {
        Serial.println("UNKNOWN");
    }
 
    uint64_t cardSize = SD_MMC.cardSize();
    int cardSizeInMB = cardSize/(1024 * 1024);
     
    Serial.print("Card size: ");
    Serial.println(cardSizeInMB);
 
 
    uint64_t bytesAvailable = SD_MMC.totalBytes(); 
    int bytesAvailableInMB = bytesAvailable/(1024 * 1024);
 
    Serial.print("MB available: ");
    Serial.println(bytesAvailableInMB);
 
 
    uint64_t bytesUsed = SD_MMC.usedBytes();
    int bytesUsedInMB = bytesUsed/(1024 * 1024);
 
    Serial.print("MB used: ");
    Serial.println(bytesUsedInMB);

  // Initialize camera
  camera_config_t config;
  config.ledc_timer = LEDC_TIMER_0;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.pin_pwdn = CAM_PIN_PWDN;
  config.pin_reset = CAM_PIN_RESET;
  config.pin_xclk = CAM_PIN_XCLK;
  config.pin_sscb_sda = CAM_PIN_SIOD;
  config.pin_sscb_scl = CAM_PIN_SIOC;
  config.pin_d7 = CAM_PIN_D7;
  config.pin_d6 = CAM_PIN_D6;
  config.pin_d5 = CAM_PIN_D5;
  config.pin_d4 = CAM_PIN_D4;
  config.pin_d3 = CAM_PIN_D3;
  config.pin_d2 = CAM_PIN_D2;
  config.pin_d1 = CAM_PIN_D1;
  config.pin_d0 = CAM_PIN_D0;
  config.pin_vsync = CAM_PIN_VSYNC;
  config.pin_href = CAM_PIN_HREF;
  config.pin_pclk = CAM_PIN_PCLK;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_SVGA;
  config.jpeg_quality = 12;
  config.fb_count = 1;

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  Serial.println("Camera initialized");
}

void loop() {
  delay(1000);
  // Take a picture
  camera_fb_t* fb = esp_camera_fb_get();
  if (!fb) {
    Serial.println("Camera capture failed");
    return;
  }else{
    Serial.println("photo taken succesfully!");
  }
 delay(1000);
  // Create filename with timestamp 
  //String(millis()) 

 // file.write(fb->buf, fb->len);
  EEPROM.begin(EEPROM_SIZE);
  pictureNumber = EEPROM.read(0) + 1;

  // Path where new picture will be saved in SD Card
  String path = "/picture" + String(pictureNumber) +".jpg";

  fs::FS &fs = SD_MMC; 
  Serial.printf("Picture file name: %s\n", path.c_str());
  
  File file = fs.open(path.c_str(), FILE_WRITE);
  if(!file){
    Serial.println("Failed to open file in writing mode");
  } 
  else {
    file.write(fb->buf, fb->len); // payload (image), payload length
    Serial.printf("Saved file to path: %s\n", path.c_str());
    EEPROM.write(0, pictureNumber);
    EEPROM.commit();
  }
  file.close();

  // Free camera framebuffer
  esp_camera_fb_return(fb);

  // Wait for the interval to elapse
  delay(1000);
  }
