/*Optional config definitions*/
#define DEBUG_MODE                // Enable serial debug at 9600 baud
#define WAKEUP_SOURCE_TIMER       // Enable timer to wake from deep sleep
#define WAKEUP_SOURCE_GPIO        // Enable GPIO to wake from deep sleep
#define OLED_DISPLAY              // Enable readout to external display

/*Required config definitions*/
//
#define BATTERY_GPIO            GPIO_NUM_26
//Wakeup Timer
#ifdef  WAKEUP_SOURCE_TIMER
#define uS_TO_M_FACTOR 60000000ULL // Conversion factor for micro seconds to minutes */
#define TIME_TO_SLEEP  1          // Time ESP32 will go to sleep (in minutes) */
#endif
//Wakeup Pin
#ifdef  WAKEUP_SOURCE_GPIO
#define BUTTON_PIN_BITMASK(GPIO) (1ULL << GPIO)  // 2 ^ GPIO_NUMBER in hex
#define USE_EXT0_WAKEUP          1               // 1 = EXT0 wakeup, 0 = EXT1 wakeup
#define WAKEUP_GPIO              GPIO_NUM_33     // Only RTC IO are allowed - ESP32 Pin example
#endif
//OLED Display
#ifdef OLED_DISPLAY
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels
#define OLED_RESET     -1
#endif