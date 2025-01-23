#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_strip.h"
#include "ml_leddriver.h"

#define LED_GPIO 19         
#define LED_COUNT 12      
#define BLINK_DELAY_MS 500  

led_strip_handle_t strip = NULL;  

void blink_Task(void *arg) {
    // WS2812 LED strip configuration
    const led_strip_config_t led_config = {
        .strip_gpio_num = LED_GPIO,
        .max_leds = LED_COUNT,
        .led_pixel_format = LED_PIXEL_FORMAT_GRB,
        .led_model = LED_MODEL_WS2812,
    };

    const led_strip_rmt_config_t rmt_config = {};  
    led_strip_new_rmt_device(&led_config, &rmt_config, &strip);
    
    if (!strip) {
        printf("Failed to initialize WS2812 LED driver!\n");
        vTaskDelete(NULL);
    }

    printf("WS2812 LED Blinking Task Started!\n");

    while (1) {
        for (int i = 0; i < LED_COUNT; i++) {
            ESP_ERROR_CHECK(led_strip_set_pixel(strip, i, 255, 255, 255));
        }
        ESP_ERROR_CHECK(led_strip_refresh(strip));
        vTaskDelay(pdMS_TO_TICKS(BLINK_DELAY_MS));

        // Turn all LEDs OFF
        ESP_ERROR_CHECK(led_strip_clear(strip));
        vTaskDelay(pdMS_TO_TICKS(BLINK_DELAY_MS));
    }
}

void app_main() {
    ws2812_init();
    
    xTaskCreate(&blink_Task, "blink_Task", 4096, NULL, 5, NULL);
}
