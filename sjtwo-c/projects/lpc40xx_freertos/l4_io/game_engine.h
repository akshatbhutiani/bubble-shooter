#pragma once
#include <game_music.h>
#include <led_gfx.h>
#include <ledmatrix.h>

typedef enum {
  LEVEL__MAIN_MENU = 0,
  LEVEL__LEVEL1,
  LEVEL__LEVEL2,
  LEVEL__LEVEL3,
  LEVEL__PAUSE,
  LEVEL__GAME_OVER,
  LEVEL__NEXT_LEVEL,
  LEVEL__GAME_FINISHED,
  LEVEL__TEST
} game_levels;

typedef enum { GO_BACK = 0, MAIN_MENU, RESTART_LEVEL } pause_menu_options;

typedef struct {
  pause_menu_options option;
  bool selected;
  bool pause_loaded;
} pause_menu_actions;

typedef enum { NONE = 0, LEFT, RIGHT } input_intent;

typedef enum {
  STATIONARY = 0,
  UP_LEFT,
  UP,
  UP_RIGHT,
  BOTTOM_RIGHT,
  BOTTOM,
  BOTTOM_LEFT
} ball_physics_movement; // Indicates where the ball will be moving in the next frame.

typedef struct {
  input_intent direction; // This is the next direction to move the shooter cursor. 0 keeps the previous direction.
  int8_t previous_shooter_position; // This was the previous shooters position. 0 is up, while left is a negative value,
                                    // right is a positive value.
  ball_physics_movement shooter_direction;
  gfx__cursor ball_launch_position;
} bubbleshooter_input_direction;

typedef struct {
  pixel_color balls[2]; // No size is needed as there must always be 2 balls in the queue every frame.
} gfx_ball_queue;

typedef struct {
  gfx__cursor center_of_ball;
  pixel_color properties;
} bubbleshooter_player_ball;

typedef struct {
  bubbleshooter_input_direction input_direction; // Determines which direction the cursor should be in.
  bool button_presses[2];                        // When passed, what to draw next.
  gfx_ball_queue ball_queue;
  bubbleshooter_player_ball player_ball; // Represents where to place the player ball.
  ball_physics_movement ball_movement;   // If stationary, place the ball inside the launcher.
  uint8_t credits;                    // Game credits. Required to press the button on the main menu to start the game.
  uint16_t score;                     // No more than 5 digits (ideally want a score no > 99999)
  uint8_t turn;                       // The number of turns left before the game ends.
  pixel_color bouncy_wall_color;      // The color of the walls around the level. Used for moving the sticky wall down.
  pixel_color launcher_color;         // The color for the launcher.
  gfx__cursor launcher_position;      // The location of the launcher. Used to redraw the shooter and queue.
  gfx__cursor top_of_queue;           // The location where to place the balls for the queue.
  gfx__cursor queue_location;         // The location of the queue.
  gfx__cursor failure_line;           // The location of the failure line. Used to detect game over condition.
  gfx__cursor sticky_wall_line;       // The location of the sticky wall line. Used to move the wall down every turn.
  game_levels level;                  // Tracks the current level the player is at.
  bool level_complete;                // If there are no more sticky balls left over. set this flag to true.
  game_music_select music_track_pins; // The pins for selecting the music using GPIO.
  game_music_tracks music_track;      // Tracks the Music or SFX that should be played on each game state.
} game_level_state;

typedef struct {
  pixel_color color;
  gfx__cursor center_position;
} gfx__sticky_ball;

typedef struct {
  gfx__sticky_ball sticky_balls[10];
} gfx__sticky_ball_row;

typedef enum {
  BALL_EMPTY = 0,
  BALL_BLUE,
  BALL_GREEN,
  BALL_CYAN,
  BALL_RED,
  BALL_MAGENTA,
  BALL_YELLOW,
  BALL_WHITE,
} gfx__sticky_ball_colors;

typedef struct {
  pixel_color sticky_balls_colors[10];
} game_sticky_ball_row;

// ------------------------> Stick Wall Logic <---------------------------
void clear_all_sticky_balls(void);
bool sticky_ball_is_valid(gfx__sticky_ball selected_sticky_ball);
void load_single_sticky_ball(LedMatrixDisplay led_display, gfx__sticky_ball selected_sticky_ball,
                             bool choose_back_frame);
void load_all_sticky_balls(LedMatrixDisplay led_display, bool choose_back_frame);
void push_sticky_ball_row(LedMatrixDisplay led_display, game_sticky_ball_row raw_sticky_ball_row, int row,
                          gfx__cursor stick_wall_position, bool choose_back_frame);
bool attach_to_sticky_wall(game_level_state game_state, bool choose_back_frame);

// ------------------------> Configure Level <---------------------------
gfx__cursor initialize_launcher(LedMatrixDisplay led_display, pixel_color launcher_color, bool choose_back_frame);
game_level_state redraw_launcher_with_inputs(LedMatrixDisplay led_display, game_level_state game_state,
                                             bool choose_back_frame);

// ------------------------> Game logic <---------------------------
bool check_failure_condition(LedMatrixDisplay led_display, game_level_state game_state, bool choose_back_frame);
game_level_state move_sticky_wall(LedMatrixDisplay led_display, game_level_state game_state, uint8_t decrement_amount);
void destroy_all_sticky_balls(LedMatrixDisplay led_display, game_level_state game_state);
game_level_state scan_sticky_balls(LedMatrixDisplay led_display, game_level_state game_state, bool choose_back_frame);
game_level_state destroy_sticky_balls_on_matching_color(LedMatrixDisplay led_display, game_level_state game_state,
                                                        bool choose_back_frame);
game_level_state play_level(LedMatrixDisplay led_display, game_level_state game_state, bool choose_back_frame);
game_level_state play_game(LedMatrixDisplay led_display, game_level_state game_state, bool choose_back_frame);
game_level_state find_music_track(game_level_state game_state);
