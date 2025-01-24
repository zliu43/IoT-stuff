/**
 * @file ws2812_driver.c
 * @brief WS2812 LED Driver for ESP32-S3 Korvo 1 v5.0 board
 */

#include "ml_leddriver.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "led_strip.h"
#include "driver/rmt_tx.h"
#include "esp_err.h"

#define LED_GPIO 19
#define NUM_LEDS 12
#define RMT_CLK_DIV 8   

#define BIT0_HIGH 3
#define BIT0_LOW 9

#define BIT1_HIGH 9
#define BIT1_LOW 3

#define RESET_US 50

// Global RMT Channel & Encoder
static rmt_channel_handle_t rmt_channel = NULL;
static rmt_encoder_handle_t ws2812_encoder = NULL;

// LED Data Buffer (GRB format)
static uint8_t rgb_data[NUM_LEDS * 3] = {0};

// Transmission Configuration
static rmt_transmit_config_t tx_cfg = {
	.loop_count = 0
};

// Encoder Configuration
static rmt_bytes_encoder_config_t ws2812_encoder_config = {
	.bit0 = {
		.level0 = 1,
		.duration0 = BIT0_HIGH, 
		.level1 = 0,
		.duration1 = BIT0_LOW
	},
	.bit1 = {
		.level0 = 1,
		.duration0 = BIT1_HIGH, 
		.level1 = 0,
		.duration1 = BIT1_LOW
	},
	.flags.msb_first = true  
};

/**
 * @brief Initializes the WS2812 driver using the ESP32 RMT peripheral.
 *
 * This function configures the RMT TX channel and creates an RMT encoder
 * to generate the correct signal for WS2812 LEDs.
 */
void ws2812_init() {
	rmt_tx_channel_config_t tx_config = {
		.clk_src = RMT_CLK_SRC_DEFAULT,
		.gpio_num = LED_GPIO,
		.resolution_hz = 10000000,  // 10 MHz 
		.mem_block_symbols = 64,    
		.trans_queue_depth = 1
	};
	
	ESP_ERROR_CHECK(rmt_new_tx_channel(&tx_config, &rmt_channel));
	ESP_ERROR_CHECK(rmt_enable(rmt_channel));

	// 2️⃣ Create Encoder
	ESP_ERROR_CHECK(rmt_new_bytes_encoder(&ws2812_encoder_config, &ws2812_encoder));
}

/**
 * @brief Sets the color of a specific LED in the buffer.
 *
 * Stores the RGB values in the GRB format required by WS2812 LEDs.
 * This function does not immediately update the LED strip; call `writeLED()`
 * after setting colors to push the data to the LEDs.
 *
 * @param led_num The index of the LED (0 to NUM_LEDS - 1).
 * @param red Red intensity (0-255).
 * @param green Green intensity (0-255).
 * @param blue Blue intensity (0-255).
 */
void setLED(uint8_t led_num, uint8_t red, uint8_t green, uint8_t blue) {
	if (led_num >= NUM_LEDS) {
		return;  // Prevent buffer overflow
	}

	int index = led_num * 3;
	rgb_data[index] = green;  //GREEN -> RED -> BLUE
	rgb_data[index + 1] = red;
	rgb_data[index + 2] = blue;
}

/**
 * @brief Sends the color data from the buffer to the WS2812 LED strip.
 *
 * Uses the ESP32 RMT peripheral to transmit the stored RGB data
 * to the LEDs. This function must be called after `setLED()` to 
 * apply changes.
 */
void writeLEDs() {
	if (ws2812_encoder == NULL) {
		return;  // Prevent crash if encoder isn't initialized
	}

	ESP_ERROR_CHECK(rmt_transmit(rmt_channel, ws2812_encoder, rgb_data, NUM_LEDS * 3, &tx_cfg));
}

void setLEDs(){
	return;
}

void clearLEDs(){
	for(int i = 0; i < NUM_LEDS; ++i){
		int index = i * 3;
		rgb_data[index] = 0b00000000;  
		rgb_data[index + 1] = 0b00000000;
		rgb_data[index + 2] = 0b00000000;
	}
}
