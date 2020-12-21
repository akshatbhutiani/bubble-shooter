#pragma once
#include <game_gfx.h>
#include <led_gfx.h>
#include <ledmatrix.h>

typedef struct {
  gpio_s joystick;
  gpio_s button1;
  gpio_s button2;
} game_inputs;

game_inputs initialize_game_inputs(void);
game_level_state read_game_controller(game_inputs external_inputs, game_level_state game_state);
game_level_state read_game_controller_yaxis(game_inputs external_inputs, game_level_state game_state);