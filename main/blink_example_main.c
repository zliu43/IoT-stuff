#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_strip.h"
#include "ml_leddriver.h"


led_strip_handle_t strip = NULL;  

void app_main() {
    ws2812_init();

    setLED(0, 255, 0, 0);  
    setLED(1, 0, 255, 0);  
    setLED(2, 0, 0, 255); 

    writeLED();
}
