#pragma once
#include <gpio.h>

typedef enum {
  MUSIC__NONE = 0,
  MUSIC__MAIN_MENU,
  MUSIC_LEVEL_1,
  MUSIC_LEVEL_2,
  MUSIC_LEVEL_3,
  SFX_GAME_OVER,
  SFX_BALL_SHOOT,
  SFX_BALL_DESTROY,
} game_music_tracks;

typedef struct {
  gpio_s bit2;
  gpio_s bit1;
  gpio_s bit0;
} game_music_select;

game_music_select initialize_track_select(void);
void change_music_track(game_music_select track_pins, game_music_tracks track);