//Library Includes
#include <Arduino.h>
#include <driver/rtc_io.h>
/*Required config definitions*/
#define uS_TO_M_FACTOR 60000000ULL // Conversion factor for micro seconds to minutes */
#define TIME_TO_SLEEP  1          // Time ESP32 will go to sleep (in minutes) */
#define BUTTON_PIN_BITMASK(GPIO) (1ULL << GPIO)  // 2 ^ GPIO_NUMBER in hex
#define USE_EXT0_WAKEUP          1               // 1 = EXT0 wakeup, 0 = EXT1 wakeup
#define WAKEUP_GPIO              GPIO_NUM_33     // Only RTC IO are allowed - ESP32 Pin example
/*Optional config definitions*/
#define DEBUG_MODE                 // Enable serial debug at 9600 baud

RTC_DATA_ATTR int bootCount = 0;
RTC_DATA_ATTR int last = 0;
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
//Configure wakeup timer period
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_M_FACTOR);
  esp_sleep_enable_ext0_wakeup(WAKEUP_GPIO, 1);  //1 = High, 0 = Low
  // Configure pullup/downs via RTCIO to tie wakeup pins to inactive level during deepsleep.
  // EXT0 resides in the same power domain (RTC_PERIPH) as the RTC IO pullup/downs.
  // No need to keep that power domain explicitly, unlike EXT1.
  rtc_gpio_pullup_dis(WAKEUP_GPIO);
  rtc_gpio_pulldown_en(WAKEUP_GPIO);

  #ifdef DEBUG_MODE
    Serial.println("Setup ESP32 to sleep for " + String(TIME_TO_SLEEP) + " Minute(s)");
    Serial.println("Going to sleep now");
    Serial.flush();
  #endif
  //Deep sleep for configured period
  esp_deep_sleep_start();
}

void loop() {
  //This is not going to be called
}