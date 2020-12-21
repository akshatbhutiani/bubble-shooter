#pragma once

#include "adc.h"
#include "gpio.h"
#include <stdbool.h>
#include <stdint.h>

/* This driver contains functions used to interface the ADA512 2 - axis Analog Joystick and button.
 * We use the ADC driver to get values of the joystick.
 */

typedef struct {
  uint16_t x_data;
  uint16_t y_data;
  bool switch_pressed;
} data;

gpio_s enable_joystick(gpio__port_e adc1, uint8_t x_value, gpio__port_e adc2, uint8_t y_value, gpio__port_e p, int pin);

data get_data(adc_channel_e x_port, adc_channel_e y_port, gpio_s button_press);
