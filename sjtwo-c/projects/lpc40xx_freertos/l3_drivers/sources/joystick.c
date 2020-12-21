#include "joystick.h"
#include "adc.h"
#include "gpio.h"
#include "lpc40xx.h"

gpio_s p1;
gpio_s adc_xvalue;
gpio_s adc_yvalue;

gpio_s enable_joystick(gpio__port_e adc1, uint8_t x_pin, gpio__port_e adc2, uint8_t y_pin, gpio__port_e p, int pin) {
  const uint32_t set_analog_mode = (1U << 7);

  LPC_IOCON->P1_30 &= ~(set_analog_mode);
  LPC_IOCON->P0_25 &= ~(set_analog_mode);

  p1 = gpio__construct_as_input(p, pin);

  adc_xvalue = gpio__construct_with_function(
      adc1, x_pin, 1); // Here we are using pins 0,25 and pins 1,30 for the joystick and button
  adc_yvalue = gpio__construct_with_function(adc2, y_pin, 3);
  adc__initialize();
  return p1;
}

data get_data(adc_channel_e x_port, adc_channel_e y_port, gpio_s button_press) {
  data s;
  s.x_data = adc__get_adc_value(x_port);
  s.y_data = adc__get_adc_value(y_port);

  if (gpio__get(button_press)) {
    s.switch_pressed = true;
  } else {
    s.switch_pressed = false;
  }

  return s;
}
