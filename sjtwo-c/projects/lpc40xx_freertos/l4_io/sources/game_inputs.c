#include <game_gfx.h>
#include <game_inputs.h>
#include <gpio.h>
#include <joystick.h>

/**************************************************************************************
 * Initialized the GPIO pins for Joystick inputs and buttons.
 *
 * Joystick X: P0.25 (Potentiometer: VOUT)
 * Joystick Y: P1.30
 * Joystick SEL: P1,23
 * Button 1: P1.29
 * Button 2: P2.1
 **************************************************************************************/
game_inputs initialize_game_inputs(void) {
  game_inputs external_inputs;

  // Initialize the External Joystick.
  external_inputs.joystick = enable_joystick(GPIO__PORT_0, 25, GPIO__PORT_1, 30, GPIO__PORT_1, 23);

  // Initialize Button 1 & Button 2.
  external_inputs.button1 = gpio__construct_with_function(GPIO__FUNCTION_1, 29, GPIO__FUNCITON_0_IO_PIN);
  external_inputs.button1 = gpio__construct_as_input(GPIO__FUNCTION_1, 29); // P1.29
  external_inputs.button2 = gpio__construct_with_function(GPIO__FUNCTION_2, 1, GPIO__FUNCITON_0_IO_PIN);
  external_inputs.button2 = gpio__construct_as_input(GPIO__FUNCTION_2, 1); // P2.1 (bottom of P1.29)
  gpio__enable_pull_down_resistors(external_inputs.button1);
  gpio__enable_pull_down_resistors(external_inputs.button2);

  return external_inputs;
}

/**************************************************************************************
 * Reads the External Joystick and 2 buttons and returns an input intent for the game.
 *
 * Joystick: Analog Thumb Joystick OR Potentiometer
 * Buttons 1 & 2: 2 switch buttons (any)
 *
 * NOTE:
 * For the potentiometer, connect in reverse.
 * 5V = GND
 * VOUT = ADC2 (P0.25)
 * GND = 5V
 **************************************************************************************/
game_level_state read_game_controller(game_inputs external_inputs, game_level_state game_state) {
  data d = get_data(ADC__CHANNEL_2, ADC__CHANNEL_4, external_inputs.joystick);

  // Decode Joystick direction intent.
  // printf("[X,Y]: %d,%d\n", d.x_data, d.y_data);
  if (d.x_data <= 4090 && d.x_data >= 50) {
    // printf("N\n");
    game_state.input_direction.direction = NONE;
  } else if (d.x_data > 4090) {
    // printf("R\n");
    game_state.input_direction.direction = RIGHT;
  } else if (d.x_data < 50) {
    // printf("L\n");
    game_state.input_direction.direction = LEFT;
  }

  // Decode the button presses
  if (gpio__get(external_inputs.button1)) {
    game_state.button_presses[0] = true;
  } else {
    game_state.button_presses[0] = false;
  }
  if (gpio__get(external_inputs.button2)) {
    game_state.button_presses[1] = true;
  } else {
    game_state.button_presses[1] = false;
  }

  return game_state;
}

/**************************************************************************************
 * Reads the External Joystick and 2 buttons and returns an input intent for the game.
 *
 * Joystick: Analog Thumb Joystick OR Potentiometer
 * Buttons 1 & 2: 2 switch buttons (any)
 *
 * NOTE:
 * For the potentiometer, connect in reverse.
 * 5V = GND
 * VOUT = ADC2 (P0.25)
 * GND = 5V
 **************************************************************************************/
game_level_state read_game_controller_yaxis(game_inputs external_inputs, game_level_state game_state) {
  data d = get_data(ADC__CHANNEL_2, ADC__CHANNEL_4, external_inputs.joystick);

  // Decode Joystick direction intent.
  printf("[X,Y]: %d,%d\n", d.x_data, d.y_data);
  if (d.y_data <= 2400 && d.y_data >= 2000) {
    // printf("N\n");
    game_state.input_direction.direction = NONE;
  } else if (d.y_data < 300) {
    // printf("R\n");
    game_state.input_direction.direction = RIGHT;
  } else if (d.y_data > 4000) {
    // printf("L\n");
    game_state.input_direction.direction = LEFT;
  }

  // Decode the button presses
  if (gpio__get(external_inputs.button1)) {
    game_state.button_presses[0] = true;
  } else {
    game_state.button_presses[0] = false;
  }
  if (gpio__get(external_inputs.button2)) {
    game_state.button_presses[1] = true;
  } else {
    game_state.button_presses[1] = false;
  }

  return game_state;
}