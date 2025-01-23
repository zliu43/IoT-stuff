#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_strip.h"
#include "ml_leddriver.h"
#include "ml_buttondriver.h"


led_strip_handle_t strip = NULL;  

void app_main() {
    ws2812_init();
    while(1){
        button_t button = read_buttons();
        switch(button){
            case BUTTON_REC:
                setLED(0, 0,0,255);
                break;
            case BUTTON_MODE:
                setLED(1, 0,0,255);
                break;
            case BUTTON_PLAY:
                setLED(2, 0,0,255);
                break;
            case BUTTON_SET:
                setLED(3, 0,0,255);
                break;
            case BUTTON_VOL_MINUS:
                setLED(4, 0,0,255);
                break;
            case BUTTON_VOL_PLUS:
                clearLEDs();
                break;
            case BUTTON_NONE:
                break;
            default:
                break;
            
            writeLEDs();
        }
    }
}
