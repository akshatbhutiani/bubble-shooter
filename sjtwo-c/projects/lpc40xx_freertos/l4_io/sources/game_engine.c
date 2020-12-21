#include <delay.h>
#include <game_engine.h>
#include <game_gfx.h>
#include <game_music.h>
#include <led_gfx.h>
#include <ledmatrix.h>
#include <stdio.h>
#include <time.h>

static volatile gfx__cursor launcher_position;
static volatile game_levels current_level;
static volatile uint16_t highest_score = 0;
static volatile int movement_counter = 0;

// This storage is where the sticky balls will be stored and manipulated.
static volatile gfx__sticky_ball_row sticky_ball_rows[15];

void clear_all_sticky_balls(void) {
  // A 'no sticky ball' is essentially telling us there is no valid sticky ball in that row position.
  gfx__sticky_ball no_sticky_ball;
  no_sticky_ball.color.Red = 0;
  no_sticky_ball.color.Blue = 0;
  no_sticky_ball.color.Green = 0;
  no_sticky_ball.color.event = BACKGROUND_OBJECT;
  no_sticky_ball.center_position.x = 0;
  no_sticky_ball.center_position.y = 0;

  // Mark all the sticky balls in each row as not-valid.
  for (int i = 0; i < 15; ++i) {
    for (int j = 0; j < 10; ++j) {
      sticky_ball_rows[i].sticky_balls[j] = no_sticky_ball;
    }
  }
}

bool sticky_ball_is_valid(gfx__sticky_ball selected_sticky_ball) {
  if (selected_sticky_ball.color.event == STICKY_BALL) {
    return true;
  } else {
    return false;
  }
}

void load_single_sticky_ball(LedMatrixDisplay led_display, gfx__sticky_ball selected_sticky_ball,
                             bool choose_back_frame) {
  bool valid_sticky_ball = sticky_ball_is_valid(selected_sticky_ball);

  if (valid_sticky_ball) {
    place_ball(led_display, selected_sticky_ball.color, selected_sticky_ball.center_position, false, choose_back_frame);
  }
}

void load_all_sticky_balls(LedMatrixDisplay led_display, bool choose_back_frame) {
  for (int i = 0; i < 15; ++i) {
    for (int j = 0; j < 10; ++j) {
      load_single_sticky_ball(led_display, sticky_ball_rows[i].sticky_balls[j], choose_back_frame);
    }
  }
}

/****************************************************
 * Currently only used for setting balls for levels.
 *
 * NOTE:
 * Can possibly use these functions to find balls
 * to destroy upon collision with player ball.
 ****************************************************/
void push_sticky_ball_row(LedMatrixDisplay led_display, game_sticky_ball_row raw_sticky_ball_row, int row,
                          gfx__cursor stick_wall_position, bool choose_back_frame) {
  // Simply add the sticky ball row you desire to the list of sticky ball rows.
  gfx__sticky_ball_row configured_sticky_ball_row;
  if (row > 0 || row < 16) {
    for (int i = 0; i < 10; ++i) {
      volatile int actual_row = row - 1;
      // Set the color of the stick ball as desired. Note that a black (0,0,0) color will make the ball not draw on the
      // screen.
      configured_sticky_ball_row.sticky_balls[i].color = raw_sticky_ball_row.sticky_balls_colors[i];
      if (configured_sticky_ball_row.sticky_balls[i].color.Red == 0 &&
          configured_sticky_ball_row.sticky_balls[i].color.Blue == 0 &&
          configured_sticky_ball_row.sticky_balls[i].color.Green == 0) {
        configured_sticky_ball_row.sticky_balls[i].color.event = BACKGROUND_OBJECT;
      } else {
        configured_sticky_ball_row.sticky_balls[i].color.event = STICKY_BALL;
      }

      if (actual_row % 2 == 0) {
        configured_sticky_ball_row.sticky_balls[i].center_position.x = stick_wall_position.x + 1 + (3 * i);
        configured_sticky_ball_row.sticky_balls[i].center_position.y = stick_wall_position.y - (2 * actual_row) - 2;
      } else {
        configured_sticky_ball_row.sticky_balls[i].center_position.x = stick_wall_position.x + 2 + (3 * i);
        configured_sticky_ball_row.sticky_balls[i].center_position.y = stick_wall_position.y - (2 * actual_row) - 2;
      }
    }
    sticky_ball_rows[row - 1] = configured_sticky_ball_row;
  }
}

/************************************************************************
 * Checks for sticky ball objects around the vicinity of the player ball.
 *
 * If sticky balls are found around the vicinity of the player ball
 * and the player ball is trying to move to an invalid location,
 * return true, otherwise return false and allow the ball to continue
 * to move.
 *
 * NOTE:
 * The movements below are supported ONLY:
 * UP, UP_LEFT, UP_RIGHT, BOTTOM_LEFT, BOTTOM_RIGHT
 ***********************************************************************/
bool attach_to_sticky_wall(game_level_state game_state, bool choose_back_frame) {
  // Assume that the game state has information of the player ball as well as the current direction it is traveling.
  // Check in a ring around the player ball for any sticky balls.
  gfx__cursor player_ball_position = game_state.player_ball.center_of_ball;

  if (game_state.ball_movement == UP) {
    gfx__cursor vicinity1 = {game_state.player_ball.center_of_ball.x, game_state.player_ball.center_of_ball.y + 2};
    pixel_color vicinity1_color = get_pixel(vicinity1.x, vicinity1.y, choose_back_frame);
    // printf("(UP) Vicinity (%d,%d) type=[%d], color=(%d,%d,%d)\n", vicinity1.x, vicinity1.y, vicinity1_color.event,
    //       vicinity1_color.Red, vicinity1_color.Green, vicinity1_color.Blue);

    if (vicinity1_color.event == STICKY_BALL) {
      return true;
    } else {
      return false;
    }
  } else if (game_state.ball_movement == UP_LEFT) {
    gfx__cursor vicinity1 = {game_state.player_ball.center_of_ball.x - 2, game_state.player_ball.center_of_ball.y + 1};
    gfx__cursor vicinity2 = {game_state.player_ball.center_of_ball.x - 1, game_state.player_ball.center_of_ball.y + 2};
    gfx__cursor vicinity3 = {game_state.player_ball.center_of_ball.x - 1, game_state.player_ball.center_of_ball.y + 1};
    pixel_color vicinity1_color = get_pixel(vicinity1.x, vicinity1.y, choose_back_frame);
    pixel_color vicinity2_color = get_pixel(vicinity2.x, vicinity2.y, choose_back_frame);
    pixel_color vicinity3_color = get_pixel(vicinity3.x, vicinity3.y, choose_back_frame);

    if (vicinity1_color.event == STICKY_BALL || vicinity2_color.event == STICKY_BALL ||
        vicinity3_color.event == STICKY_BALL) {
      return true;
    } else {
      return false;
    }
  } else if (game_state.ball_movement == UP_RIGHT) {
    gfx__cursor vicinity1 = {game_state.player_ball.center_of_ball.x + 1, game_state.player_ball.center_of_ball.y + 2};
    gfx__cursor vicinity2 = {game_state.player_ball.center_of_ball.x + 1, game_state.player_ball.center_of_ball.y + 1};
    gfx__cursor vicinity3 = {game_state.player_ball.center_of_ball.x + 2, game_state.player_ball.center_of_ball.y + 1};
    pixel_color vicinity1_color = get_pixel(vicinity1.x, vicinity1.y, choose_back_frame);
    pixel_color vicinity2_color = get_pixel(vicinity2.x, vicinity2.y, choose_back_frame);
    pixel_color vicinity3_color = get_pixel(vicinity3.x, vicinity3.y, choose_back_frame);

    if (vicinity1_color.event == STICKY_BALL || vicinity2_color.event == STICKY_BALL ||
        vicinity3_color.event == STICKY_BALL) {
      return true;
    } else {
      return false;
    }
  } else if (game_state.ball_movement == BOTTOM_LEFT) {
    gfx__cursor vicinity1 = {game_state.player_ball.center_of_ball.x - 1, game_state.player_ball.center_of_ball.y - 1};
    gfx__cursor vicinity2 = {game_state.player_ball.center_of_ball.x - 2, game_state.player_ball.center_of_ball.y - 1};
    gfx__cursor vicinity3 = {game_state.player_ball.center_of_ball.x - 1, game_state.player_ball.center_of_ball.y - 2};
    pixel_color vicinity1_color = get_pixel(vicinity1.x, vicinity1.y, choose_back_frame);
    pixel_color vicinity2_color = get_pixel(vicinity2.x, vicinity2.y, choose_back_frame);
    pixel_color vicinity3_color = get_pixel(vicinity3.x, vicinity3.y, choose_back_frame);

    if (vicinity1_color.event == STICKY_BALL || vicinity2_color.event == STICKY_BALL ||
        vicinity3_color.event == STICKY_BALL) {
      return true;
    } else {
      return false;
    }
  } else if (game_state.ball_movement == BOTTOM_RIGHT) {
    gfx__cursor vicinity1 = {game_state.player_ball.center_of_ball.x + 1, game_state.player_ball.center_of_ball.y - 1};
    gfx__cursor vicinity2 = {game_state.player_ball.center_of_ball.x + 2, game_state.player_ball.center_of_ball.y - 1};
    gfx__cursor vicinity3 = {game_state.player_ball.center_of_ball.x + 1, game_state.player_ball.center_of_ball.y - 2};
    pixel_color vicinity1_color = get_pixel(vicinity1.x, vicinity1.y, choose_back_frame);
    pixel_color vicinity2_color = get_pixel(vicinity2.x, vicinity2.y, choose_back_frame);
    pixel_color vicinity3_color = get_pixel(vicinity3.x, vicinity3.y, choose_back_frame);

    if (vicinity1_color.event == STICKY_BALL || vicinity2_color.event == STICKY_BALL ||
        vicinity3_color.event == STICKY_BALL) {
      return true;
    } else {
      return false;
    }
  } else if (game_state.ball_movement == BOTTOM) {
    gfx__cursor vicinity1 = {game_state.player_ball.center_of_ball.x, game_state.player_ball.center_of_ball.y - 2};
    pixel_color vicinity1_color = get_pixel(vicinity1.x, vicinity1.y, choose_back_frame);

    if (vicinity1_color.event == STICKY_BALL) {
      return true;
    } else {
      return false;
    }
  } else {
    return false;
  }
}

/**********************************************************
 * Draws the launcher (only) on an empty frame.
 *
 * NOTE:
 * Move to game_gfx.h?
 *********************************************************/
gfx__cursor initialize_launcher(LedMatrixDisplay led_display, pixel_color launcher_color, bool choose_back_frame) {
  // Launcher position properties.
  gfx__cursor launcher_position = {11, 1};

  // Draw the launcher
  place_launcher(led_display, launcher_color, launcher_position, choose_back_frame);

  return launcher_position;
}

/**********************************************************
 * Redraws a launcher based on the given input direction.
 *
 * NOTE:
 * Move to game_gfx.h?
 *********************************************************/
game_level_state redraw_launcher_with_inputs(LedMatrixDisplay led_display, game_level_state game_state,
                                             bool choose_back_frame) {
  // See game_gfx.c, Lines 484-492. Clears the screen ONLY if shooter input is not none.
  // Clear a small section of the screen only when the shooter is intended to move.
  for (int i = 1; i < 31; ++i) {
    for (int j = 1; j < 11; ++j) {
      reset_pixel(led_display, i, j, choose_back_frame);
    }
  }

  // Redraw the launcher. Required for new shooter position.
  place_launcher(led_display, game_state.launcher_color, game_state.launcher_position, choose_back_frame);

  // Draw the new shooter.
  game_state.input_direction =
      place_shooter(led_display, game_state.launcher_position, game_state.input_direction, choose_back_frame);

  return game_state;
}

/**********************************************************
 * Checks the fail line for any balls that are sticky.
 *
 * If any balls were found on the line that are sticky,
 * or if the sticky wall is found, return a true
 * indicating game over has occurred. Otherwise return
 * false.
 *********************************************************/
bool check_failure_condition(LedMatrixDisplay led_display, game_level_state game_state, bool choose_back_frame) {
  gfx__cursor fail_line = game_state.failure_line;
  volatile bool game_over = false;
  for (int i = 0; i < 30; ++i) {
    pixel_color failure_line_pixel = get_pixel(fail_line.x + i, fail_line.y, choose_back_frame);
    if (failure_line_pixel.event == STICKY_BALL || failure_line_pixel.event == STICKY_WALL) {
      game_over = true;
      break;
    }
  }
  return game_over;
}

/**********************************************************
 * When this function is called, the entire wall is moved
 * down. This is done on BOTH frames.
 *
 * NOTE:
 * Currently brute forced. Ideally we want to copy pixels
 * from the sticky wall line and tip of longest sticky
 * ball down a few pixels given by decrement_amount.
 *
 * However, the logic might be too time consuming to
 * develop right now.
 *********************************************************/
game_level_state move_sticky_wall(LedMatrixDisplay led_display, game_level_state game_state, uint8_t decrement_amount) {
  gfx__cursor sticky_wall_line = game_state.sticky_wall_line;
  gfx__cursor failure_line = game_state.failure_line;
  gfx__cursor new_sticky_wall_line = {sticky_wall_line.x, sticky_wall_line.y - decrement_amount};

  gfx__cursor previous_location = {failure_line.x - 1, failure_line.y + 1};
  gfx__cursor shifted_location = {failure_line.x - 1, failure_line.y + 1 - decrement_amount};

  uint8_t area_length = 32;
  uint8_t area_width = (sticky_wall_line.y - failure_line.y);

  // When we move the pixels, the previous pixels get removed completely.
  // So we will need to redraw the necessary pixels later.
  move_area(led_display, area_length, area_width, previous_location, shifted_location, true);
  move_area(led_display, area_length, area_width, previous_location, shifted_location, false);

  // fill the side pixels of the walls with the level color to ensure that the wall decrements down.
  for (int i = 0; i < decrement_amount + 1; ++i) {
    modify_pixel(led_display, game_state.bouncy_wall_color, new_sticky_wall_line.x - 1, new_sticky_wall_line.y + i,
                 true);
    modify_pixel(led_display, game_state.bouncy_wall_color, led_display.rows - 1, new_sticky_wall_line.y + i, true);
    modify_pixel(led_display, game_state.bouncy_wall_color, new_sticky_wall_line.x - 1, new_sticky_wall_line.y + i,
                 false);
    modify_pixel(led_display, game_state.bouncy_wall_color, led_display.rows - 1, new_sticky_wall_line.y + i, false);
  }

  game_state.sticky_wall_line = new_sticky_wall_line;
  return game_state;
}

/*****************************************************
 * Clears all sticky balls on the level.
 *
 * NOTE:
 * This is just to demonstrate game logic is working.
 * It's not meant to be used in the final game.
 *****************************************************/
void destroy_all_sticky_balls(LedMatrixDisplay led_display, game_level_state game_state) {
  int end_row = led_display.rows;
  int start_col = game_state.failure_line.y + 1;
  int end_col = game_state.sticky_wall_line.y;

  // Clear out all the pixels inside the game window.
  for (int i = 0; i < end_row; ++i) {
    for (int j = start_col; j < end_col; ++j) {
      reset_pixel(led_display, i, j, true);
      reset_pixel(led_display, i, j, false);
    }
  }

  // Redraw the bouncy walls.
  draw_bouncy_walls(led_display, game_state.bouncy_wall_color, true);
  draw_bouncy_walls(led_display, game_state.bouncy_wall_color, false);
}

/*****************************************************
 * Obtains a count for the sticky balls in a level.
 *
 * The function scans between the failure line and
 * sticky wall line for any STICKY_BALL pixels.
 *
 * A count of colors for each are obtaining and
 * the top 3 colors will be loaded into the queue.
 *****************************************************/
game_level_state scan_sticky_balls(LedMatrixDisplay led_display, game_level_state game_state, bool choose_back_frame) {
  gfx__cursor sticky_wall_line = game_state.sticky_wall_line;
  gfx__cursor failure_line = game_state.failure_line;

  uint16_t blue_balls, green_balls, cyan_balls, red_balls, magenta_balls, yellow_balls, white_balls;
  volatile uint16_t balls_counts[7] = {0, 0, 0, 0, 0, 0, 0};
  volatile pixel_color observed_pixel;

  int end_row = led_display.rows;
  int start_col = failure_line.y + 1;
  int end_col = sticky_wall_line.y;

  // Get raw pixel counts.
  for (int i = 0; i < end_row; ++i) {
    for (int j = start_col; j < end_col; ++j) {
      observed_pixel = get_pixel(i, j, choose_back_frame);

      for (int k = 0; k < 7; ++k) {
        pixel_color comparison_color = wheel(k);
        if (observed_pixel.Red == comparison_color.Red && observed_pixel.Green == comparison_color.Green &&
            observed_pixel.Blue == comparison_color.Blue) {
          if (observed_pixel.event == STICKY_BALL) {
            ++balls_counts[k];
          }
        }
      }
    }
  }

  // Divide by 5 to obtain the actual counts of the balls.
  for (int i = 0; i < 7; ++i) {
    balls_counts[i] = balls_counts[i] / 5;
  }

  // Find the 3 maxes.
  int most_balls[3];

  // Find first max.
  volatile long max = 0;
  most_balls[0] = 0;
  for (int i = 0; i < 7; i++) {
    if (balls_counts[i] > max) {
      max = balls_counts[i];
      most_balls[0] = i;
    }
  }
  // Find the second max.
  max = 0;
  most_balls[1] = 0;
  for (int i = 0; i < 7; i++) {
    if (i == most_balls[0]) {
      continue;
    }
    if (balls_counts[i] > max) {
      max = balls_counts[i];
      most_balls[1] = i;
    }
  }
  // Find the third max.
  max = 0;
  most_balls[2] = 0;
  for (int i = 0; i < 7; i++) {
    if (i == most_balls[0] || i == most_balls[1]) {
      continue;
    }
    if (balls_counts[i] > max) {
      max = balls_counts[i];
      most_balls[2] = i;
    }
  }

  // Since we found all 3 maxes, we can load those 3 colors into the queue.
  // Add the ball with the most count first (if it is not already there)

  // Move the balls currently in the queue, up by 1.
  game_state.player_ball.properties = game_state.ball_queue.balls[0];
  game_state.player_ball.properties.event = PLAYER_BALL;
  game_state.ball_queue.balls[0] = game_state.ball_queue.balls[1];
  game_state.ball_queue.balls[0].event = PLAYER_BALL;

  // Do some logic to ensure we get a different color for the third ball.
  pixel_color player_balls[] = {wheel(most_balls[0]), wheel(most_balls[1]), wheel(most_balls[2])};

  // Assign the next queue item a color based on the colors of the three most popular balls. If neither is found, assign
  // a random color.
  if (balls_counts[1] == 0) {
    // If only the last ball is left, set the colored ball to be the color matching that ball to win the level (if
    // smart).
    game_state.ball_queue.balls[1] = player_balls[0];
    game_state.ball_queue.balls[1].event = PLAYER_BALL;
  } else {
    if (game_state.ball_queue.balls[0].Red == player_balls[0].Red &&
        game_state.ball_queue.balls[0].Green == player_balls[0].Green &&
        game_state.ball_queue.balls[0].Blue == player_balls[0].Blue) {
      // The 1st queue item is the same color as the most populated ball. Give the color of the 2nd most popular ball.
      game_state.ball_queue.balls[1] = player_balls[1];
      game_state.ball_queue.balls[1].event = PLAYER_BALL;
    } else if (game_state.ball_queue.balls[0].Red == player_balls[1].Red &&
               game_state.ball_queue.balls[0].Green == player_balls[1].Green &&
               game_state.ball_queue.balls[0].Blue == player_balls[1].Blue) {
      // The 1st queue item is the same color as the 2nd most populated ball. Give the color of the 3rd most popular
      // ball.
      game_state.ball_queue.balls[1] = player_balls[2];
      game_state.ball_queue.balls[1].event = PLAYER_BALL;
    } else if (game_state.ball_queue.balls[0].Red == player_balls[2].Red &&
               game_state.ball_queue.balls[0].Green == player_balls[2].Green &&
               game_state.ball_queue.balls[0].Blue == player_balls[2].Blue) {
      // The 1st queue item is the same color as the 3rd most populated ball. Assign a random color.
      uint8_t random_number = (uint8_t)rand();
      int wheel_color = random_number % 6;
      game_state.ball_queue.balls[1] = wheel(wheel_color);
      game_state.ball_queue.balls[1].event = PLAYER_BALL;
    } else {
      // The 1st queue item is not any of the most populated balls, therefore assign to the most populated ball.
      game_state.ball_queue.balls[1] = player_balls[0];
      game_state.ball_queue.balls[1].event = PLAYER_BALL;
    }
  }
  // Check if there are no more balls left over (completed the level)
  game_state.level_complete = true;
  for (int i = 0; i < 7; ++i) {
    if (balls_counts[i] != 0) {
      game_state.level_complete = false;
      break;
    }
  }

  return game_state;
}

/*****************************************************
 * Checks the vicinity of the player ball that
 * just converted to a sticky ball and if matching
 * color was found, destroy EVERY colorored ball
 * on the screen that is a sticky ball matching the
 * color.
 *
 * Additionally, every ball that gets destroyed
 * adds many points to the score.
 *
 * NOTE:
 * Perhaps the more balls that gests destroyed,
 * more the points gets multiplied.
 *****************************************************/
game_level_state destroy_sticky_balls_on_matching_color(LedMatrixDisplay led_display, game_level_state game_state,
                                                        bool choose_back_frame) {
  // Assume that the player ball coordinates are currently located at the location where it became a sticky ball.
  pixel_color player_ball = game_state.player_ball.properties;
  gfx__cursor player_ball_position = game_state.player_ball.center_of_ball;
  // printf("Player ball: %d,%d\n", player_ball_position.x, player_ball_position.y);

  gfx__cursor start_pos = {player_ball_position.x - 2, player_ball_position.y - 2};
  gfx__cursor end_pos = {player_ball_position.x + 2, player_ball_position.y + 2};
  // printf("Start pos: %d,%d\n", start_pos.x, start_pos.y);
  // printf("End pos: %d,%d\n", end_pos.x, end_pos.y);

  // The below function checks the vicinity around the player ball for a sticky ball object. If even one match was
  // found, Set the matching_color_found flag to true. This will tell us to destroy all the colored sticky balls on the
  // screen matching the color.
  volatile uint8_t matches_found = 0;
  for (int i = start_pos.x; i < end_pos.x; ++i) {
    for (int j = start_pos.y; j < end_pos.y; ++j) {
      // printf("Checking: %d,%d\n", i, j);
      pixel_color check_pixel = get_pixel(i, j, choose_back_frame);
      if (check_pixel.Red == player_ball.Red && check_pixel.Green == player_ball.Green &&
          check_pixel.Blue == player_ball.Blue && check_pixel.event == STICKY_BALL) {
        ++matches_found;
      }
#if (0)
      else {
        printf("No matching color (%d,%d,%d), player=%d,%d,%d\n", check_pixel.Red, check_pixel.Green, check_pixel.Blue,
               player_ball.Red, player_ball.Green, player_ball.Blue);
      }
#endif
    }
  }
  matches_found -= 5; // Subtract by 5 to get the true count.
  printf("Matches found = %d\n", matches_found);

  // If a matching color was found, use a similar routine to scan_sticky_balls() function to remove all sticky balls
  // matching the colors provided.
  if (matches_found > 0) {
    gfx__cursor sticky_wall_line = game_state.sticky_wall_line;
    gfx__cursor failure_line = game_state.failure_line;
    int end_row = led_display.rows;
    int start_col = failure_line.y + 1;
    int end_col = sticky_wall_line.y;
    volatile uint16_t score = game_state.score;

    // Remove the pixel who is a STICKY_BALL object and matches the color given by the player ball.
    for (int i = 0; i < end_row; ++i) {
      for (int j = start_col; j < end_col; ++j) {
        pixel_color observed_pixel = get_pixel(i, j, choose_back_frame);
        if (observed_pixel.Red == player_ball.Red && observed_pixel.Green == player_ball.Green &&
            observed_pixel.Blue == player_ball.Blue && observed_pixel.event == STICKY_BALL) {
          reset_pixel(led_display, i, j, true);
          reset_pixel(led_display, i, j, false);
          score += (50 / 5); // Each ball is worth 50 points, so divide by 5 because of 5 pixels.
        }
      }
    }
    game_state.score = score;                             // Add the new score due to balls getting destroyed.
    game_state.player_ball.properties.event == DESTROYED; // May need to be set volatile if it gets ignored.

    // Redraw the bouncy walls.
    draw_bouncy_walls(led_display, game_state.bouncy_wall_color, true);
    draw_bouncy_walls(led_display, game_state.bouncy_wall_color, false);

  } else {
    game_state.player_ball.properties.event == STICKY_BALL;
  }

  return game_state;
}

/**************************************
 * Controls functionality of a level.
 *************************************/
game_level_state play_level(LedMatrixDisplay led_display, game_level_state game_state, bool choose_back_frame) {
  gfx__cursor location;
  gfx__cursor ball_in_launcher = {15, 3}; // This is where to place the ball inside launcher when ball is ready.

  // Check the game inputs. If Button 1 was pressed, launch the ball and wait until the ball returns not_moving.
  if (!game_state.button_presses[0] && game_state.ball_movement == STATIONARY) {
    // Redraw the launcher, shooter, ball and the ball queue.
    game_state = redraw_launcher_with_inputs(led_display, game_state, choose_back_frame);
    place_ball_queue(led_display, game_state.launcher_color, game_state.queue_location, choose_back_frame);
    gfx_load_ball_queue(led_display, game_state.ball_queue.balls, game_state.top_of_queue, choose_back_frame);

    // Place the ball inside the launcher. (Simulates - Ready to deploy)
    place_ball(led_display, game_state.player_ball.properties, ball_in_launcher, true, choose_back_frame);

    // Set the game music to the music track of the level.
    game_state = find_music_track(game_state);
    change_music_track(game_state.music_track_pins, game_state.music_track);

  } else if (game_state.button_presses[0] && game_state.ball_movement == STATIONARY) {
    // Update both frames with the same shooter position. Also update the ball queue.
    game_state = redraw_launcher_with_inputs(led_display, game_state, choose_back_frame);
    place_ball_queue(led_display, game_state.launcher_color, game_state.queue_location, choose_back_frame);
    gfx_load_ball_queue(led_display, game_state.ball_queue.balls, game_state.top_of_queue, choose_back_frame);

    // The first time the button is pressed, start moving the ball. Launcher will no longer be controllable.
    game_state.player_ball.center_of_ball = game_state.input_direction.ball_launch_position;
    place_ball(led_display, game_state.player_ball.properties, game_state.player_ball.center_of_ball, true,
               choose_back_frame);

    // Set the Ball Movement to be the Shooter Direction set by the shooter direction function.
    game_state.ball_movement = game_state.input_direction.shooter_direction;

    // Reset the movement counter so as to accurately move the ball between front and back frames.
    movement_counter = 0;

    // If the button was pressed, set the music track to the ball sound effect.
    change_music_track(game_state.music_track_pins, SFX_BALL_SHOOT);

  } else {
    remove_ball(led_display, ball_in_launcher, choose_back_frame);

    // While the ball is still moving, keep the Ball Shoot SFX playing,
    // or just handle on sister SJ2 board.
    change_music_track(game_state.music_track_pins, SFX_BALL_SHOOT);
  }

  // Used to destroy ball that touches this.
  draw_fail_line(led_display, game_state, choose_back_frame);

  // Assume that the ball is moving, meaning we do not redraw the launcher.
  if (game_state.ball_movement == UP) {
    if (movement_counter == 0) {
      remove_ball(led_display, game_state.player_ball.center_of_ball, choose_back_frame);
    } else {
      gfx__cursor previous_ball_location = game_state.player_ball.center_of_ball;
      previous_ball_location.y -= 1; // Decrement by 1.
      remove_ball(led_display, previous_ball_location, choose_back_frame);
    }

    // Move the ball by incrementing the coordinates of the previous center of the ball.
    // Only place the ball if the pixels to positions above are not belonging to the sticky wall.
    location.x = game_state.player_ball.center_of_ball.x;
    location.y = game_state.player_ball.center_of_ball.y;
    pixel_color scan_pixel_above = get_pixel(location.x, location.y + 2, choose_back_frame);

    // Only if the pixel ahead is not a sticky wall, do you place the ball at the new location.
    if (scan_pixel_above.event != STICKY_WALL) {
      // Only move forward if no sticky balls are found in the vicinity of the ball.
      bool sticky_ball_in_vicinity = attach_to_sticky_wall(game_state, choose_back_frame);

      if (sticky_ball_in_vicinity) {
        // Update both frames or somehow tell the game that you have a new ball and it needs to be drawn every new
        // frame. This is until we have a way to attach this to a list of all sticky balls (maybe a dumb append?)
        place_ball(led_display, game_state.player_ball.properties, game_state.player_ball.center_of_ball, false,
                   choose_back_frame);
        place_ball(led_display, game_state.player_ball.properties, game_state.player_ball.center_of_ball, false,
                   !choose_back_frame);
        game_state.ball_movement = STATIONARY;

        // Also decrement the wall down. Commit to both frames.
        game_state = move_sticky_wall(led_display, game_state, 1);

        // Do an inventory of sticky balls by color and choose the ball colors with the highest ball count.
        game_state = destroy_sticky_balls_on_matching_color(led_display, game_state, choose_back_frame);

        if (game_state.player_ball.properties.event == DESTROYED) {
          change_music_track(game_state.music_track_pins, SFX_BALL_DESTROY);
          game_state.player_ball.properties.event = STICKY_BALL;
        } else {
          game_state = find_music_track(game_state);
          change_music_track(game_state.music_track_pins, game_state.music_track);
        }
        game_state = scan_sticky_balls(led_display, game_state, choose_back_frame);
        place_ball_queue(led_display, game_state.launcher_color, game_state.queue_location, choose_back_frame);
        gfx_load_ball_queue(led_display, game_state.ball_queue.balls, game_state.top_of_queue, choose_back_frame);

        // Update the score and turn. The score in this case should be calculated if ball gets destroyed.
        game_state.turn += 1;
        draw_level_stats(led_display, game_state, true, choose_back_frame);
        draw_level_stats(led_display, game_state, true, !choose_back_frame);

      } else {
        game_state.player_ball.center_of_ball.y += 1;
        place_ball(led_display, game_state.player_ball.properties, game_state.player_ball.center_of_ball, true,
                   choose_back_frame);
        ++movement_counter;
      }
    } else {
      // If the ball touched the sticky wall, it automatically gets destroyed.
      // TODO: If ball is near vicinity (vertically) near a ball, stick the ball close to that.
      game_state.player_ball.properties.event = DESTROYED;
      remove_ball(led_display, game_state.player_ball.center_of_ball, choose_back_frame);
      remove_ball(led_display, game_state.player_ball.center_of_ball, !choose_back_frame);
      game_state.ball_movement = STATIONARY;

      if (game_state.player_ball.properties.event == DESTROYED) {
        change_music_track(game_state.music_track_pins, SFX_BALL_DESTROY);
        game_state.player_ball.properties.event = STICKY_BALL;
      } else {
        game_state = find_music_track(game_state);
        change_music_track(game_state.music_track_pins, game_state.music_track);
      }
      game_state = move_sticky_wall(led_display, game_state, 1);
      game_state = scan_sticky_balls(led_display, game_state, choose_back_frame);
      place_ball_queue(led_display, game_state.launcher_color, game_state.queue_location, choose_back_frame);
      gfx_load_ball_queue(led_display, game_state.ball_queue.balls, game_state.top_of_queue, choose_back_frame);
      game_state.turn += 1;
      draw_level_stats(led_display, game_state, true, choose_back_frame);
      draw_level_stats(led_display, game_state, true, !choose_back_frame);
    }
  } else if (game_state.ball_movement == UP_RIGHT) {
    if (movement_counter == 0) {
      remove_ball(led_display, game_state.player_ball.center_of_ball, choose_back_frame);
    } else {
      gfx__cursor previous_ball_location = game_state.player_ball.center_of_ball;
      previous_ball_location.x -= 1; // Previous ball X-coordinate == x-1.
      previous_ball_location.y -= 1; // Previous ball Y-coordinate == y-1.
      remove_ball(led_display, previous_ball_location, choose_back_frame);
    }
    location.x = game_state.player_ball.center_of_ball.x;
    location.y = game_state.player_ball.center_of_ball.y;
    pixel_color scan_pixel_above = get_pixel(location.x, location.y + 2, choose_back_frame);
    pixel_color scan_pixel_right = get_pixel(location.x + 2, location.y, choose_back_frame);

    if (scan_pixel_right.event == BOUNCY_WALL) {
      game_state.ball_movement = UP_LEFT;
      movement_counter = 0;

      // Redraw the ball at the same location for smooth wall bounch animation.
      place_ball(led_display, game_state.player_ball.properties, game_state.player_ball.center_of_ball, true,
                 choose_back_frame);
      game_state = find_music_track(game_state);
      change_music_track(game_state.music_track_pins, game_state.music_track);

    } else if (scan_pixel_above.event != STICKY_WALL) {
      bool sticky_ball_in_vicinity = attach_to_sticky_wall(game_state, choose_back_frame);

      if (sticky_ball_in_vicinity) {
        place_ball(led_display, game_state.player_ball.properties, game_state.player_ball.center_of_ball, false,
                   choose_back_frame);
        place_ball(led_display, game_state.player_ball.properties, game_state.player_ball.center_of_ball, false,
                   !choose_back_frame);
        game_state.ball_movement = STATIONARY;

        game_state = move_sticky_wall(led_display, game_state, 1);
        game_state = destroy_sticky_balls_on_matching_color(led_display, game_state, choose_back_frame);
        if (game_state.player_ball.properties.event == DESTROYED) {
          change_music_track(game_state.music_track_pins, SFX_BALL_DESTROY);
          game_state.player_ball.properties.event = STICKY_BALL;
        } else {
          game_state = find_music_track(game_state);
          change_music_track(game_state.music_track_pins, game_state.music_track);
        }
        game_state = scan_sticky_balls(led_display, game_state, choose_back_frame);
        place_ball_queue(led_display, game_state.launcher_color, game_state.queue_location, choose_back_frame);
        gfx_load_ball_queue(led_display, game_state.ball_queue.balls, game_state.top_of_queue, choose_back_frame);
        game_state.turn += 1;
        draw_level_stats(led_display, game_state, true, choose_back_frame);
        draw_level_stats(led_display, game_state, true, !choose_back_frame);

      } else {
        game_state.player_ball.center_of_ball.x += 1;
        game_state.player_ball.center_of_ball.y += 1;
        place_ball(led_display, game_state.player_ball.properties, game_state.player_ball.center_of_ball, true,
                   choose_back_frame);
        ++movement_counter;
      }
    } else {
      game_state.player_ball.properties.event = DESTROYED;
      remove_ball(led_display, game_state.player_ball.center_of_ball, choose_back_frame);
      remove_ball(led_display, game_state.player_ball.center_of_ball, !choose_back_frame);
      game_state.ball_movement = STATIONARY;

      if (game_state.player_ball.properties.event == DESTROYED) {
        change_music_track(game_state.music_track_pins, SFX_BALL_DESTROY);
        game_state.player_ball.properties.event = STICKY_BALL;
      } else {
        game_state = find_music_track(game_state);
        change_music_track(game_state.music_track_pins, game_state.music_track);
      }
      game_state = move_sticky_wall(led_display, game_state, 1);
      game_state = scan_sticky_balls(led_display, game_state, choose_back_frame);
      place_ball_queue(led_display, game_state.launcher_color, game_state.queue_location, choose_back_frame);
      gfx_load_ball_queue(led_display, game_state.ball_queue.balls, game_state.top_of_queue, choose_back_frame);
      game_state.turn += 1;
      draw_level_stats(led_display, game_state, true, choose_back_frame);
      draw_level_stats(led_display, game_state, true, !choose_back_frame);
    }
  } else if (game_state.ball_movement == UP_LEFT) {
    if (movement_counter == 0) {
      remove_ball(led_display, game_state.player_ball.center_of_ball, choose_back_frame);
    } else {
      gfx__cursor previous_ball_location = game_state.player_ball.center_of_ball;
      previous_ball_location.x += 1; // Previous ball X-coordinate == x+1.
      previous_ball_location.y -= 1; // Previous ball Y-coordinate == y-1.
      remove_ball(led_display, previous_ball_location, choose_back_frame);
    }
    location.x = game_state.player_ball.center_of_ball.x;
    location.y = game_state.player_ball.center_of_ball.y;
    pixel_color scan_pixel_above = get_pixel(location.x, location.y + 2, choose_back_frame);
    pixel_color scan_pixel_left = get_pixel(location.x - 2, location.y, choose_back_frame);

    if (scan_pixel_left.event == BOUNCY_WALL) {
      game_state.ball_movement = UP_RIGHT;
      movement_counter = 0;

      place_ball(led_display, game_state.player_ball.properties, game_state.player_ball.center_of_ball, true,
                 choose_back_frame);
      game_state = find_music_track(game_state);
      change_music_track(game_state.music_track_pins, game_state.music_track);
    } else if (scan_pixel_above.event != STICKY_WALL) {
      bool sticky_ball_in_vicinity = attach_to_sticky_wall(game_state, choose_back_frame);

      if (sticky_ball_in_vicinity) {
        place_ball(led_display, game_state.player_ball.properties, game_state.player_ball.center_of_ball, false,
                   choose_back_frame);
        place_ball(led_display, game_state.player_ball.properties, game_state.player_ball.center_of_ball, false,
                   !choose_back_frame);
        game_state.ball_movement = STATIONARY;

        game_state = move_sticky_wall(led_display, game_state, 1);
        game_state = destroy_sticky_balls_on_matching_color(led_display, game_state, choose_back_frame);
        if (game_state.player_ball.properties.event == DESTROYED) {
          change_music_track(game_state.music_track_pins, SFX_BALL_DESTROY);
          game_state.player_ball.properties.event = STICKY_BALL;
        } else {
          game_state = find_music_track(game_state);
          change_music_track(game_state.music_track_pins, game_state.music_track);
        }
        game_state = scan_sticky_balls(led_display, game_state, choose_back_frame);
        place_ball_queue(led_display, game_state.launcher_color, game_state.queue_location, choose_back_frame);
        gfx_load_ball_queue(led_display, game_state.ball_queue.balls, game_state.top_of_queue, choose_back_frame);
        game_state.turn += 1;
        draw_level_stats(led_display, game_state, true, choose_back_frame);
        draw_level_stats(led_display, game_state, true, !choose_back_frame);

      } else {
        game_state.player_ball.center_of_ball.x -= 1;
        game_state.player_ball.center_of_ball.y += 1;
        place_ball(led_display, game_state.player_ball.properties, game_state.player_ball.center_of_ball, true,
                   choose_back_frame);
        ++movement_counter;
      }

    } else {
      game_state.player_ball.properties.event = DESTROYED;
      remove_ball(led_display, game_state.player_ball.center_of_ball, choose_back_frame);
      remove_ball(led_display, game_state.player_ball.center_of_ball, !choose_back_frame);
      game_state.ball_movement = STATIONARY;
      if (game_state.player_ball.properties.event == DESTROYED) {
        change_music_track(game_state.music_track_pins, SFX_BALL_DESTROY);
        game_state.player_ball.properties.event = STICKY_BALL;
      } else {
        game_state = find_music_track(game_state);
        change_music_track(game_state.music_track_pins, game_state.music_track);
      }
      game_state = move_sticky_wall(led_display, game_state, 1);
      game_state = scan_sticky_balls(led_display, game_state, choose_back_frame);
      place_ball_queue(led_display, game_state.launcher_color, game_state.queue_location, choose_back_frame);
      gfx_load_ball_queue(led_display, game_state.ball_queue.balls, game_state.top_of_queue, choose_back_frame);
      game_state.turn += 1;
      draw_level_stats(led_display, game_state, true, choose_back_frame);
      draw_level_stats(led_display, game_state, true, !choose_back_frame);
    }
  } else if (game_state.ball_movement == BOTTOM_LEFT) {
    // Prevent the ball from going out of bounds and causing a system crash.
    if ((game_state.player_ball.center_of_ball.y - 2) == 0) {
      game_state.player_ball.properties.event = DESTROYED;
      remove_ball(led_display, game_state.player_ball.center_of_ball, choose_back_frame);
      remove_ball(led_display, game_state.player_ball.center_of_ball, !choose_back_frame);
      game_state.ball_movement = STATIONARY;

      if (game_state.player_ball.properties.event == DESTROYED) {
        change_music_track(game_state.music_track_pins, SFX_BALL_DESTROY);
        game_state.player_ball.properties.event = STICKY_BALL;
      } else {
        game_state = find_music_track(game_state);
        change_music_track(game_state.music_track_pins, game_state.music_track);
      }
      game_state = move_sticky_wall(led_display, game_state, 1);
      game_state = scan_sticky_balls(led_display, game_state, choose_back_frame);
      place_ball_queue(led_display, game_state.launcher_color, game_state.queue_location, choose_back_frame);
      gfx_load_ball_queue(led_display, game_state.ball_queue.balls, game_state.top_of_queue, choose_back_frame);
      game_state.turn += 1;
      draw_level_stats(led_display, game_state, true, choose_back_frame);
      draw_level_stats(led_display, game_state, true, !choose_back_frame);
    }

    if (movement_counter == 0) {
      remove_ball(led_display, game_state.player_ball.center_of_ball, choose_back_frame);
    } else {
      gfx__cursor previous_ball_location = game_state.player_ball.center_of_ball;
      previous_ball_location.x += 1; // Previous ball X-coordinate == x+1.
      previous_ball_location.y += 1; // Previous ball Y-coordinate == y+1.
      remove_ball(led_display, previous_ball_location, choose_back_frame);
    }
    location.x = game_state.player_ball.center_of_ball.x;
    location.y = game_state.player_ball.center_of_ball.y;
    pixel_color scan_pixel_above = get_pixel(location.x, location.y + 2, choose_back_frame);
    pixel_color scan_pixel_left = get_pixel(location.x - 2, location.y, choose_back_frame);

    if (scan_pixel_left.event == BOUNCY_WALL) {
      game_state.ball_movement = BOTTOM_RIGHT;
      movement_counter = 0;

      place_ball(led_display, game_state.player_ball.properties, game_state.player_ball.center_of_ball, true,
                 choose_back_frame);
      game_state = find_music_track(game_state);
      change_music_track(game_state.music_track_pins, game_state.music_track);
    } else if (scan_pixel_above.event != STICKY_WALL) {
      bool sticky_ball_in_vicinity = attach_to_sticky_wall(game_state, choose_back_frame);

      if (sticky_ball_in_vicinity) {
        place_ball(led_display, game_state.player_ball.properties, game_state.player_ball.center_of_ball, false,
                   choose_back_frame);
        place_ball(led_display, game_state.player_ball.properties, game_state.player_ball.center_of_ball, false,
                   !choose_back_frame);
        game_state.ball_movement = STATIONARY;

        game_state = move_sticky_wall(led_display, game_state, 1);
        game_state = destroy_sticky_balls_on_matching_color(led_display, game_state, choose_back_frame);
        if (game_state.player_ball.properties.event == DESTROYED) {
          change_music_track(game_state.music_track_pins, SFX_BALL_DESTROY);
          game_state.player_ball.properties.event = STICKY_BALL;
        } else {
          game_state = find_music_track(game_state);
          change_music_track(game_state.music_track_pins, game_state.music_track);
        }
        game_state = scan_sticky_balls(led_display, game_state, choose_back_frame);
        place_ball_queue(led_display, game_state.launcher_color, game_state.queue_location, choose_back_frame);
        gfx_load_ball_queue(led_display, game_state.ball_queue.balls, game_state.top_of_queue, choose_back_frame);
        game_state.turn += 1;
        draw_level_stats(led_display, game_state, true, choose_back_frame);
        draw_level_stats(led_display, game_state, true, !choose_back_frame);

      } else {
        game_state.player_ball.center_of_ball.x -= 1;
        game_state.player_ball.center_of_ball.y -= 1;
        place_ball(led_display, game_state.player_ball.properties, game_state.player_ball.center_of_ball, true,
                   choose_back_frame);
        ++movement_counter;
      }
    } else {
      game_state.player_ball.properties.event = DESTROYED;
      remove_ball(led_display, game_state.player_ball.center_of_ball, choose_back_frame);
      remove_ball(led_display, game_state.player_ball.center_of_ball, !choose_back_frame);
      game_state.ball_movement = STATIONARY;

      if (game_state.player_ball.properties.event == DESTROYED) {
        change_music_track(game_state.music_track_pins, SFX_BALL_DESTROY);
        game_state.player_ball.properties.event = STICKY_BALL;
      } else {
        game_state = find_music_track(game_state);
        change_music_track(game_state.music_track_pins, game_state.music_track);
      }
      game_state = move_sticky_wall(led_display, game_state, 1);
      game_state = scan_sticky_balls(led_display, game_state, choose_back_frame);
      place_ball_queue(led_display, game_state.launcher_color, game_state.queue_location, choose_back_frame);
      gfx_load_ball_queue(led_display, game_state.ball_queue.balls, game_state.top_of_queue, choose_back_frame);
      game_state.turn += 1;
      draw_level_stats(led_display, game_state, true, choose_back_frame);
      draw_level_stats(led_display, game_state, true, !choose_back_frame);
    }
  } else if (game_state.ball_movement == BOTTOM_RIGHT) {
    if ((game_state.player_ball.center_of_ball.y - 2) == 0) {
      game_state.player_ball.properties.event = DESTROYED;
      remove_ball(led_display, game_state.player_ball.center_of_ball, choose_back_frame);
      remove_ball(led_display, game_state.player_ball.center_of_ball, !choose_back_frame);
      game_state.ball_movement = STATIONARY;

      if (game_state.player_ball.properties.event == DESTROYED) {
        change_music_track(game_state.music_track_pins, SFX_BALL_DESTROY);
        game_state.player_ball.properties.event = STICKY_BALL;
      } else {
        game_state = find_music_track(game_state);
        change_music_track(game_state.music_track_pins, game_state.music_track);
      }
      game_state = move_sticky_wall(led_display, game_state, 1);
      game_state = scan_sticky_balls(led_display, game_state, choose_back_frame);
      place_ball_queue(led_display, game_state.launcher_color, game_state.queue_location, choose_back_frame);
      gfx_load_ball_queue(led_display, game_state.ball_queue.balls, game_state.top_of_queue, choose_back_frame);
      game_state.turn += 1;
      draw_level_stats(led_display, game_state, true, choose_back_frame);
      draw_level_stats(led_display, game_state, true, !choose_back_frame);
    }

    if (movement_counter == 0) {
      remove_ball(led_display, game_state.player_ball.center_of_ball, choose_back_frame);
    } else {
      gfx__cursor previous_ball_location = game_state.player_ball.center_of_ball;
      previous_ball_location.x -= 1; // Previous ball X-coordinate == x-1.
      previous_ball_location.y += 1; // Previous ball Y-coordinate == y+1.
      remove_ball(led_display, previous_ball_location, choose_back_frame);
    }
    location.x = game_state.player_ball.center_of_ball.x;
    location.y = game_state.player_ball.center_of_ball.y;
    pixel_color scan_pixel_above = get_pixel(location.x, location.y + 2, choose_back_frame);
    pixel_color scan_pixel_left = get_pixel(location.x - 2, location.y, choose_back_frame);

    if (scan_pixel_left.event == BOUNCY_WALL) {
      game_state.ball_movement = BOTTOM_LEFT;
      movement_counter = 0;

      place_ball(led_display, game_state.player_ball.properties, game_state.player_ball.center_of_ball, true,
                 choose_back_frame);
      game_state = find_music_track(game_state);
      change_music_track(game_state.music_track_pins, game_state.music_track);
    } else if (scan_pixel_above.event != STICKY_WALL) {
      bool sticky_ball_in_vicinity = attach_to_sticky_wall(game_state, choose_back_frame);

      if (sticky_ball_in_vicinity) {
        place_ball(led_display, game_state.player_ball.properties, game_state.player_ball.center_of_ball, false,
                   choose_back_frame);
        place_ball(led_display, game_state.player_ball.properties, game_state.player_ball.center_of_ball, false,
                   !choose_back_frame);
        game_state.ball_movement = STATIONARY;

        game_state = move_sticky_wall(led_display, game_state, 1);
        game_state = destroy_sticky_balls_on_matching_color(led_display, game_state, choose_back_frame);
        if (game_state.player_ball.properties.event == DESTROYED) {
          change_music_track(game_state.music_track_pins, SFX_BALL_DESTROY);
          game_state.player_ball.properties.event = STICKY_BALL;
        } else {
          game_state = find_music_track(game_state);
          change_music_track(game_state.music_track_pins, game_state.music_track);
        }
        game_state = scan_sticky_balls(led_display, game_state, choose_back_frame);
        place_ball_queue(led_display, game_state.launcher_color, game_state.queue_location, choose_back_frame);
        gfx_load_ball_queue(led_display, game_state.ball_queue.balls, game_state.top_of_queue, choose_back_frame);
        game_state.turn += 1;
        draw_level_stats(led_display, game_state, true, choose_back_frame);
        draw_level_stats(led_display, game_state, true, !choose_back_frame);

      } else {
        game_state.player_ball.center_of_ball.x += 1;
        game_state.player_ball.center_of_ball.y -= 1;
        place_ball(led_display, game_state.player_ball.properties, game_state.player_ball.center_of_ball, true,
                   choose_back_frame);
        ++movement_counter;
      }
    } else {
      game_state.player_ball.properties.event = DESTROYED;
      remove_ball(led_display, game_state.player_ball.center_of_ball, choose_back_frame);
      remove_ball(led_display, game_state.player_ball.center_of_ball, !choose_back_frame);
      game_state.ball_movement = STATIONARY;

      if (game_state.player_ball.properties.event == DESTROYED) {
        change_music_track(game_state.music_track_pins, SFX_BALL_DESTROY);
        game_state.player_ball.properties.event = STICKY_BALL;
      } else {
        game_state = find_music_track(game_state);
        change_music_track(game_state.music_track_pins, game_state.music_track);
      }
      game_state = move_sticky_wall(led_display, game_state, 1);
      game_state = scan_sticky_balls(led_display, game_state, choose_back_frame);
      place_ball_queue(led_display, game_state.launcher_color, game_state.queue_location, choose_back_frame);
      gfx_load_ball_queue(led_display, game_state.ball_queue.balls, game_state.top_of_queue, choose_back_frame);
      game_state.turn += 1;
      draw_level_stats(led_display, game_state, true, choose_back_frame);
      draw_level_stats(led_display, game_state, true, !choose_back_frame);
    }
  }

  return game_state;
}

/***********************************
 * State Machine for running the
 * game.
 ***********************************/
game_level_state play_game(LedMatrixDisplay led_display, game_level_state game_state, bool choose_back_frame) {
  current_level = game_state.level;

  // If the current level is being played, check the failure line.
  if (current_level == LEVEL__LEVEL1 || current_level == LEVEL__LEVEL2 || current_level == LEVEL__LEVEL3) {
    bool game_over = check_failure_condition(led_display, game_state, choose_back_frame);
    if (game_over) {
      current_level = LEVEL__GAME_OVER;
      game_state.level = current_level;
      game_state.music_track = MUSIC__NONE;
      change_music_track(game_state.music_track_pins, game_state.music_track);

      clear_screen(led_display, !choose_back_frame);
      game_gameover(led_display, game_state.score, !choose_back_frame);
    }
  }

  // Game credits.
  if (current_level == LEVEL__MAIN_MENU && game_state.button_presses[1]) {
    // Adds credits. Required to start the game.
    game_state.credits += 1;
  }

  if (current_level == LEVEL__GAME_OVER && !game_state.button_presses[0]) {
    clear_screen(led_display, choose_back_frame);
    game_gameover(led_display, game_state.score, choose_back_frame);
    game_state.level = current_level;

    game_state.music_track = SFX_GAME_OVER;
    change_music_track(game_state.music_track_pins, game_state.music_track);

  } else if (current_level == LEVEL__GAME_OVER && game_state.button_presses[0]) {
    current_level = LEVEL__MAIN_MENU;
    game_state.level = current_level;
    game_state.music_track = MUSIC__MAIN_MENU;

    // Clear the currently displaying screen and wait for a few seconds.
    clear_screen(led_display, !choose_back_frame);
    clear_screen(led_display, choose_back_frame);
    change_music_track(game_state.music_track_pins, MUSIC__NONE);
    game_state.credits -= 1;
    delay__ms(500);

  } else if (current_level == LEVEL__MAIN_MENU && !game_state.button_presses[0]) {
    current_level = LEVEL__MAIN_MENU;
    game_state.level = current_level;

    if (highest_score > game_state.score) {
      game_state.score = highest_score;
    }
    clear_screen(led_display, choose_back_frame);
    game_state = game_splash_screen(led_display, game_state, choose_back_frame);

    game_state.music_track = MUSIC__MAIN_MENU;
    change_music_track(game_state.music_track_pins, MUSIC__MAIN_MENU);

  } else if (current_level == LEVEL__MAIN_MENU && game_state.button_presses[0]) {
    if (game_state.credits > 0) {
      current_level = LEVEL__LEVEL1;
      game_state.level = current_level;
      game_state.music_track = MUSIC__NONE;

      // Shows the Next level text screen for 3 seconds.
      clear_screen(led_display, !choose_back_frame);
      game_prepare_for_next_level(led_display, current_level, !choose_back_frame);
      change_music_track(game_state.music_track_pins, MUSIC__NONE);
      delay__ms(3000);

      // Load the entire graphics for the current level.
      game_state.level_complete = false;
      clear_screen(led_display, choose_back_frame);
      game_state.score = 0; // Reset the score so that you can play the game.
      game_state = game_level1(led_display, game_state, choose_back_frame);
      clear_screen(led_display, !choose_back_frame);
      game_state = game_level1(led_display, game_state, !choose_back_frame);
    } else {
      current_level = LEVEL__MAIN_MENU;
      game_state.level = current_level;

      if (highest_score > game_state.score) {
        game_state.score = highest_score;
      }
      clear_screen(led_display, choose_back_frame);
      game_state = game_splash_screen(led_display, game_state, choose_back_frame);

      game_state.music_track = MUSIC__MAIN_MENU;
      change_music_track(game_state.music_track_pins, MUSIC__MAIN_MENU);
    }
  } else if (current_level == LEVEL__LEVEL1) {
    if (game_state.ball_movement == STATIONARY && game_state.button_presses[1]) {
      // For now, let use destroy all the balls on the screen.
      destroy_all_sticky_balls(led_display, game_state);
      game_state = scan_sticky_balls(led_display, game_state, choose_back_frame);
      easter_egg(led_display, !choose_back_frame);
      easter_egg(led_display, choose_back_frame);
      change_music_track(game_state.music_track_pins, game_state.music_track);
      delay__ms(3000);

    } else if (game_state.level_complete) {
      current_level = LEVEL__LEVEL2;
      game_state.level = current_level;
      game_state.music_track = MUSIC__NONE;

      // Shows the Next level text screen for 3 seconds.
      clear_screen(led_display, !choose_back_frame);
      game_prepare_for_next_level(led_display, current_level, !choose_back_frame);
      change_music_track(game_state.music_track_pins, MUSIC__NONE);
      delay__ms(3000);

      // Load the entire graphics for the current level.
      game_state.level_complete = false;
      clear_screen(led_display, choose_back_frame);
      game_state = game_level2(led_display, game_state, choose_back_frame);
      clear_screen(led_display, !choose_back_frame);
      game_state = game_level2(led_display, game_state, !choose_back_frame);
    } else {
      game_state = play_level(led_display, game_state, choose_back_frame);
      current_level = game_state.level;
    }
  } else if (current_level == LEVEL__LEVEL2) {
    if (game_state.ball_movement == STATIONARY && game_state.button_presses[1]) {
      // For now, let use destroy all the balls on the screen.
      destroy_all_sticky_balls(led_display, game_state);
      game_state = scan_sticky_balls(led_display, game_state, choose_back_frame);
      easter_egg(led_display, !choose_back_frame);
      easter_egg(led_display, choose_back_frame);
      change_music_track(game_state.music_track_pins, game_state.music_track);
      delay__ms(3000);

    } else if (game_state.level_complete) {
      current_level = LEVEL__LEVEL3;
      game_state.level = current_level;
      game_state.music_track = MUSIC__NONE;

      // Shows the Next level text screen for 3 seconds.
      clear_screen(led_display, !choose_back_frame);
      game_prepare_for_next_level(led_display, current_level, !choose_back_frame);
      change_music_track(game_state.music_track_pins, MUSIC__NONE);
      delay__ms(3000);

      // Load the entire graphics for the current level.
      game_state.level_complete = false;
      clear_screen(led_display, choose_back_frame);
      game_state = game_level3(led_display, game_state, choose_back_frame);
      clear_screen(led_display, !choose_back_frame);
      game_state = game_level3(led_display, game_state, !choose_back_frame);
    } else {
      game_state = play_level(led_display, game_state, choose_back_frame);
      current_level = game_state.level;
      change_music_track(game_state.music_track_pins, game_state.music_track);
    }
  } else if (current_level == LEVEL__LEVEL3) {
    if (game_state.ball_movement == STATIONARY && game_state.button_presses[1]) {
      // For now, let use destroy all the balls on the screen.
      destroy_all_sticky_balls(led_display, game_state);
      game_state = scan_sticky_balls(led_display, game_state, choose_back_frame);
      easter_egg(led_display, !choose_back_frame);
      easter_egg(led_display, choose_back_frame);
      change_music_track(game_state.music_track_pins, game_state.music_track);
      delay__ms(3000);

    } else if (game_state.level_complete) {
      current_level = LEVEL__GAME_FINISHED;
      game_state.level = current_level;

      clear_screen(led_display, !choose_back_frame);
      change_music_track(game_state.music_track_pins, MUSIC__NONE);
      delay__ms(100);

      clear_screen(led_display, choose_back_frame);
      game_won(led_display, game_state.score, choose_back_frame);
    } else {
      game_state = play_level(led_display, game_state, choose_back_frame);
      current_level = game_state.level;
      change_music_track(game_state.music_track_pins, game_state.music_track);
    }
  } else if (current_level == LEVEL__GAME_FINISHED && !game_state.button_presses[0]) {
    current_level = LEVEL__GAME_FINISHED;
    game_state.level = current_level;
    clear_screen(led_display, choose_back_frame);
    game_won(led_display, game_state.score, choose_back_frame);

    game_state.music_track = MUSIC__NONE;
    change_music_track(game_state.music_track_pins, MUSIC__NONE);

  } else if (current_level == LEVEL__GAME_FINISHED && game_state.button_presses[0]) {
    current_level = LEVEL__MAIN_MENU;
    game_state.level = current_level;

    clear_screen(led_display, !choose_back_frame);
    game_state.music_track = MUSIC__NONE;
    change_music_track(game_state.music_track_pins, MUSIC__NONE);
    game_state.credits -= 1;
    delay__ms(100);

    if (highest_score > game_state.score) {
      game_state.score = highest_score;
    }
    clear_screen(led_display, choose_back_frame);
    game_state = game_splash_screen(led_display, game_state, choose_back_frame);
  }

  return game_state;
}

/*************************************************
 * Loads the music_track parameter to select
 * the background music or SFX on the sister SJ2
 * Board.
 *************************************************/
game_level_state find_music_track(game_level_state game_state) {
  if (game_state.level == LEVEL__MAIN_MENU) {
    game_state.music_track = MUSIC__MAIN_MENU;
  } else if (game_state.level == LEVEL__LEVEL1) {
    game_state.music_track = MUSIC_LEVEL_1;
  } else if (game_state.level == LEVEL__LEVEL2) {
    game_state.music_track = MUSIC_LEVEL_2;
  } else if (game_state.level == LEVEL__LEVEL3) {
    game_state.music_track = MUSIC_LEVEL_3;
  } else if (game_state.level == LEVEL__GAME_OVER) {
    game_state.music_track = SFX_GAME_OVER;
  } else if (game_state.level == LEVEL__GAME_FINISHED) {
    game_state.music_track = MUSIC__NONE;
  }

  return game_state;
}