#ifndef ML_LEDDRIVER_H
#define ML_LEDDRIVER_H

#include <stdint.h>

/**
 * @brief Initializes the WS2812 driver.
 *
 * Sets up the RMT peripheral and prepares the system for controlling WS2812 LEDs.
 */
void ws2812_init();

/**
 * @brief Sets the color of an LED in the buffer.
 *
 * Call `writeLED()` after setting colors to update the LED strip.
 *
 * @param led_num The index of the LED (0 to NUM_LEDS - 1).
 * @param red Red intensity (0-255).
 * @param green Green intensity (0-255).
 * @param blue Blue intensity (0-255).
 */
void setLED(uint8_t led_num, uint8_t red, uint8_t green, uint8_t blue);

/**
 * @brief Transmits the color buffer to the WS2812 LED strip.
 *
 * Uses the ESP32 RMT peripheral to send the stored color data to the LEDs.
 * This function must be called after `setLED()` to apply changes.
 */
void writeLED();

#endif // ML_LEDDRIVER_H
