#include <game_engine.h>
#include <game_gfx.h>
#include <led_gfx.h>
#include <ledmatrix.h>
#include <stdbool.h>
#include <stdint.h>

// Used by the pause menu to redraw the pixels upon selected resuming the level.
volatile pixel_color background_gfx[24][15];

/*******************************************
 * Changes the orientation of the cursor
 * based on the value of joystick_x.
 *
 * There are only 7 possible directions
 * for the curstor.
 ******************************************/
bubbleshooter_input_direction place_shooter(LedMatrixDisplay led_display, gfx__cursor launcher_position,
                                            bubbleshooter_input_direction position_change_intent,
                                            bool choose_back_frame) {
  volatile int8_t shooter_position = 0;
  pixel_color tail = {0, 0, 1};
  pixel_color head = {1, 0, 0};

  // Based off the direction intent, move the cursor, except when the direction is NONE.
  if (position_change_intent.direction == NONE) {
    shooter_position = position_change_intent.previous_shooter_position;
  } else if (position_change_intent.direction == LEFT) {
    if (position_change_intent.previous_shooter_position == -3) {
      shooter_position = position_change_intent.previous_shooter_position;
    } else {
      shooter_position = position_change_intent.previous_shooter_position - 1;
    }
  } else if (position_change_intent.direction == RIGHT) {
    if (position_change_intent.previous_shooter_position == 3) {
      shooter_position = position_change_intent.previous_shooter_position;
    } else {
      shooter_position = position_change_intent.previous_shooter_position + 1;
    }
  }
  // Update the position which represents the direction to point the shooter.
  position_change_intent.previous_shooter_position = shooter_position;
  position_change_intent.direction = NONE; // Already moved.

  // All the various positions for the cursor.
  if (shooter_position == -3) {
    modify_pixel(led_display, tail, launcher_position.x - 1, launcher_position.y + 4, choose_back_frame);
    modify_pixel(led_display, tail, launcher_position.x, launcher_position.y + 3, choose_back_frame);
    modify_pixel(led_display, head, launcher_position.x - 2, launcher_position.y + 3, choose_back_frame);
    modify_pixel(led_display, head, launcher_position.x - 2, launcher_position.y + 4, choose_back_frame);
    modify_pixel(led_display, head, launcher_position.x - 2, launcher_position.y + 5, choose_back_frame);
    modify_pixel(led_display, head, launcher_position.x - 1, launcher_position.y + 5, choose_back_frame);
    modify_pixel(led_display, head, launcher_position.x, launcher_position.y + 5, choose_back_frame);
    position_change_intent.shooter_direction = UP_LEFT;
    position_change_intent.ball_launch_position.x = launcher_position.x - 3;
    position_change_intent.ball_launch_position.y = launcher_position.y + 6;
  } else if (shooter_position == -2) {
    modify_pixel(led_display, tail, launcher_position.x + 1, launcher_position.y + 4, choose_back_frame);
    modify_pixel(led_display, tail, launcher_position.x, launcher_position.y + 5, choose_back_frame);
    modify_pixel(led_display, head, launcher_position.x - 1, launcher_position.y + 4, choose_back_frame);
    modify_pixel(led_display, head, launcher_position.x - 1, launcher_position.y + 5, choose_back_frame);
    modify_pixel(led_display, head, launcher_position.x - 1, launcher_position.y + 6, choose_back_frame);
    modify_pixel(led_display, head, launcher_position.x, launcher_position.y + 6, choose_back_frame);
    modify_pixel(led_display, head, launcher_position.x + 1, launcher_position.y + 6, choose_back_frame);
    position_change_intent.shooter_direction = UP_LEFT;
    position_change_intent.ball_launch_position.x = launcher_position.x - 2;
    position_change_intent.ball_launch_position.y = launcher_position.y + 7;
  } else if (shooter_position == -1) { // Left
    modify_pixel(led_display, tail, launcher_position.x + 2, launcher_position.y + 5, choose_back_frame);
    modify_pixel(led_display, tail, launcher_position.x + 1, launcher_position.y + 6, choose_back_frame);
    modify_pixel(led_display, head, launcher_position.x, launcher_position.y + 5, choose_back_frame);
    modify_pixel(led_display, head, launcher_position.x, launcher_position.y + 6, choose_back_frame);
    modify_pixel(led_display, head, launcher_position.x, launcher_position.y + 7, choose_back_frame);
    modify_pixel(led_display, head, launcher_position.x + 1, launcher_position.y + 7, choose_back_frame);
    modify_pixel(led_display, head, launcher_position.x + 2, launcher_position.y + 7, choose_back_frame);
    position_change_intent.shooter_direction = UP_LEFT;
    position_change_intent.ball_launch_position.x = launcher_position.x - 1;
    position_change_intent.ball_launch_position.y = launcher_position.y + 8;
  } else if (shooter_position == 0) { // Center
    modify_pixel(led_display, tail, launcher_position.x + 4, launcher_position.y + 6, choose_back_frame);
    modify_pixel(led_display, tail, launcher_position.x + 4, launcher_position.y + 7, choose_back_frame);
    modify_pixel(led_display, head, launcher_position.x + 2, launcher_position.y + 6, choose_back_frame);
    modify_pixel(led_display, head, launcher_position.x + 3, launcher_position.y + 7, choose_back_frame);
    modify_pixel(led_display, head, launcher_position.x + 4, launcher_position.y + 8, choose_back_frame);
    modify_pixel(led_display, head, launcher_position.x + 5, launcher_position.y + 7, choose_back_frame);
    modify_pixel(led_display, head, launcher_position.x + 6, launcher_position.y + 6, choose_back_frame);
    position_change_intent.shooter_direction = UP;
    position_change_intent.ball_launch_position.x = launcher_position.x + 4;
    position_change_intent.ball_launch_position.y = launcher_position.y + 10;
  } else if (shooter_position == 1) { // Right
    modify_pixel(led_display, tail, launcher_position.x + 6, launcher_position.y + 5, choose_back_frame);
    modify_pixel(led_display, tail, launcher_position.x + 7, launcher_position.y + 6, choose_back_frame);
    modify_pixel(led_display, head, launcher_position.x + 8, launcher_position.y + 5, choose_back_frame);
    modify_pixel(led_display, head, launcher_position.x + 8, launcher_position.y + 6, choose_back_frame);
    modify_pixel(led_display, head, launcher_position.x + 8, launcher_position.y + 7, choose_back_frame);
    modify_pixel(led_display, head, launcher_position.x + 7, launcher_position.y + 7, choose_back_frame);
    modify_pixel(led_display, head, launcher_position.x + 6, launcher_position.y + 7, choose_back_frame);
    position_change_intent.shooter_direction = UP_RIGHT;
    position_change_intent.ball_launch_position.x = launcher_position.x + 9;
    position_change_intent.ball_launch_position.y = launcher_position.y + 8;
  } else if (shooter_position == 2) {
    modify_pixel(led_display, tail, launcher_position.x + 7, launcher_position.y + 4, choose_back_frame);
    modify_pixel(led_display, tail, launcher_position.x + 8, launcher_position.y + 5, choose_back_frame);
    modify_pixel(led_display, head, launcher_position.x + 9, launcher_position.y + 4, choose_back_frame);
    modify_pixel(led_display, head, launcher_position.x + 9, launcher_position.y + 5, choose_back_frame);
    modify_pixel(led_display, head, launcher_position.x + 9, launcher_position.y + 6, choose_back_frame);
    modify_pixel(led_display, head, launcher_position.x + 8, launcher_position.y + 6, choose_back_frame);
    modify_pixel(led_display, head, launcher_position.x + 7, launcher_position.y + 6, choose_back_frame);
    position_change_intent.shooter_direction = UP_RIGHT;
    position_change_intent.ball_launch_position.x = launcher_position.x + 10;
    position_change_intent.ball_launch_position.y = launcher_position.y + 7;
  } else if (shooter_position == 3) {
    modify_pixel(led_display, tail, launcher_position.x + 8, launcher_position.y + 3, choose_back_frame);
    modify_pixel(led_display, tail, launcher_position.x + 9, launcher_position.y + 4, choose_back_frame);
    modify_pixel(led_display, head, launcher_position.x + 10, launcher_position.y + 3, choose_back_frame);
    modify_pixel(led_display, head, launcher_position.x + 10, launcher_position.y + 4, choose_back_frame);
    modify_pixel(led_display, head, launcher_position.x + 10, launcher_position.y + 5, choose_back_frame);
    modify_pixel(led_display, head, launcher_position.x + 9, launcher_position.y + 5, choose_back_frame);
    modify_pixel(led_display, head, launcher_position.x + 8, launcher_position.y + 5, choose_back_frame);
    position_change_intent.shooter_direction = UP_RIGHT;
    position_change_intent.ball_launch_position.x = launcher_position.x + 11;
    position_change_intent.ball_launch_position.y = launcher_position.y + 6;
  }

  return position_change_intent;
}

/***********************************
 * Places a launcher on the screen.
 ***********************************/
void place_launcher(LedMatrixDisplay led_display, pixel_color rgb, gfx__cursor start_cursor, bool choose_back_frame) {
  /***********************************************
   *    xxxxxx
   *   x      x << This is a launcher
   *  xxxxxxxxxx
   *
   * It has a enum property of BACKGROUND_OBJECT.
   *
   * The launcher is oriented vertically for
   * our game.
   **********************************************/
  game_event object_property = BACKGROUND_OBJECT;
  rgb.event = object_property;

  // Draw a vertical line.
  draw_line(led_display, rgb, ROTATION_90, start_cursor, 9, choose_back_frame);

  // Fill the launcher shape.
  modify_pixel(led_display, rgb, start_cursor.x + 0, start_cursor.y + 1, choose_back_frame);
  modify_pixel(led_display, rgb, start_cursor.x + 0, start_cursor.y + 2, choose_back_frame);
  modify_pixel(led_display, rgb, start_cursor.x + 1, start_cursor.y + 3, choose_back_frame);
  modify_pixel(led_display, rgb, start_cursor.x + 2, start_cursor.y + 4, choose_back_frame);
  modify_pixel(led_display, rgb, start_cursor.x + 3, start_cursor.y + 5, choose_back_frame);
  modify_pixel(led_display, rgb, start_cursor.x + 4, start_cursor.y + 5, choose_back_frame);
  modify_pixel(led_display, rgb, start_cursor.x + 5, start_cursor.y + 5, choose_back_frame);
  modify_pixel(led_display, rgb, start_cursor.x + 6, start_cursor.y + 4, choose_back_frame);
  modify_pixel(led_display, rgb, start_cursor.x + 7, start_cursor.y + 3, choose_back_frame);
  modify_pixel(led_display, rgb, start_cursor.x + 8, start_cursor.y + 1, choose_back_frame);
  modify_pixel(led_display, rgb, start_cursor.x + 8, start_cursor.y + 2, choose_back_frame);
}

/***********************************
 * Places a ball queue on the screen.
 ***********************************/
gfx__cursor place_ball_queue(LedMatrixDisplay led_display, pixel_color rgb, gfx__cursor start_cursor,
                             bool choose_back_frame) {
  /***********************************************
   *    xxxxxxxx
   *    X o  o   << the ball queue
   *    x   xxxx
   *
   * It has a enum property of BACKGROUND_OBJECT.
   *
   * The ball queue is oriented vertically for
   * our game.
   **********************************************/
  game_event object_property = BACKGROUND_OBJECT;
  rgb.event = object_property;
  gfx__cursor location = {start_cursor.x + 1, start_cursor.y + 4};
  gfx__cursor top_of_queue = {location.x, start_cursor.y + 1};

  // Draw a vertical line.
  draw_line(led_display, rgb, ROTATION_90, location, 8, choose_back_frame);

  // Draw a horizontal line.
  draw_line(led_display, rgb, ROTATION_0, start_cursor, 4, choose_back_frame);

  return top_of_queue;
}

/************************************************
 * Loads the balls into the queue on the screen.
 ***********************************************/
void gfx_load_ball_queue(LedMatrixDisplay led_display, pixel_color *ball_queue, gfx__cursor queue_position,
                         bool choose_back_frame) {
  /***********************************************
   * It has a enum property of BACKGROUND_OBJECT.
   *
   * The launcher is oriented vertically for
   * our game.
   **********************************************/
  gfx__cursor location = {queue_position.x + 1, queue_position.y + 1};

  // Place the first ball at the location of the cursor inside the queue GFX.
  place_ball(led_display, ball_queue[0], location, true, choose_back_frame);

  // Place the second ball adjacent to the first ball, vertically.
  location.x += 4;
  place_ball(led_display, ball_queue[1], location, true, choose_back_frame);
}

/**********************************************************
 * Draws a fail line just above the center shooter.
 *
 * Any wall object such as STICKY_BALL or STICKY_WALL
 * which touches this line will instantly cause a game
 * over condition to occur.
 *********************************************************/
gfx__cursor draw_fail_line(LedMatrixDisplay led_display, game_level_state game_state, bool choose_back_frame) {
  // The fail line is just above the launcher and dictates a game over.
  pixel_color failure_line_color = wheel(6);
  failure_line_color.event = FAILURE_LINE; // Tells ball when touching surface, it needs to bounce off.

  // Draw the sticky wall in between the bouncy walls.
  draw_line(led_display, failure_line_color, ROTATION_90, game_state.failure_line, led_display.rows - 2,
            choose_back_frame);

  return game_state.failure_line;
}

/**********************************************************
 * Draws the side walls with a BOUNCY_WALL property
 * which will allow the player ball to bounce off
 *
 * (see play_level() for more details.)
 *********************************************************/
void draw_bouncy_walls(LedMatrixDisplay led_display, pixel_color bouncy_wall_color, bool choose_back_frame) {
  // The bouncy walls are the walls to the left and right of the launcher.
  volatile pixel_color new_color = bouncy_wall_color;
  new_color.event = BOUNCY_WALL; // Tells ball when touching surface, it needs to bounce off.

  gfx__cursor bouncy_wall_left = {0, 0};
  gfx__cursor bouncy_wall_right = {31, 0};
  draw_line(led_display, new_color, ROTATION_0, bouncy_wall_left, led_display.columns, choose_back_frame);
  draw_line(led_display, new_color, ROTATION_0, bouncy_wall_right, led_display.columns, choose_back_frame);
}

/**********************************************************
 * Draws the falling wall or ceiling which will have
 * sticky balls attached to it.
 *
 * NOTE:
 * The falling wall/ceiling has some unique properties.
 *
 * Sticky balls can be pre-programmed to have balls attached
 * to the wall.
 *
 * If no nearby ball is detected, the player ball will get
 * destroyed when touching this wall.
 *********************************************************/
gfx__cursor draw_falling_wall(LedMatrixDisplay led_display, bool choose_back_frame) {
  // The falling wall/ceiling has some unique properties.
  // Sticky balls can be pre-programmed to have balls attached to the wall.
  // If no nearby ball is detected, the player ball will get destroyed when touching this wall.
  pixel_color sticky_wall_color = wheel(6);
  sticky_wall_color.event = STICKY_WALL; // Tells ball when touching surface, it needs to bounce off.

  // Draw the sticky wall in between the bouncy walls.
  gfx__cursor sticky_wall_position = {1, led_display.columns - 18};
  draw_line(led_display, sticky_wall_color, ROTATION_90, sticky_wall_position, led_display.rows - 2, choose_back_frame);

  return sticky_wall_position;
}

/*******************************
 * Places a ball on the screen.
 ******************************/
void place_ball(LedMatrixDisplay led_display, pixel_color rgb, gfx__cursor start_cursor, bool is_player,
                bool choose_back_frame) {
  /*******************************************
   *      x
   *     xxx   << This is the shape of a ball.
   *      x
   *
   * The start cursor is the center of the ball.
   *
   * It has a enum property of PLAYER_BALL.
   ********************************************/
  volatile pixel_color new_ball = rgb;
  if (is_player) {
    new_ball.event = PLAYER_BALL;
  } else {
    new_ball.event = STICKY_BALL;
  }
  modify_pixel(led_display, new_ball, start_cursor.x + 0, start_cursor.y + 1, choose_back_frame);
  modify_pixel(led_display, new_ball, start_cursor.x - 1, start_cursor.y + 0, choose_back_frame);
  modify_pixel(led_display, new_ball, start_cursor.x + 0, start_cursor.y + 0, choose_back_frame);
  modify_pixel(led_display, new_ball, start_cursor.x + 1, start_cursor.y + 0, choose_back_frame);
  modify_pixel(led_display, new_ball, start_cursor.x + 0, start_cursor.y - 1, choose_back_frame);
}

/************************************
 * Removes the ball from the screen.
 *
 * The start cursor is the center
 * of the ball.
 *
 * NOTE:
 * See the place_ball() function.
 ***********************************/
void remove_ball(LedMatrixDisplay led_display, gfx__cursor start_cursor, bool choose_back_frame) {
  reset_pixel(led_display, start_cursor.x + 0, start_cursor.y + 1, choose_back_frame);
  reset_pixel(led_display, start_cursor.x - 1, start_cursor.y + 0, choose_back_frame);
  reset_pixel(led_display, start_cursor.x + 0, start_cursor.y + 0, choose_back_frame);
  reset_pixel(led_display, start_cursor.x + 1, start_cursor.y + 0, choose_back_frame);
  reset_pixel(led_display, start_cursor.x + 0, start_cursor.y - 1, choose_back_frame);
}

/************************************************************
 * Draws the Turn, High Score and Level # for a given level.
 *
 * NOTE:
 * Ensure you set flag new_data = true if you want to refresh
 * any of these values.
 ************************************************************/
void draw_level_stats(LedMatrixDisplay led_display, game_level_state current_game_state, bool new_data,
                      bool choose_back_frame) {
  if (new_data) {
    for (int i = 1; i < 31; ++i) {
      for (int j = 50; j < 63; ++j) {
        reset_pixel(led_display, i, j, choose_back_frame);
      }
    }
  }

  pixel_color text_color = wheel(5); // Yellow.
  gfx__cursor location = {3, 58};

  // Turn = "T" on top-left corner.
  int text_length = 2;
  char turn_text[] = "T";
  gfx_printline_characters(led_display, turn_text, text_length, location, ROTATION_90, text_color, false, false,
                           choose_back_frame);

  /**********************************************
   * Add Hi Score value to the top-right corner.
   *********************************************/
  text_color = wheel(0); // Blue
  location.x = 12;
  location.y = 58;

  if (current_game_state.score >= 10000) {
    char current_score[6];
    sprintf(current_score, "%d", current_game_state.score);
    gfx_printline_characters(led_display, current_score, 6, location, ROTATION_90, text_color, true, false,
                             choose_back_frame);
  } else if (current_game_state.score < 10) {
    char current_score[2];
    sprintf(current_score, "%d", current_game_state.score);
    gfx_printline_characters(led_display, current_score, 2, location, ROTATION_90, text_color, true, false,
                             choose_back_frame);
  } else if (current_game_state.score < 100) {
    char current_score[3];
    sprintf(current_score, "%d", current_game_state.score);
    gfx_printline_characters(led_display, current_score, 3, location, ROTATION_90, text_color, true, false,
                             choose_back_frame);
  } else if (current_game_state.score < 1000) {
    char current_score[4];
    sprintf(current_score, "%d", current_game_state.score);
    gfx_printline_characters(led_display, current_score, 4, location, ROTATION_90, text_color, true, false,
                             choose_back_frame);
  } else if (current_game_state.score < 10000) {
    char current_score[5];
    sprintf(current_score, "%d", current_game_state.score);
    gfx_printline_characters(led_display, current_score, 5, location, ROTATION_90, text_color, true, false,
                             choose_back_frame);
  }

  // Turn number is just under the Turn "T" text.
  text_color = wheel(0); // Blue
  location.x = 2;
  location.y = 52;
  text_length = 3;
  char current_turn[3];
  sprintf(current_turn, "%d", current_game_state.turn);
  gfx_printline_characters(led_display, current_turn, text_length, location, ROTATION_90, text_color, true, false,
                           choose_back_frame);

  // Display the Level Text.
  text_color = wheel(5); // Yellow
  location.x = 14;
  location.y = 52;
  text_length = 4;
  char level_text[] = "LVL";
  gfx_printline_characters(led_display, level_text, text_length, location, ROTATION_90, text_color, true, false,
                           choose_back_frame);

  // Display the Level #.
  text_color = wheel(0); // Blue
  location.x = 27;
  location.y = 52;
  text_length = 3;
  char current_level[3];
  sprintf(current_level, "%d", current_game_state.level);
  gfx_printline_characters(led_display, current_level, text_length, location, ROTATION_90, text_color, true, false,
                           choose_back_frame);
}

/****************************************************************
 * Places the launcher, fail boundary, cursor and queue objects
 * on the screen.
 *
 * NOTE:
 * This function is standard for every playable level.
 ****************************************************************/
game_level_state game_level_base_player(LedMatrixDisplay led_display, game_level_state current_game_state,
                                        bool choose_back_frame) {
  gfx__cursor location;

  /************************************
   * Place the launcher on the screen.
   ***********************************/
  pixel_color launcher_color = wheel(6); // White.
  gfx__cursor launcher_position = {11, 1};
  place_launcher(led_display, launcher_color, launcher_position, choose_back_frame);
  current_game_state.launcher_color = launcher_color;       // Stored for input control in game_engine.h
  current_game_state.launcher_position = launcher_position; // Stored for input control in game_engine.h

  /***********************************************************
   * Place the player ball inside the launcher on the screen.
   **********************************************************/
  if (current_game_state.ball_movement == STATIONARY && current_game_state.player_ball.properties.event != DESTROYED) {
    location.x = 15;
    location.y = 3;

    pixel_color ball_color = current_game_state.player_ball.properties;
    place_ball(led_display, ball_color, location, true, choose_back_frame);
  }

  /**************************************************************************
   * BALL QUEUE: Place the Ball Queue border and the balls inside the queue.
   **************************************************************************/
  location.x = 22;
  location.y = 0;
  current_game_state.queue_location = location;
  current_game_state.top_of_queue =
      place_ball_queue(led_display, launcher_color, current_game_state.queue_location, choose_back_frame);
  gfx_load_ball_queue(led_display, current_game_state.ball_queue.balls, current_game_state.top_of_queue,
                      choose_back_frame);

  /********************************
   * Place the Shooter (position)
   *******************************/
  current_game_state.input_direction =
      place_shooter(led_display, launcher_position, current_game_state.input_direction, choose_back_frame);

  /****************************
   * Place the Fail boundary.
   ****************************/
  pixel_color fail_boundary_color = wheel(6); // White.
  fail_boundary_color.event = FAILURE_LINE;

  location.x = 1;
  location.y = launcher_position.y + 9;
  current_game_state.failure_line = location; // Set the failure line coordinates for use by game_engine.h
  draw_line(led_display, fail_boundary_color, ROTATION_90, location, 30, choose_back_frame);

  return current_game_state;
}

/****************************************************************
 * Places the turn, score and level text for each game level.
 *
 * Additionally runs the function game_level_base_player().
 *
 * NOTE:
 * This function is standard for every playable level.
 ****************************************************************/
game_level_state game_level_base(LedMatrixDisplay led_display, pixel_color border_color,
                                 game_level_state current_game_state, bool choose_back_frame) {
  gfx__cursor location = {0, 0};
  current_game_state.bouncy_wall_color = border_color;

  /*****************************************************************
   * Draw the borders for the level.
   *****************************************************************/
  // Draw the top-left vertical line.
  draw_line(led_display, border_color, ROTATION_90, location, led_display.rows, choose_back_frame);

  // Draw the top-right vertical line.
  location.x = 0;
  location.y = led_display.columns - 1;
  draw_line(led_display, border_color, ROTATION_90, location, led_display.rows, choose_back_frame);

  // Draw the side walls which will be used to bounch the ball back and forth.
  draw_bouncy_walls(led_display, border_color, choose_back_frame);

  /**************************************************************
   * Draw the border where score, turn and level number are shown.
   **************************************************************/
  location.x = 0;
  location.y = led_display.columns - 15;
  draw_line(led_display, border_color, ROTATION_90, location, led_display.rows, choose_back_frame);

  /***************************************
   * Draw the Turn, Score and Level text.
   **************************************/
  draw_level_stats(led_display, current_game_state, true, choose_back_frame);

  /*****************************************************************
   * Load the launcher, fail boundary, ball queue and the shooter.
   ****************************************************************/
  current_game_state = game_level_base_player(led_display, current_game_state, choose_back_frame);

  return current_game_state;
}

/****************************************************************
 * Shows the high score details on both the splash screen and
 * the game over screens.
 ****************************************************************/
void game_show_high_score_details(LedMatrixDisplay led_display, gfx__cursor location, pixel_color text_color,
                                  uint16_t highscore, bool choose_back_frame) {
  /********************
   * Add Hi Score text.
   ********************/
  int text_length = 9;
  char hi_score_label[] = "HI SCORE";
  gfx_printline_characters(led_display, hi_score_label, text_length, location, ROTATION_90, text_color, true, false,
                           choose_back_frame);

  /*********************
   * Add Hi Score value.
   *********************/
  if (highscore >= 10000) {
    location.x += 4;
    location.y -= 6;
    char high_score_value[6];
    sprintf(high_score_value, "%d", highscore);
    gfx_printline_characters(led_display, high_score_value, 6, location, ROTATION_90, text_color, true, false,
                             choose_back_frame);
  } else if (highscore < 10) {
    location.x += 12;
    location.y -= 6;
    char high_score_value[2];
    sprintf(high_score_value, "%d", highscore);
    gfx_printline_characters(led_display, high_score_value, 2, location, ROTATION_90, text_color, true, false,
                             choose_back_frame);
  } else if (highscore < 100) {
    location.x += 9;
    location.y -= 6;
    char high_score_value[3];
    sprintf(high_score_value, "%d", highscore);
    gfx_printline_characters(led_display, high_score_value, 3, location, ROTATION_90, text_color, true, false,
                             choose_back_frame);
  } else if (highscore < 1000) {
    location.x += 6;
    location.y -= 6;
    char high_score_value[4];
    sprintf(high_score_value, "%d", highscore);
    gfx_printline_characters(led_display, high_score_value, 4, location, ROTATION_90, text_color, true, false,
                             choose_back_frame);
  } else if (highscore < 10000) {
    location.x += 4;
    location.y -= 6;
    char high_score_value[5];
    sprintf(high_score_value, "%d", highscore);
    gfx_printline_characters(led_display, high_score_value, 5, location, ROTATION_90, text_color, true, false,
                             choose_back_frame);
  }
}

/*****************************************************
 * Generates the splash screen/main menu for the game.
 *
 * NOTE:
 * The high score must be pulled from the SD
 * Card, or we can pass it to this function.
 *****************************************************/
game_level_state game_splash_screen(LedMatrixDisplay led_display, game_level_state current_game_state,
                                    bool choose_back_frame) {
  uint8_t display_max_length = led_display.rows;
  uint8_t display_max_width = led_display.columns;
  pixel_color text_color;
  gfx__cursor location;

  // Normally we may clear the entire screen. However, that might be too costly to do.
  if (current_game_state.input_direction.direction != NONE) {
    // Clear a small section of the screen only when the shooter is intended to move.
    for (int i = 1; i < 31; ++i) {
      for (int j = 1; j < 31; ++j) {
        reset_pixel(led_display, i, j, choose_back_frame);
      }
    }
  }

  /*****************************************************************
   * Draw the borders for the splash screen.
   *****************************************************************/
  pixel_color border = wheel(0); // Blue
  location.x = 0;
  location.y = 0;

  // Draw the top-left vertical and horizontal lines.
  draw_line(led_display, border, ROTATION_90, location, display_max_length, choose_back_frame);
  draw_line(led_display, border, ROTATION_0, location, display_max_width, choose_back_frame);

  // Draw the top-right vertical line.
  location.x = 0;
  location.y = display_max_width - 1;
  draw_line(led_display, border, ROTATION_90, location, display_max_length, choose_back_frame);

  // Draw the bottom-left horizontal line.
  location.x = display_max_length - 1;
  location.y = 0;
  draw_line(led_display, border, ROTATION_0, location, display_max_width, choose_back_frame);

  /********************************
   * Draw the fill area (for text)
   *********************************/
  pixel_color fill_color = wheel(3); // Red.
  location.x = 1;
  location.y = 45;
  draw_area(led_display, fill_color, 30, 18, location, choose_back_frame);

  /***************************************************
   * Add game name text. Orientation will be vertical.
   ***************************************************/
  text_color = wheel(6); // White.
  location.x = 2;
  location.y = 56;

  int text_length = 16;
  char game_name[] = "BUBBLE^SHOOTER!";
  gfx_printline_characters(led_display, game_name, text_length, location, ROTATION_90, text_color, true, false,
                           choose_back_frame);

  /********************************************
   * Add the Play text.
   *
   * Only on the back frame we show the text.
   * This simulates the Play blinking.
   *******************************************/
  if (choose_back_frame) {
    text_color = wheel(2); // Cyan.
    location.x = 8;
    location.y = 37;

    text_length = 5;
    char play_text[] = "PLAY";
    gfx_printline_characters(led_display, play_text, text_length, location, ROTATION_90, text_color, true, false,
                             choose_back_frame);
  }

  /*******************************
   * Show High Score text (full)
   ******************************/
  text_color = wheel(5); // Yellow
  location.x = 2;
  location.y = 20;
  game_show_high_score_details(led_display, location, text_color, current_game_state.score, choose_back_frame);

  /*******************************************************************
   * Load the launcher, fail boundary, ball queue and the shooter.
   ******************************************************************/
  current_game_state = game_level_base_player(led_display, current_game_state, choose_back_frame);

  /***************************************************
   * Show the game credits on the bottom left corner.
   ***************************************************/
  text_color = wheel(4); // Magenta
  location.x = 12;
  location.y = 26;
  text_length = 3;
  char player_text[] = "P1";
  gfx_printline_characters(led_display, player_text, text_length, location, ROTATION_90, text_color, true, false,
                           choose_back_frame);

  text_color = wheel(3); // Red
  location.x = 2;
  location.y = 3;
  char player_credits[1];
  if (current_game_state.credits > 9) {
    current_game_state.credits = 9;
  }
  sprintf(player_credits, "%d", current_game_state.credits);
  gfx_printline_characters(led_display, player_credits, 3, location, ROTATION_90, text_color, true, false,
                           choose_back_frame);

  return current_game_state;
}

/*************************************************
 * Generates the game over screen for the game.
 *
 * NOTE:
 * The high score must be pulled from the SD
 * Card, or we can pass it to this function.
 *************************************************/
void game_gameover(LedMatrixDisplay led_display, uint16_t highscore, bool choose_back_frame) {
  uint8_t display_max_length = led_display.rows;
  uint8_t display_max_width = led_display.columns;

  /********************************************
   * Draw the borders for the game over screen.
   *********************************************/
  pixel_color border = wheel(3); // Red.
  gfx__cursor location = {0, 0};

  // Draw the top-left vertical and horizontal lines.
  draw_line(led_display, border, ROTATION_90, location, display_max_length, choose_back_frame);
  draw_line(led_display, border, ROTATION_0, location, display_max_width, choose_back_frame);

  // Draw the top-right vertical line.
  location.x = 0;
  location.y = display_max_width - 1;
  draw_line(led_display, border, ROTATION_90, location, display_max_length, choose_back_frame);

  // Draw the bottom-left horizontal line.
  location.x = display_max_length - 1;
  location.y = 0;
  draw_line(led_display, border, ROTATION_0, location, display_max_width, choose_back_frame);

  /********************************
   * Display the gameover text.
   ********************************/
  pixel_color text_color = wheel(3); // Cyan.
  location.x = 8;
  location.y = 37;

  int text_length = 10;
  char gameover_text[] = "GAME^OVER!";
  gfx_printline_characters(led_display, gameover_text, text_length, location, ROTATION_90, text_color, true, false,
                           choose_back_frame);

  /*******************************
   * Show High Score text (full)
   ******************************/
  text_color = wheel(0); // Blue
  location.x = 2;
  location.y = 19;
  game_show_high_score_details(led_display, location, text_color, highscore, choose_back_frame);
}

/*****************************************************
 * Generates the pause menu on a region of the game.
 *
 * NOTE:
 * You should save the previous frame in case the
 * user wants to resume the game.
 *****************************************************/
void game_pause_menu(LedMatrixDisplay led_display, pause_menu_actions pause_action, bool choose_back_frame) {
  // TODO: Display a pause menu with a very small region with 3 options and an arrow.
  uint8_t display_max_length = led_display.rows;
  uint8_t display_max_width = led_display.columns;
  gfx__cursor location = {4, 30};

  // Save the contents of the memory location in case the menu needs to be unpaused.
  // At the same time, erase the pixels at the given location.
  if (!pause_action.selected) {
    for (int i = 0; i < 24; ++i) {
      for (int j = 0; j < 15; ++j) {
        uint8_t x = i + location.x;
        uint8_t y = j + location.y;

        pixel_color modified_pixel = reset_pixel(led_display, x, y, choose_back_frame);

        // Only save the background graphics once in case the player wants to unpause the game/level.
        if (!pause_action.pause_loaded) {
          background_gfx[i][j] = modified_pixel;
        }
      }
    }

    // If the pause screen was not loaded yet, draw all the pixels.
    if (!pause_action.pause_loaded) {
      /*********************************************
       * Draw the borders for the pause menu.
       *
       * NOTE: This can overlay above the level.
       *********************************************/
      pixel_color border = wheel(0); // Blue.

      // Top-line of the pause menu. Vertical orientation.
      location.x = 4;
      location.y = 45;
      draw_line(led_display, border, ROTATION_90, location, 24, choose_back_frame);

      // Bottom-line of the pause menu. Vertical orientation.
      location.x = 4;
      location.y = 30;
      draw_line(led_display, border, ROTATION_90, location, 24, choose_back_frame);

      // Left-line of the pause menu. Vertical orientation.
      location.x = 4;
      location.y = 30;
      draw_line(led_display, border, ROTATION_0, location, 15, choose_back_frame);

      // Right-line of the pause menu. Vertical orientation.
      location.x = 27;
      location.y = 30;
      draw_line(led_display, border, ROTATION_0, location, 15, choose_back_frame);

      /*****************************************
       * Draw the Pause Text and the 3 options.
       ******************************************/
      pixel_color text_color = wheel(0); // Blue.
      location.x = 7;
      location.y = 40;

      int text_length = 12;
      char pause_text[] = "PAUSE^M X R";
      gfx_printline_characters(led_display, pause_text, text_length, location, ROTATION_90, text_color, true, false,
                               choose_back_frame);

      /************************************************
       * Draw the Indicator given by the option value.
       ***********************************************/
      pixel_color options_color = wheel(3); // Red.
      if (pause_action.option == MAIN_MENU) {
        location.x = 7;
        location.y = 38;
        draw_line(led_display, options_color, ROTATION_90, location, 5, choose_back_frame);

      } else if (pause_action.option == GO_BACK) {
        location.x = 14;
        location.y = 38;
        draw_line(led_display, options_color, ROTATION_90, location, 5, choose_back_frame);
      } else if (pause_action.option == RESTART_LEVEL) {
        location.x = 21;
        location.y = 38;
        draw_line(led_display, options_color, ROTATION_90, location, 3, choose_back_frame);
      }
    }
  } else {
    // If the pause screen was marked as selected, AND go back is chosen, redraw the previous pixels.
    if (pause_action.option == GO_BACK) {
      for (int i = 0; i < 24; ++i) {
        for (int j = 0; j < 15; ++j) {
          uint8_t x = i + location.x;
          uint8_t y = j + location.y;

          modify_pixel(led_display, background_gfx[i][j], x, y, choose_back_frame);
        }
      }
    } else {
      ; // Do something else. When returning to the main menu we can return a struct that triggers the next state, like
        // going back to the main menu, or restarting the level.
    }
  }
}

/*********************************************************
 * Tells you that the next level is about to be loaded.
 *
 * NOTE:
 * This is loaded prior to the next level. Once it is
 * loaded, after about 3 seconds, the level starts.
 **********************************************************/
void game_prepare_for_next_level(LedMatrixDisplay led_display, uint8_t level_number, bool choose_back_frame) {
  // TODO: Display a pause menu with a very small region with 3 options and an arrow.
  uint8_t display_max_length = led_display.rows;
  uint8_t display_max_width = led_display.columns;
  gfx__cursor location = {4, 30};

  // Clear the entire screen.
  clear_screen(led_display, choose_back_frame);

  /********************************************
   * Draw the borders for level preview.
   *********************************************/
  pixel_color border = wheel(6); // White.

  // Top-line of the pause menu. Vertical orientation.
  location.x = 4;
  location.y = 44;
  draw_line(led_display, border, ROTATION_90, location, 24, choose_back_frame);

  // Bottom-line of the pause menu. Vertical orientation.
  location.x = 4;
  location.y = 30;
  draw_line(led_display, border, ROTATION_90, location, 24, choose_back_frame);

  // Left-line of the pause menu. Vertical orientation.
  location.x = 4;
  location.y = 30;
  draw_line(led_display, border, ROTATION_0, location, 14, choose_back_frame);

  // Right-line of the pause menu. Vertical orientation.
  location.x = 27;
  location.y = 30;
  draw_line(led_display, border, ROTATION_0, location, 14, choose_back_frame);

  /***********************
   * Draw the Level text.
   **********************/
  pixel_color text_color = wheel(5); // Yellow.
  location.x = 7;
  location.y = 38;

  int text_length = 6;
  char level_text[] = "LEVEL";
  gfx_printline_characters(led_display, level_text, text_length, location, ROTATION_90, text_color, true, false,
                           choose_back_frame);

  /***********************
   * Draw the Level #
   **********************/
  location.x += 7;
  location.y -= 6;

  // Now assume you have a high score. you can display this high score value by doing a string conversion.
  text_length = 3;
  char level_value[3];
  sprintf(level_value, "%d", level_number);
  gfx_printline_characters(led_display, level_value, text_length, location, ROTATION_90, text_color, true, false,
                           choose_back_frame);
}

/*********************************************************
 * Level 1 (Beginner)
 *
 * PROPERTIES:
 * Ball layout is simple.
 *
 * NOTE:
 * The game engine will control the ball orientation &
 * game logic.
 **********************************************************/
game_level_state game_level1(LedMatrixDisplay led_display, game_level_state current_game_state,
                             bool choose_back_frame) {
  // Below is unique parameters for the level.
  volatile pixel_color level_border_color = wheel(1); // Green
  level_border_color.event = BOUNCY_WALL;
  current_game_state.turn = 0;

  // Define the ball colors for each row.
  pixel_color empty_ball;
  game_sticky_ball_row sticky_ball_row;
  pixel_color all_ball_colors[] = {wheel(0), wheel(5), wheel(3)}; // B,Y,R

  // Assign the queue items the colors.
  current_game_state.player_ball.properties = all_ball_colors[0];
  current_game_state.player_ball.properties.event = PLAYER_BALL;
  current_game_state.ball_queue.balls[0] = all_ball_colors[1];
  current_game_state.ball_queue.balls[0].event = PLAYER_BALL;
  current_game_state.ball_queue.balls[1] = all_ball_colors[2];
  current_game_state.ball_queue.balls[1].event = PLAYER_BALL;

  // Draws the main components of the level, including the level stats (top) & the launcher/queue on the bottom.
  current_game_state = game_level_base(led_display, level_border_color, current_game_state, choose_back_frame);

  // Draw the initial sticky wall.
  gfx__cursor sticky_wall_position = draw_falling_wall(led_display, choose_back_frame);
  current_game_state.sticky_wall_line = sticky_wall_position;

  // Clears any stick balls on the screen. Assume there are no sticky balls at the moment.
  clear_all_sticky_balls();

  sticky_ball_row.sticky_balls_colors[0] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[1] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[2] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[3] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[4] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[5] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[6] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[7] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[8] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[9] = all_ball_colors[0];
  push_sticky_ball_row(led_display, sticky_ball_row, 1, sticky_wall_position, choose_back_frame);

  sticky_ball_row.sticky_balls_colors[0] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[1] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[2] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[3] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[4] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[5] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[6] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[7] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[8] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[9] = all_ball_colors[2];
  push_sticky_ball_row(led_display, sticky_ball_row, 2, sticky_wall_position, choose_back_frame);

  sticky_ball_row.sticky_balls_colors[0] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[1] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[2] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[3] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[4] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[5] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[6] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[7] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[8] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[9] = all_ball_colors[2];
  push_sticky_ball_row(led_display, sticky_ball_row, 3, sticky_wall_position, choose_back_frame);

  sticky_ball_row.sticky_balls_colors[0] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[1] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[2] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[3] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[4] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[5] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[6] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[7] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[8] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[9] = all_ball_colors[0];
  push_sticky_ball_row(led_display, sticky_ball_row, 4, sticky_wall_position, choose_back_frame);

  sticky_ball_row.sticky_balls_colors[0] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[1] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[2] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[3] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[4] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[5] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[6] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[7] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[8] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[9] = all_ball_colors[2];
  push_sticky_ball_row(led_display, sticky_ball_row, 5, sticky_wall_position, choose_back_frame);

  sticky_ball_row.sticky_balls_colors[0] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[1] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[2] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[3] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[4] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[5] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[6] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[7] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[8] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[9] = all_ball_colors[2];
  push_sticky_ball_row(led_display, sticky_ball_row, 6, sticky_wall_position, choose_back_frame);

  sticky_ball_row.sticky_balls_colors[0] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[1] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[2] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[3] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[4] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[5] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[6] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[7] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[8] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[9] = all_ball_colors[2];
  push_sticky_ball_row(led_display, sticky_ball_row, 7, sticky_wall_position, choose_back_frame);

  // Load the sticky balls to both the front and back frame since this is subject to change.
  load_all_sticky_balls(led_display, choose_back_frame);
  load_all_sticky_balls(led_display, !choose_back_frame);

  return current_game_state;
}

/*********************************************************
 * Level 2 (Intermediate)
 *
 * PROPERTIES:
 * Ball layout is more complicated..
 *
 * NOTE:
 * The game engine will control the ball orientation &
 * game logic.
 **********************************************************/
game_level_state game_level2(LedMatrixDisplay led_display, game_level_state current_game_state,
                             bool choose_back_frame) {
  // Below is unique parameters for the level.
  pixel_color level_border_color = wheel(2); // Cyan
  level_border_color.event = BOUNCY_WALL;
  current_game_state.turn = 0;

  // Define the ball colors for each row.
  pixel_color empty_ball;
  game_sticky_ball_row sticky_ball_row;
  pixel_color all_ball_colors[] = {wheel(4), wheel(3), wheel(5), wheel(0), wheel(1)}; // M,R,Y,B,

  // Assign the queue items the colors.
  current_game_state.player_ball.properties = all_ball_colors[0];
  current_game_state.player_ball.properties.event = PLAYER_BALL;
  current_game_state.ball_queue.balls[0] = all_ball_colors[2];
  current_game_state.ball_queue.balls[0].event = PLAYER_BALL;
  current_game_state.ball_queue.balls[1] = all_ball_colors[3];
  current_game_state.ball_queue.balls[1].event = PLAYER_BALL;

  // Draws the main components of the level, including the level stats (top) & the launcher/queue on the bottom.
  current_game_state = game_level_base(led_display, level_border_color, current_game_state, choose_back_frame);

  // Draw the initial sticky wall.
  gfx__cursor sticky_wall_position = draw_falling_wall(led_display, choose_back_frame);
  current_game_state.sticky_wall_line = sticky_wall_position;

  // Clears any stick balls on the screen. Assume there are no sticky balls at the moment.
  clear_all_sticky_balls();

  sticky_ball_row.sticky_balls_colors[0] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[1] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[2] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[3] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[4] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[5] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[6] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[7] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[8] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[9] = all_ball_colors[2];
  push_sticky_ball_row(led_display, sticky_ball_row, 1, sticky_wall_position, choose_back_frame);

  sticky_ball_row.sticky_balls_colors[0] = all_ball_colors[3];
  sticky_ball_row.sticky_balls_colors[1] = all_ball_colors[3];
  sticky_ball_row.sticky_balls_colors[2] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[3] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[4] = all_ball_colors[4];
  sticky_ball_row.sticky_balls_colors[5] = all_ball_colors[4];
  sticky_ball_row.sticky_balls_colors[6] = all_ball_colors[3];
  sticky_ball_row.sticky_balls_colors[7] = all_ball_colors[3];
  sticky_ball_row.sticky_balls_colors[8] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[9] = all_ball_colors[1];
  push_sticky_ball_row(led_display, sticky_ball_row, 2, sticky_wall_position, choose_back_frame);

  sticky_ball_row.sticky_balls_colors[0] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[1] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[2] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[3] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[4] = all_ball_colors[4];
  sticky_ball_row.sticky_balls_colors[5] = all_ball_colors[4];
  sticky_ball_row.sticky_balls_colors[6] = all_ball_colors[3];
  sticky_ball_row.sticky_balls_colors[7] = all_ball_colors[3];
  sticky_ball_row.sticky_balls_colors[8] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[9] = all_ball_colors[1];
  push_sticky_ball_row(led_display, sticky_ball_row, 3, sticky_wall_position, choose_back_frame);

  sticky_ball_row.sticky_balls_colors[0] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[1] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[2] = all_ball_colors[4];
  sticky_ball_row.sticky_balls_colors[3] = all_ball_colors[4];
  sticky_ball_row.sticky_balls_colors[4] = all_ball_colors[3];
  sticky_ball_row.sticky_balls_colors[5] = all_ball_colors[3];
  sticky_ball_row.sticky_balls_colors[6] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[7] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[8] = all_ball_colors[4];
  sticky_ball_row.sticky_balls_colors[9] = all_ball_colors[4];
  push_sticky_ball_row(led_display, sticky_ball_row, 4, sticky_wall_position, choose_back_frame);

  sticky_ball_row.sticky_balls_colors[0] = all_ball_colors[4];
  sticky_ball_row.sticky_balls_colors[1] = all_ball_colors[4];
  sticky_ball_row.sticky_balls_colors[2] = all_ball_colors[3];
  sticky_ball_row.sticky_balls_colors[3] = all_ball_colors[3];
  sticky_ball_row.sticky_balls_colors[4] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[5] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[6] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[7] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[8] = all_ball_colors[4];
  sticky_ball_row.sticky_balls_colors[9] = all_ball_colors[4];
  push_sticky_ball_row(led_display, sticky_ball_row, 5, sticky_wall_position, choose_back_frame);

  sticky_ball_row.sticky_balls_colors[0] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[1] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[2] = all_ball_colors[3];
  sticky_ball_row.sticky_balls_colors[3] = all_ball_colors[3];
  sticky_ball_row.sticky_balls_colors[4] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[5] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[6] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[7] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[8] = all_ball_colors[3];
  sticky_ball_row.sticky_balls_colors[9] = all_ball_colors[3];
  push_sticky_ball_row(led_display, sticky_ball_row, 6, sticky_wall_position, choose_back_frame);

  sticky_ball_row.sticky_balls_colors[0] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[1] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[2] = all_ball_colors[3];
  sticky_ball_row.sticky_balls_colors[3] = all_ball_colors[3];
  sticky_ball_row.sticky_balls_colors[4] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[5] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[6] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[7] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[8] = all_ball_colors[4];
  sticky_ball_row.sticky_balls_colors[9] = all_ball_colors[4];
  push_sticky_ball_row(led_display, sticky_ball_row, 7, sticky_wall_position, choose_back_frame);

  // Load the sticky balls to both the front and back frame since this is subject to change.
  load_all_sticky_balls(led_display, choose_back_frame);
  load_all_sticky_balls(led_display, !choose_back_frame);

  return current_game_state;
}

/*********************************************************
 * Level 3 (Advanced)
 *
 * PROPERTIES:
 * Ball layout is really complicated.
 *
 * NOTE:
 * The game engine will control the ball orientation &
 * game logic.
 **********************************************************/
game_level_state game_level3(LedMatrixDisplay led_display, game_level_state current_game_state,
                             bool choose_back_frame) {
  // Below is unique parameters for the level.
  pixel_color level_border_color = wheel(4); // Magenta
  level_border_color.event = BOUNCY_WALL;
  current_game_state.turn = 0;

  // Define the ball colors for each row.
  pixel_color empty_ball;
  game_sticky_ball_row sticky_ball_row;
  pixel_color all_ball_colors[] = {wheel(3), wheel(0), wheel(1), wheel(2), wheel(5)}; // R,B,G,Y,C

  // Assign the queue items the colors.
  current_game_state.player_ball.properties = all_ball_colors[0];
  current_game_state.player_ball.properties.event = PLAYER_BALL;
  current_game_state.ball_queue.balls[0] = all_ball_colors[2];
  current_game_state.ball_queue.balls[0].event = PLAYER_BALL;
  current_game_state.ball_queue.balls[1] = all_ball_colors[4];
  current_game_state.ball_queue.balls[1].event = PLAYER_BALL;

  // Draws the main components of the level, including the level stats (top) & the launcher/queue on the bottom.
  current_game_state = game_level_base(led_display, level_border_color, current_game_state, choose_back_frame);

  // Draw the initial sticky wall.
  gfx__cursor sticky_wall_position = draw_falling_wall(led_display, choose_back_frame);
  current_game_state.sticky_wall_line = sticky_wall_position;

  // Clears any stick balls on the screen. Assume there are no sticky balls at the moment.
  clear_all_sticky_balls();

  sticky_ball_row.sticky_balls_colors[0] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[1] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[2] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[3] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[4] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[5] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[6] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[7] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[8] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[9] = all_ball_colors[0];
  push_sticky_ball_row(led_display, sticky_ball_row, 1, sticky_wall_position, choose_back_frame);

  sticky_ball_row.sticky_balls_colors[0] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[1] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[2] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[3] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[4] = all_ball_colors[3];
  sticky_ball_row.sticky_balls_colors[5] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[6] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[7] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[8] = all_ball_colors[3];
  sticky_ball_row.sticky_balls_colors[9] = all_ball_colors[3];
  push_sticky_ball_row(led_display, sticky_ball_row, 2, sticky_wall_position, choose_back_frame);

  sticky_ball_row.sticky_balls_colors[0] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[1] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[2] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[3] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[4] = all_ball_colors[3];
  sticky_ball_row.sticky_balls_colors[5] = all_ball_colors[4];
  sticky_ball_row.sticky_balls_colors[6] = all_ball_colors[3];
  sticky_ball_row.sticky_balls_colors[7] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[8] = all_ball_colors[4];
  sticky_ball_row.sticky_balls_colors[9] = all_ball_colors[2];
  push_sticky_ball_row(led_display, sticky_ball_row, 3, sticky_wall_position, choose_back_frame);

  sticky_ball_row.sticky_balls_colors[0] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[1] = all_ball_colors[3];
  sticky_ball_row.sticky_balls_colors[2] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[3] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[4] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[5] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[6] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[7] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[8] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[9] = all_ball_colors[0];
  push_sticky_ball_row(led_display, sticky_ball_row, 4, sticky_wall_position, choose_back_frame);

  sticky_ball_row.sticky_balls_colors[0] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[1] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[2] = all_ball_colors[3];
  sticky_ball_row.sticky_balls_colors[3] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[4] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[5] = all_ball_colors[3];
  sticky_ball_row.sticky_balls_colors[6] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[7] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[8] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[9] = all_ball_colors[1];
  push_sticky_ball_row(led_display, sticky_ball_row, 5, sticky_wall_position, choose_back_frame);

  sticky_ball_row.sticky_balls_colors[0] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[1] = all_ball_colors[4];
  sticky_ball_row.sticky_balls_colors[2] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[3] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[4] = all_ball_colors[3];
  sticky_ball_row.sticky_balls_colors[5] = all_ball_colors[4];
  sticky_ball_row.sticky_balls_colors[6] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[7] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[8] = all_ball_colors[3];
  sticky_ball_row.sticky_balls_colors[9] = all_ball_colors[3];
  push_sticky_ball_row(led_display, sticky_ball_row, 6, sticky_wall_position, choose_back_frame);

  sticky_ball_row.sticky_balls_colors[0] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[1] = all_ball_colors[3];
  sticky_ball_row.sticky_balls_colors[2] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[3] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[4] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[5] = all_ball_colors[1];
  sticky_ball_row.sticky_balls_colors[6] = all_ball_colors[2];
  sticky_ball_row.sticky_balls_colors[7] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[8] = all_ball_colors[0];
  sticky_ball_row.sticky_balls_colors[9] = all_ball_colors[4];
  push_sticky_ball_row(led_display, sticky_ball_row, 7, sticky_wall_position, choose_back_frame);

  // Load the sticky balls to both the front and back frame since this is subject to change.
  load_all_sticky_balls(led_display, choose_back_frame);
  load_all_sticky_balls(led_display, !choose_back_frame);

  return current_game_state;
}

/*********************************************************
 * Win game screen.
 *
 * Displays credits for everyone
 **********************************************************/
void game_won(LedMatrixDisplay led_display, uint16_t highscore, bool choose_back_frame) {
  uint8_t display_max_length = led_display.rows;
  uint8_t display_max_width = led_display.columns;

  /********************************************
   * Draw the borders for the game over screen.
   *********************************************/
  pixel_color border = wheel(1); // Green.
  gfx__cursor location = {0, 0};

  // Draw the top-left vertical and horizontal lines.
  draw_line(led_display, border, ROTATION_90, location, display_max_length, choose_back_frame);
  draw_line(led_display, border, ROTATION_0, location, display_max_width, choose_back_frame);

  // Draw the top-right vertical line.
  location.x = 0;
  location.y = display_max_width - 1;
  draw_line(led_display, border, ROTATION_90, location, display_max_length, choose_back_frame);

  // Draw the bottom-left horizontal line.
  location.x = display_max_length - 1;
  location.y = 0;
  draw_line(led_display, border, ROTATION_0, location, display_max_width, choose_back_frame);

  /*******************************
   * Show High Score text (full)
   ******************************/
  pixel_color text_color = wheel(0); // Blue
  location.x = 2;
  location.y = 57;
  game_show_high_score_details(led_display, location, text_color, highscore, choose_back_frame);

  /********************************
   * Display the credits for the
   * developers of the game.
   ********************************/
  text_color = wheel(6); // White
  location.x = 2;
  location.y = 42;

  int text_length = 9;
  char credits1_text[] = "CREDITS:";
  gfx_printline_characters(led_display, credits1_text, text_length, location, ROTATION_90, text_color, true, false,
                           choose_back_frame);

  location.x = 2;
  location.y = 33;
  text_length = 29;
  char credits2_text[] = "AKSHAT^AMIRAJ^ANIRUDH^HISAAM";
  gfx_printline_characters(led_display, credits2_text, text_length, location, ROTATION_90, text_color, true, true,
                           choose_back_frame);

  location.x = 2;
  location.y = 2;
  text_length = 12;
  char credits3_text[] = "CMPE    244";
  gfx_printline_characters(led_display, credits3_text, text_length, location, ROTATION_90, text_color, false, false,
                           choose_back_frame);
}

/**************************************************************
 * Displays the easter egg once a certain condition is met :)
 *************************************************************/
void easter_egg(LedMatrixDisplay led_display, bool choose_back_frame) {
  // Using modify_pixel, we will draw and object on the screen.
  pixel_color text_color = wheel(4); // Magenta
  gfx__cursor location = {2, 30};
  int text_length = 15;
  char easter_egg_text[] = "YOU^CHEATER!!!";
  gfx_printline_characters(led_display, easter_egg_text, text_length, location, ROTATION_90, text_color, true, false,
                           choose_back_frame);
}