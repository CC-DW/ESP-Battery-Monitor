//Library Includes
#include <Arduino.h>
#include <driver/rtc_io.h>

/*Optional config definitions*/
#define DEBUG_MODE                // Enable serial debug at 9600 baud
#define WAKEUP_SOURCE_TIMER       // Enable timer to wake from deep sleep
#define WAKEUP_SOURCE_GPIO        // Enable GPIO to wake from deep sleep

/*Required config definitions*/
#ifdef  WAKEUP_SOURCE_TIMER
#define uS_TO_M_FACTOR 60000000ULL // Conversion factor for micro seconds to minutes */
#define TIME_TO_SLEEP  1          // Time ESP32 will go to sleep (in minutes) */
#endif
#ifdef  WAKEUP_SOURCE_GPIO
#define BUTTON_PIN_BITMASK(GPIO) (1ULL << GPIO)  // 2 ^ GPIO_NUMBER in hex
#define USE_EXT0_WAKEUP          1               // 1 = EXT0 wakeup, 0 = EXT1 wakeup
#define WAKEUP_GPIO              GPIO_NUM_33     // Only RTC IO are allowed - ESP32 Pin example
#endif

RTC_DATA_ATTR int bootCount = 0;

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