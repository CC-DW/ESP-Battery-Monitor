//Library Includes
#include <Arduino.h>
#include <driver/rtc_io.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <config.h>
#include <get_Voltage.h>

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT,&Wire, OLED_RESET);

RTC_DATA_ATTR u_int bootCount = 0;

RTC_DATA_ATTR uint16_t batteryVoltage = 0;
RTC_DATA_ATTR uint16_t batteryRawVoltage = 0;

#ifdef DEBUG_MODE
/*
DEBUG FUNCTION
Print the ESP wakeup source 
*/
void print_wakeup_reason() {
  esp_sleep_wakeup_cause_t wakeup_reason;

  wakeup_reason = esp_sleep_get_wakeup_cause();

  switch (wakeup_reason) {
    case ESP_SLEEP_WAKEUP_EXT0:     Serial.println("Wakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1:     Serial.println("Wakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER:    Serial.println("Wakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD: Serial.println("Wakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP:      Serial.println("Wakeup caused by ULP program"); break;
    default:                        Serial.printf("Wakeup was not caused by deep sleep: %d\n", wakeup_reason); break;
  }
}
#endif
void setup() {
  Serial.begin(9600);
  delay(1000);  //Inline delay for serial to start

  //Increment boot number
  ++bootCount;
  #ifdef OLED_DISPLAY
    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { 
      #ifdef DEBUG_MODE
        Serial.println(F("SSD1306 allocation failed"));
        for(;;); // Loop until connected
      #endif
    }
  #endif
  #ifdef DEBUG_MODE
  pinMode(DEBUG_HIGH_PIN, OUTPUT);
  pinMode(DEBUG_LOW_PIN, OUTPUT);
  digitalWrite(DEBUG_HIGH_PIN, HIGH);
  digitalWrite(DEBUG_LOW_PIN, LOW);
  #endif

  batteryRawVoltage = get_raw_voltage(BATTERY_GPIO);
  batteryVoltage = get_mv_voltage(BATTERY_GPIO);
  
  #ifdef DEBUG_MODE
    Serial.println("Raw Voltage on battery GPIO " + String(BATTERY_GPIO) + ": "+ batteryVoltage + "mA");
  #endif
  #ifdef OLED_DISPLAY
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.println("Voltage: " + String(batteryVoltage) + "mv");
    display.println("(" + String(batteryRawVoltage) + ")");
    display.display();
    delay(2000); // Pause for 2 seconds
  #endif

  #ifdef DEBUG_MODE
    //Print current boot count and waekeup reason
    Serial.println("Boot number: " + String(bootCount));
    print_wakeup_reason();
  #endif
#ifdef WAKEUP_SOURCE_TIMER
  //Configure wakeup timer period
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_M_FACTOR);
#endif
#ifdef WAKEUP_SOURCE_GPIO
  //Configure wakeup GPIO
  esp_sleep_enable_ext0_wakeup(WAKEUP_GPIO, HIGH);
  rtc_gpio_pullup_dis(WAKEUP_GPIO);
  rtc_gpio_pulldown_en(WAKEUP_GPIO);
#endif
  #ifdef DEBUG_MODE
    #ifdef WAKEUP_SOURCE_TIMER
      Serial.println("Setup ESP32 to sleep for " + String(TIME_TO_SLEEP) + " Minute(s)");
    #endif
    #ifdef WAKEUP_SOURCE_GPIO
      Serial.println("Setup ESP32 to sleep until HIGH input on pin " + String(WAKEUP_GPIO));
    #endif
    Serial.println("Going to sleep now");
    Serial.flush();
  #endif
  //Enter deep sleep until
  esp_deep_sleep_start();
}

void loop() {
  //This is not going to be called
}