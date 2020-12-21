#pragma once
#include <game_music.h>
#include <gpio.h>

/****************************************************
 * Initializes 3 GPIO bins used on the SJ2 board
 * to be used to select a track #. The pins are
 * read back from a sister SJ2 board which decodes
 * the 3 bits and plays music automatically.
 *
 * NOTE:
 * These pins are not using UART, SPI or I2C.
 * Using those may cause an interrupt routine to
 * occur which may result in flickering of our
 * display which just cannot be interruptted.
 ****************************************************/
game_music_select initialize_track_select(void) {
  game_music_select game_music_selection;

  // Track # = Track[2:0]: ABC --> A = bit2, B = bit1, C = bit 0
  gpio__construct_with_function(GPIO__PORT_0, 15, GPIO__FUNCITON_0_IO_PIN);
  game_music_selection.bit2 = gpio__construct_as_output(GPIO__PORT_0, 15); // P0.15
  gpio__construct_with_function(GPIO__PORT_0, 18, GPIO__FUNCITON_0_IO_PIN);
  game_music_selection.bit1 = gpio__construct_as_output(GPIO__PORT_0, 18); // P0.18
  gpio__construct_with_function(GPIO__PORT_0, 1, GPIO__FUNCITON_0_IO_PIN);
  game_music_selection.bit0 = gpio__construct_as_output(GPIO__PORT_0, 1); // P0.1

  return game_music_selection;
}

/****************************************************
 * Specify a track number between 0-7.
 *
 * Track:
 * 0 - Main Menu (BG Music)
 * 1 - Level 1 (BG Music)
 * 2 - Level 2 (BG Music)
 * 3 - Level 3 (BG Music)
 * 4 - Game Over (SFX)
 * 5 - Ball Shoot (SFX)
 * 6 - Ball Destroyed (SFX)
 * 7 - Game Won (SFX)
 ****************************************************/
void change_music_track(game_music_select track_pins, game_music_tracks track) {
  if (track == MUSIC__NONE) {
    gpio__reset(track_pins.bit2);
    gpio__reset(track_pins.bit1);
    gpio__reset(track_pins.bit0);
  } else if (track == MUSIC_LEVEL_1) {
    gpio__reset(track_pins.bit2);
    gpio__reset(track_pins.bit1);
    gpio__set(track_pins.bit0);
  } else if (track == MUSIC_LEVEL_2) {
    gpio__reset(track_pins.bit2);
    gpio__set(track_pins.bit1);
    gpio__reset(track_pins.bit0);
  } else if (track == MUSIC_LEVEL_3) {
    gpio__reset(track_pins.bit2);
    gpio__set(track_pins.bit1);
    gpio__set(track_pins.bit0);
  } else if (track == SFX_GAME_OVER) {
    gpio__set(track_pins.bit2);
    gpio__reset(track_pins.bit1);
    gpio__reset(track_pins.bit0);
  } else if (track == SFX_BALL_SHOOT) {
    gpio__set(track_pins.bit2);
    gpio__reset(track_pins.bit1);
    gpio__set(track_pins.bit0);
  } else if (track == SFX_BALL_DESTROY) {
    gpio__set(track_pins.bit2);
    gpio__set(track_pins.bit1);
    gpio__reset(track_pins.bit0);
  } else {
    gpio__set(track_pins.bit2);
    gpio__set(track_pins.bit1);
    gpio__set(track_pins.bit0);
  }
}