#include "FreeRTOS.h"
#include "game_inputs.h"
#include "gpio.h"
#include "joystick.h"
#include "task.h"
#include <game_engine.h>
#include <game_gfx.h>
#include <game_music.h>
#include <led_gfx.h>
#include <ledmatrix.h>
#include <stdio.h>
#include <time.h>

LedMatrixDisplay initialize_led_display(void);
static void led_display_background(void *params);
static game_level_state game_bootup_configuration(void);
static void bubbleshooter_game(void *params);

LedMatrixDisplay initialize_led_display(void) {
  // Initialize the display.
  LedMatrixDisplay led_display = initialize_display(LED_MATRIX_32x64);
  return led_display;
}

static void led_display_background(void *params) {
  LedMatrixDisplay led_display = initialize_led_display();

  // Clear the front and back frames (if currently stored in memory);
  clear_screen(led_display, false);
  clear_screen(led_display, true);

  // Continues to run in the background endlessly without stopping.
  while (true) {
    display_frame(led_display, 0);
    vTaskDelay(1); // Give 1ms of time for the other task to preload a new frame.
  }
}

static game_level_state game_bootup_configuration(void) {
  game_level_state new_game_state;

  // Initialize the Music GPIO pins
  new_game_state.music_track_pins = initialize_track_select();

  // Load the Main menu as the first level
  new_game_state.level = LEVEL__MAIN_MENU;

  // Set the game credits to 0.
  new_game_state.credits = 0;

  // TODO: Load previous high score from the SD Card.
  uint16_t high_score = 0;
  new_game_state.score = high_score;

  // Ensure the shooter is pointing up.
  new_game_state.input_direction.direction = NONE;
  new_game_state.input_direction.previous_shooter_position = 0;

  // No button was pressed/No button will be checked (yet).
  new_game_state.button_presses[0] = false;
  new_game_state.button_presses[1] = false;

  // Use a random number generator to find three numbers from 1-3.
  pixel_color wheel_colors[] = {wheel(4), wheel(1), wheel(3)};

  // Configure the Current Player Ball Movement Status and Properties.
  wheel_colors[0].event = PLAYER_BALL;
  new_game_state.player_ball.center_of_ball.x = 0;
  new_game_state.player_ball.center_of_ball.y = 0;
  new_game_state.player_ball.properties = wheel_colors[0];
  new_game_state.ball_movement = STATIONARY;

  // Set the colors of the balls in the ball_queue.
  wheel_colors[1].event = PLAYER_BALL;
  wheel_colors[2].event = PLAYER_BALL;
  new_game_state.ball_queue.balls[0] = wheel_colors[1];
  new_game_state.ball_queue.balls[1] = wheel_colors[2];

  return new_game_state;
}

static void bubbleshooter_game(void *params) {
  // Set the frame to the front.
  bool choose_back_frame = false;

  // const pixel_color rgb_color = *((pixel_color *)params);
  LedMatrixDisplay led_display = initialize_led_display();

  // On bootup of the game, let's randomize a color from 0-7 for the main menu.
  volatile game_level_state game_state = game_bootup_configuration(); // Initialize the game state properties.

  // Initialize Joystick GPIOs.
  game_inputs external_inputs = initialize_game_inputs();

  while (true) {
    // Get the game controller inputs.
    // game_state = read_game_controller(external_inputs, game_state);
    game_state = read_game_controller_yaxis(external_inputs, game_state);

    // Plays the level.
    game_state = play_game(led_display, game_state, choose_back_frame);

    // Change the frame. This is required to avoid flickering.
    choose_back_frame = gfx_switch_frame();

    // Wait 200ms for the new input.
    vTaskDelay(100);
  }
}

int main(void) {
  // static const uint16_t transition_time = 3000; // 3 second delay on switching frames.
  static const pixel_color rgb_color = {0, 1, 0};
  srand(time(0)); // Start the random number generator.

  xTaskCreate(led_display_background, "display_controller", 1500, NULL, PRIORITY_MEDIUM, NULL);
  xTaskCreate(bubbleshooter_game, "game_engine", 2000, NULL, PRIORITY_LOW, NULL);

  // puts("Starting RTOS");
  vTaskStartScheduler(); // This function never returns unless RTOS scheduler runs out of memory and fails

  return 0;
}
