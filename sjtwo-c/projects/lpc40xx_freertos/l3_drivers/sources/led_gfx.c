#include "FreeRTOS.h"
#include "task.h"
#include <delay.h>
#include <led_gfx.h>
#include <ledmatrix.h>
#include <stdint.h>
#include <stdio.h>

// extern volatile bool select_front_frame;

/*****************************************************************
 * Unless we apply the BCM technique, we can only access 7 colors.
 * 0 = Blue
 * 1 = Green
 * 2 = Cyan
 * 3 = Red
 * 4 = Magenta
 * 5 = Yellow
 * 6 = White
 *****************************************************************/
pixel_color wheel(int index) {
  pixel_color colors[] = {{0, 0, 1}, {0, 1, 0}, {0, 1, 1}, {1, 0, 0}, {1, 0, 1}, {1, 1, 0}, {1, 1, 1}};
  return colors[index % 7]; // Cycles colors from colors[0] to colors[6].
}

/*****************************************************************
 * Returns a pixel_color array object representing the character.
 *****************************************************************/
static gfx__character construct_character(LedMatrixDisplay led_matrix_display, char c, pixel_color rgb,
                                          gfx_rotation rotation) {
  gfx__character new_character;
  uint8_t fill_size = 25;
  volatile gfx__cursor *place_pixel_p; // Remove optimizations.

  // Prefill the 7x7 matrix with nothing:
  for (int i = 0; i < SHAPE_LENGTH; ++i) {
    for (int j = 0; j < SHAPE_WIDTH; ++j) {
      new_character.shape[i][j].Red = 0;
      new_character.shape[i][j].Green = 0;
      new_character.shape[i][j].Blue = 0;
    }
  }

  // Find the coordinates for LEDs to light up to form the character shape we desire.
  switch (c) {
  case 'a':
  case 'A':
    fill_size = 10; // The number of pixels that can represent the character.
    if (rotation == ROTATION_0) {
      volatile gfx__cursor place_pixel[] = {{0, 1}, {1, 0}, {1, 2}, {2, 0}, {2, 1},
                                            {2, 2}, {3, 0}, {3, 2}, {4, 0}, {4, 2}};
      place_pixel_p = &place_pixel;

      // Set the cursor for the next character placement.
      new_character.cursor.x = 0;
      new_character.cursor.y = 3;

    } else {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {1, 2},
                                            {1, 4}, {2, 0}, {2, 1}, {2, 2}, {2, 3}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 3;
      new_character.cursor.y = 0;
    }
    break;

  case 'b':
  case 'B':
    fill_size = 12;
    if (rotation == ROTATION_0) {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 2}, {2, 0},
                                            {2, 1}, {3, 0}, {3, 2}, {4, 0}, {4, 1}, {4, 2}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 0;
      new_character.cursor.y = 3;
    } else {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {1, 0},
                                            {1, 2}, {1, 4}, {2, 0}, {2, 1}, {2, 3}, {2, 4}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 3;
      new_character.cursor.y = 0;
    }
    break;

  case 'c':
  case 'C':
    fill_size = 9;
    if (rotation == ROTATION_0) {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 1}, {0, 2}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {4, 1}, {4, 2}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 0;
      new_character.cursor.y = 3;
    } else {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {1, 0}, {1, 4}, {2, 0}, {2, 4}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 3;
      new_character.cursor.y = 0;
    }
    break;

  case 'd':
  case 'D':
    fill_size = 10;
    if (rotation == ROTATION_0) {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 1}, {1, 0}, {1, 2}, {2, 0},
                                            {2, 2}, {3, 0}, {3, 2}, {4, 0}, {4, 1}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 0;
      new_character.cursor.y = 3;
    } else {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4},
                                            {1, 0}, {1, 4}, {2, 1}, {2, 2}, {2, 3}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 3;
      new_character.cursor.y = 0;
    }
    break;

  case 'e':
  case 'E':
    fill_size = 11;
    if (rotation == ROTATION_0) {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 1}, {0, 2}, {1, 0}, {2, 0}, {2, 1},
                                            {2, 2}, {3, 0}, {4, 0}, {4, 1}, {4, 2}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 0;
      new_character.cursor.y = 3;
    } else {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {1, 0},
                                            {1, 2}, {1, 4}, {2, 0}, {2, 2}, {2, 4}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 3;
      new_character.cursor.y = 0;
    }
    break;

  case 'f':
  case 'F':
    fill_size = 9;
    if (rotation == ROTATION_0) {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 1}, {0, 2}, {1, 0}, {2, 0}, {2, 1}, {2, 2}, {3, 0}, {4, 0}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 0;
      new_character.cursor.y = 3;
    } else {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {1, 2}, {1, 4}, {2, 2}, {2, 4}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 3;
      new_character.cursor.y = 0;
    }
    break;

  case 'g':
  case 'G':
    fill_size = 14;
    if (rotation == ROTATION_0) {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {1, 0}, {2, 0}, {2, 2},
                                            {2, 3}, {3, 0}, {3, 3}, {4, 0}, {4, 1}, {4, 2}, {4, 3}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 0;
      new_character.cursor.y = 4;
    } else {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {1, 0}, {1, 4},
                                            {2, 0}, {2, 2}, {2, 4}, {3, 0}, {3, 1}, {3, 2}, {3, 4}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 4;
      new_character.cursor.y = 0;
    }
    break;

  case 'h':
  case 'H':
    fill_size = 11;
    if (rotation == ROTATION_0) {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 2}, {1, 0}, {1, 2}, {2, 0}, {2, 1},
                                            {2, 2}, {3, 0}, {3, 2}, {4, 0}, {4, 2}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 0;
      new_character.cursor.y = 3;
    } else {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {1, 2},
                                            {2, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 3;
      new_character.cursor.y = 0;
    }
    break;

  case 'i':
  case 'I':
    fill_size = 9;
    if (rotation == ROTATION_0) {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 1}, {0, 2}, {1, 1}, {2, 1}, {3, 1}, {4, 0}, {4, 1}, {4, 2}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 0;
      new_character.cursor.y = 3;
    } else {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 4}, {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {2, 0}, {2, 4}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 3;
      new_character.cursor.y = 0;
    }
    break;

  case 'j':
  case 'J':
    fill_size = 7;
    if (rotation == ROTATION_0) {
      volatile gfx__cursor place_pixel[] = {{0, 2}, {1, 2}, {2, 2}, {3, 0}, {3, 2}, {4, 0}, {4, 1}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 0;
      new_character.cursor.y = 3;
    } else {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 1}, {1, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 3;
      new_character.cursor.y = 0;
    }
    break;

  case 'k':
  case 'K':
    fill_size = 9;
    if (rotation == ROTATION_0) {
      volatile gfx__cursor place_pixel[] = {
          {0, 0}, {0, 2}, {1, 0}, {1, 1}, {2, 0}, {3, 0}, {3, 1}, {4, 0}, {4, 2},
      };
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 0;
      new_character.cursor.y = 3;
    } else {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {1, 1}, {1, 3}, {2, 0}, {2, 4}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 3;
      new_character.cursor.y = 0;
    }
    break;

  case 'l':
  case 'L':
    fill_size = 7;
    if (rotation == ROTATION_0) {
      volatile gfx__cursor place_pixel[] = {
          {0, 0}, {1, 0}, {2, 0}, {3, 0}, {4, 0}, {4, 1}, {4, 2},
      };
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 0;
      new_character.cursor.y = 3;
    } else {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {1, 0}, {2, 0}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 3;
      new_character.cursor.y = 0;
    }
    break;

  case 'm':
  case 'M':
    fill_size = 13;
    if (rotation == ROTATION_0) {
      volatile gfx__cursor place_pixel[] = {
          {0, 0}, {0, 4}, {1, 0}, {1, 1}, {1, 3}, {1, 4}, {2, 0}, {2, 2}, {2, 4}, {3, 0}, {3, 4}, {4, 0}, {4, 4},
      };
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 0;
      new_character.cursor.y = 5;
    } else {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {1, 3}, {2, 2},
                                            {3, 3}, {4, 0}, {4, 1}, {4, 2}, {4, 3}, {4, 4}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 5;
      new_character.cursor.y = 0;
    }
    break;

  case 'n':
  case 'N':
    fill_size = 13;
    if (rotation == ROTATION_0) {
      volatile gfx__cursor place_pixel[] = {
          {0, 0}, {0, 4}, {1, 0}, {1, 1}, {1, 4}, {2, 0}, {2, 2}, {2, 4}, {3, 0}, {3, 3}, {3, 4}, {4, 0}, {4, 4},
      };
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 0;
      new_character.cursor.y = 5;
    } else {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {1, 3}, {2, 2},
                                            {3, 1}, {4, 0}, {4, 1}, {4, 2}, {4, 3}, {4, 4}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 5;
      new_character.cursor.y = 0;
    }
    break;

  case 'o':
  case 'O':
    fill_size = 8;
    if (rotation == ROTATION_0) {
      volatile gfx__cursor place_pixel[] = {
          {0, 1}, {1, 0}, {1, 2}, {2, 0}, {2, 2}, {3, 0}, {3, 2}, {4, 1},
      };
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 0;
      new_character.cursor.y = 3;
    } else {
      volatile gfx__cursor place_pixel[] = {{0, 1}, {0, 2}, {0, 3}, {1, 0}, {1, 4}, {2, 1}, {2, 2}, {2, 3}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 3;
      new_character.cursor.y = 0;
    }
    break;

  case 'p':
  case 'P':
    fill_size = 10;
    if (rotation == ROTATION_0) {
      volatile gfx__cursor place_pixel[] = {
          {0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 2}, {2, 0}, {2, 1}, {2, 2}, {3, 0}, {4, 0},
      };
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 0;
      new_character.cursor.y = 3;
    } else {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4},
                                            {1, 2}, {1, 4}, {2, 2}, {2, 3}, {2, 4}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 3;
      new_character.cursor.y = 0;
    }
    break;

  case 'q':
  case 'Q':
    fill_size = 16;
    if (rotation == ROTATION_0) {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {1, 0}, {1, 3}, {2, 0}, {2, 1},
                                            {2, 3}, {3, 0}, {3, 2}, {3, 3}, {4, 0}, {4, 1}, {4, 2}, {4, 3}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 0;
      new_character.cursor.y = 4;
    } else {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {1, 0}, {1, 2}, {1, 4},
                                            {2, 0}, {2, 1}, {2, 4}, {3, 0}, {3, 1}, {3, 2}, {3, 3}, {3, 4}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 4;
      new_character.cursor.y = 0;
    }
    break;

  case 'r':
  case 'R':
    fill_size = 11;
    if (rotation == ROTATION_0) {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 2}, {2, 0},
                                            {2, 1}, {3, 0}, {3, 2}, {4, 0}, {4, 2}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 0;
      new_character.cursor.y = 3;
    } else {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {1, 2},
                                            {1, 4}, {2, 0}, {2, 1}, {2, 3}, {2, 4}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 3;
      new_character.cursor.y = 0;
    }
    break;

  case 's':
  case 'S':
    fill_size = 9;
    if (rotation == ROTATION_0) {
      volatile gfx__cursor place_pixel[] = {{0, 1}, {0, 2}, {1, 0}, {2, 0}, {2, 1}, {2, 2}, {3, 2}, {4, 0}, {4, 1}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 0;
      new_character.cursor.y = 3;
    } else {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 2}, {0, 3}, {1, 0}, {1, 2}, {1, 4}, {2, 1}, {2, 2}, {2, 4}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 3;
      new_character.cursor.y = 0;
    }
    break;

  case 't':
  case 'T':
    fill_size = 7;
    if (rotation == ROTATION_0) {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 1}, {0, 2}, {1, 1}, {2, 1}, {3, 1}, {4, 1}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 0;
      new_character.cursor.y = 3;
    } else {
      volatile gfx__cursor place_pixel[] = {{0, 4}, {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {2, 4}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 3;
      new_character.cursor.y = 0;
    }
    break;

  case 'u':
  case 'U':
    fill_size = 11;
    if (rotation == ROTATION_0) {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 2}, {1, 0}, {1, 2}, {2, 0}, {2, 2},
                                            {3, 0}, {3, 2}, {4, 0}, {4, 1}, {4, 2}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 0;
      new_character.cursor.y = 3;
    } else {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {1, 0},
                                            {2, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 3;
      new_character.cursor.y = 0;
    }
    break;

  case 'v':
  case 'V':
    fill_size = 9;
    if (rotation == ROTATION_0) {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 2}, {1, 0}, {1, 2}, {2, 0}, {2, 2}, {3, 0}, {3, 2}, {4, 1}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 0;
      new_character.cursor.y = 3;
    } else {
      volatile gfx__cursor place_pixel[] = {{0, 1}, {0, 2}, {0, 3}, {0, 4}, {1, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 3;
      new_character.cursor.y = 0;
    }
    break;

  case 'w':
  case 'W':
    fill_size = 13;
    if (rotation == ROTATION_0) {
      volatile gfx__cursor place_pixel[] = {
          {0, 0}, {0, 4}, {1, 0}, {1, 4}, {2, 0}, {2, 2}, {2, 4}, {3, 0}, {3, 1}, {3, 3}, {3, 4}, {4, 0}, {4, 4},
      };
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 0;
      new_character.cursor.y = 5;
    } else {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {1, 1}, {2, 2},
                                            {3, 1}, {4, 0}, {4, 1}, {4, 2}, {4, 3}, {4, 4}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 5;
      new_character.cursor.y = 0;
    }
    break;

  case 'x':
  case 'X':
    fill_size = 9;
    if (rotation == ROTATION_0) {
      volatile gfx__cursor place_pixel[] = {
          {0, 0}, {0, 4}, {1, 1}, {1, 3}, {2, 2}, {3, 1}, {3, 3}, {4, 0}, {4, 4},
      };
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 0;
      new_character.cursor.y = 5;
    } else {
      volatile gfx__cursor place_pixel[] = {
          {0, 0}, {0, 4}, {1, 1}, {1, 3}, {2, 2}, {3, 1}, {3, 3}, {4, 0}, {4, 4},
      };
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 5;
      new_character.cursor.y = 0;
    }
    break;

  case 'y':
    fill_size = 10;
    if (rotation == ROTATION_0) {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 2}, {1, 0}, {1, 2}, {2, 0},
                                            {2, 1}, {2, 2}, {3, 2}, {4, 0}, {4, 1}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 0;
      new_character.cursor.y = 3;
    } else {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 2}, {0, 3}, {0, 4}, {1, 0},
                                            {1, 2}, {2, 1}, {2, 2}, {2, 3}, {2, 4}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 3;
      new_character.cursor.y = 0;
    }
    break;

  case 'Y':
    fill_size = 7;
    if (rotation == ROTATION_0) {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 2}, {1, 0}, {1, 2}, {2, 1}, {3, 1}, {4, 1}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 0;
      new_character.cursor.y = 3;
    } else {
      volatile gfx__cursor place_pixel[] = {{0, 3}, {0, 4}, {1, 0}, {1, 1}, {1, 2}, {2, 3}, {2, 4}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 3;
      new_character.cursor.y = 0;
    }
    break;

  case 'z':
  case 'Z':
    fill_size = 9;
    if (rotation == ROTATION_0) {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 1}, {0, 2}, {1, 2}, {2, 1}, {3, 0}, {4, 0}, {4, 1}, {4, 2}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 0;
      new_character.cursor.y = 3;
    } else {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 1}, {0, 4}, {1, 0}, {1, 2}, {1, 4}, {2, 0}, {2, 3}, {2, 4}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 3;
      new_character.cursor.y = 0;
    }
    break;

  case '0':
    fill_size = 12;
    if (rotation == ROTATION_0) {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 2}, {2, 0},
                                            {2, 2}, {3, 0}, {3, 2}, {4, 0}, {4, 1}, {4, 2}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 0;
      new_character.cursor.y = 3;
    } else {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {1, 0},
                                            {1, 4}, {2, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 3;
      new_character.cursor.y = 0;
    }
    break;

  case '1':
    fill_size = 8;
    if (rotation == ROTATION_0) {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 1}, {1, 1}, {2, 1}, {3, 1}, {4, 0}, {4, 1}, {4, 2}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 0;
      new_character.cursor.y = 3;
    } else {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 4}, {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {2, 0}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 3;
      new_character.cursor.y = 0;
    }
    break;

  case '2':
    fill_size = 11;
    if (rotation == ROTATION_0) {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 1}, {0, 2}, {1, 2}, {2, 0}, {2, 1},
                                            {2, 2}, {3, 0}, {4, 0}, {4, 1}, {4, 2}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 0;
      new_character.cursor.y = 3;
    } else {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 1}, {0, 2}, {0, 4}, {1, 0}, {1, 2},
                                            {1, 4}, {2, 0}, {2, 2}, {2, 3}, {2, 4}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 3;
      new_character.cursor.y = 0;
    }
    break;

  case '3':
    fill_size = 11;
    if (rotation == ROTATION_0) {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 1}, {0, 2}, {1, 2}, {2, 0}, {2, 1},
                                            {2, 2}, {3, 2}, {4, 0}, {4, 1}, {4, 2}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 0;
      new_character.cursor.y = 3;
    } else {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 2}, {0, 4}, {1, 0}, {1, 2}, {1, 4},
                                            {2, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 3;
      new_character.cursor.y = 0;
    }
    break;

  case '4':
    fill_size = 9;
    if (rotation == ROTATION_0) {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 2}, {1, 0}, {1, 2}, {2, 0}, {2, 1}, {2, 2}, {3, 2}, {4, 2}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 0;
      new_character.cursor.y = 3;
    } else {
      volatile gfx__cursor place_pixel[] = {{0, 2}, {0, 3}, {0, 4}, {1, 2}, {2, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 3;
      new_character.cursor.y = 0;
    }
    break;

  case '5':
    fill_size = 11;
    if (rotation == ROTATION_0) {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 1}, {0, 2}, {1, 0}, {2, 0}, {2, 1},
                                            {2, 2}, {3, 2}, {4, 0}, {4, 1}, {4, 2}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 0;
      new_character.cursor.y = 3;
    } else {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 2}, {0, 3}, {0, 4}, {1, 0}, {1, 2},
                                            {1, 4}, {2, 0}, {2, 1}, {2, 2}, {2, 4}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 3;
      new_character.cursor.y = 0;
    }
    break;

  case '6':
    fill_size = 10;
    if (rotation == ROTATION_0) {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {1, 0}, {2, 0}, {2, 1}, {2, 2},
                                            {3, 0}, {3, 2}, {4, 0}, {4, 1}, {4, 2}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 0;
      new_character.cursor.y = 3;
    } else {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4},
                                            {1, 0}, {1, 2}, {2, 0}, {2, 1}, {2, 2}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 3;
      new_character.cursor.y = 0;
    }
    break;

  case '7':
    fill_size = 7;
    if (rotation == ROTATION_0) {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 1}, {0, 2}, {1, 2}, {2, 2}, {3, 2}, {4, 2}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 0;
      new_character.cursor.y = 3;
    } else {
      volatile gfx__cursor place_pixel[] = {{0, 4}, {1, 4}, {2, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 3;
      new_character.cursor.y = 0;
    }
    break;

  case '8':
    fill_size = 13;
    if (rotation == ROTATION_0) {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 2}, {2, 0}, {2, 1},
                                            {2, 2}, {3, 0}, {3, 2}, {4, 0}, {4, 1}, {4, 2}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 0;
      new_character.cursor.y = 3;
    } else {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {1, 0}, {1, 2},
                                            {1, 4}, {2, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 3;
      new_character.cursor.y = 0;
    }
    break;

  case '9':
    fill_size = 10;
    if (rotation == ROTATION_0) {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 1}, {0, 2}, {1, 0}, {1, 2},
                                            {2, 0}, {2, 1}, {2, 2}, {3, 2}, {4, 2}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 0;
      new_character.cursor.y = 3;
    } else {
      volatile gfx__cursor place_pixel[] = {{0, 2}, {0, 3}, {0, 4}, {1, 2}, {1, 4},
                                            {2, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 3;
      new_character.cursor.y = 0;
    }
    break;

  case '!':
    fill_size = 4;
    if (rotation == ROTATION_0) {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {1, 0}, {2, 0}, {4, 0}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 0;
      new_character.cursor.y = 1;
    } else {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 2}, {0, 3}, {0, 4}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 1;
      new_character.cursor.y = 0;
    }
    break;

  case ':':
    fill_size = 2;
    if (rotation == ROTATION_0) {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {2, 0}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 0;
      new_character.cursor.y = 1;
    } else {
      volatile gfx__cursor place_pixel[] = {{0, 1}, {0, 3}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 1;
      new_character.cursor.y = 0;
    }
    break;

  case '?':
    fill_size = 9;
    if (rotation == ROTATION_0) {
      volatile gfx__cursor place_pixel[] = {{0, 0}, {0, 1}, {0, 2}, {1, 2}, {2, 0}, {2, 1}, {2, 2}, {3, 0}, {5, 0}};
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 0;
      new_character.cursor.y = 3;
    } else {
      volatile gfx__cursor place_pixel[] = {
          {0, 0}, {0, 2}, {0, 3}, {0, 5}, {1, 3}, {1, 5}, {2, 3}, {2, 4}, {2, 5},
      };
      place_pixel_p = &place_pixel;
      new_character.cursor.x = 3;
      new_character.cursor.y = 0;
    }
    break;

  default:
    // Invalid character, return empty pixel array.
    return new_character;
  }

  for (int j = 0; j < fill_size; ++j) {
    uint8_t x = place_pixel_p[j].x;
    uint8_t y = place_pixel_p[j].y;

    if (x < led_matrix_display.rows && y < led_matrix_display.columns) {
      // printf("Valid pixel: %d,%d\n", x, y);
      new_character.shape[x][y].Red = rgb.Red;
      new_character.shape[x][y].Green = rgb.Green;
      new_character.shape[x][y].Blue = rgb.Blue;
    } else {
      // printf("Invalid pixel: %d,%d\n", x, y);
      continue;
    }
  }
  return new_character;
}

gfx__cursor gfx_print_character(LedMatrixDisplay led_matrix_display, char c, pixel_color rgb, gfx_rotation rotation,
                                gfx__cursor location, bool load_back_frame) {
  // Obtain the 7x7 matrix.
  gfx__character character_to_display = construct_character(led_matrix_display, c, rgb, rotation);

  // Modify the pixels at the given cursor location to for the shape of a character.
  for (int i = 0; i < SHAPE_LENGTH; ++i) {
    for (int j = 0; j < SHAPE_WIDTH; ++j) {
      pixel_color current_pixel = character_to_display.shape[i][j];

      if (current_pixel.Red == 0 && current_pixel.Green == 0 && current_pixel.Blue == 0) {
        continue; // Skip any pixels that are "empty", This allows a background layer to be possible behind character.

      } else {
        uint8_t x = i + location.x;
        uint8_t y = j + location.y;
        modify_pixel(led_matrix_display, rgb, x, y, load_back_frame);
      }
    }
  }

  // Return a cursor to where to place the next object.
  character_to_display.cursor.x += location.x;
  character_to_display.cursor.y += location.y;

  return character_to_display.cursor;
}

/*****************************************************************
 * Displays all characters of a char array either horizontally
 * or vertically.
 *
 * NOTE:
 * Add space: " "
 * Newline character: "^"
 * Supported Letters: ABCDEFGHIJKLMNOPQRSTUVWXYZ
 * Supported Numbers: 0123456789
 * Supported Symbols: :!?
 *
 * Update: Patched 12/13/2020
 *****************************************************************/
gfx__cursor gfx_printline_characters(LedMatrixDisplay led_display, char *characters, int size, gfx__cursor start_cursor,
                                     gfx_rotation rotation, pixel_color rgb, bool add_space, bool color_wheel,
                                     bool choose_back_frame) {
  gfx__cursor next_cursor;
  gfx__cursor initial_position;

  next_cursor = start_cursor;
  for (int i = 0; i < size; ++i) {
    if (i == 0) {
      initial_position.x = next_cursor.x;
      initial_position.y = next_cursor.y;
    }

    // The "^" character acts as the newline character.
    // Will add a space between each character if the flag add_space is enabled.
    if (characters[i] == ' ') {
      if (rotation == ROTATION_0) {
        next_cursor.y = next_cursor.y + 1;
      } else if (rotation == ROTATION_90) {
        next_cursor.x = next_cursor.x + 1;
      }
    } else if (add_space && i > 0) {
      if (rotation == ROTATION_0) {
        if (characters[i - 1] != '^') {
          next_cursor.y = next_cursor.y + 1;
        } else {
          next_cursor.x = next_cursor.x + 7;
          next_cursor.y = initial_position.y;
        }
      } else if (rotation == ROTATION_90) {
        if (characters[i - 1] != '^') {
          next_cursor.x = next_cursor.x + 1;
        } else {
          next_cursor.x = initial_position.x;
          next_cursor.y = next_cursor.y - 7;
        }
      }
    } else if (characters[i] == '^') {
      if (rotation == ROTATION_0) {
        next_cursor.x = next_cursor.x + 7;
        next_cursor.y = initial_position.y;
      } else if (rotation == ROTATION_90) {
        next_cursor.x = initial_position.x;
        next_cursor.y = next_cursor.y - 7;
      }
    }

    // If you want the characters to display in a rainbow, enable the color_wheel flag.
    if (color_wheel) {
      next_cursor = gfx_print_character(led_display, characters[i], wheel(i), rotation, next_cursor, choose_back_frame);
    } else {
      next_cursor = gfx_print_character(led_display, characters[i], rgb, rotation, next_cursor, choose_back_frame);
    }
  }

  return next_cursor;
}

/********************************************************************
 * Draw a vertical or horizontal line on the display with a single
 * color or property.
 *
 * NOTE:
 * Technically could implement a color wheel using wheel(integer),
 * but for now keeping it simple.
 *******************************************************************/
void draw_line(LedMatrixDisplay led_display, pixel_color color, gfx_rotation rotation, gfx__cursor start_cursor,
               uint8_t end_point, bool load_back_frame) {
  if (rotation == ROTATION_0) {
    for (int y = 0; y < end_point; ++y) {
      modify_pixel(led_display, color, start_cursor.x, start_cursor.y + y, load_back_frame);
    }
  } else if (rotation == ROTATION_90) {
    for (int x = 0; x < end_point; ++x) {
      modify_pixel(led_display, color, start_cursor.x + x, start_cursor.y, load_back_frame);
    }
  }
}

/*****************************************************************
 * Fills an entire area of pixels with a single color & property.
 *****************************************************************/
void draw_area(LedMatrixDisplay led_display, pixel_color rgb, uint8_t length, uint8_t width, gfx__cursor start_pos,
               bool choose_back_frame) {
  for (int i = 0; i < length; ++i) {
    for (int j = 0; j < width; ++j) {
      uint8_t x = start_pos.x + i;
      uint8_t y = start_pos.y + j;

      if (x < led_display.rows && y < led_display.columns) {
        modify_pixel(led_display, rgb, x, y, choose_back_frame);
      }
    }
  }
}

/*****************************************************************
 * Moves the pixels at the area from start_pos to the new area
 * given by the end_pos.
 *****************************************************************/
void move_area(LedMatrixDisplay led_display, uint8_t length, uint8_t width, gfx__cursor start_pos, gfx__cursor end_pos,
               bool choose_back_frame) {
  for (int i = 0; i < length; ++i) {
    for (int j = 0; j < width; ++j) {
      uint8_t old_x = start_pos.x + i;
      uint8_t old_y = start_pos.y + j;
      uint8_t new_x = end_pos.x + i;
      uint8_t new_y = end_pos.y + j;

      pixel_color existing_pixel = get_pixel(old_x, old_y, choose_back_frame);

      // Prevent out of bounds crashes from occurring.
      if (old_x < led_display.rows && old_y < led_display.columns && new_x < led_display.rows &&
          new_y < led_display.columns) {
        move_pixel(led_display, existing_pixel, old_x, old_y, new_x, new_y, choose_back_frame);
      }
    }
  }
}

/*****************************************************************
 * Displays all characters on the screen either horizonatally,
 * or vertically. Run this before running the display_frame task.
 *****************************************************************/
void demo_gfx_all_characters(LedMatrixDisplay led_display, pixel_color color, gfx_rotation rotation, bool color_wheel) {
  bool choose_back_frame = false;
  gfx__cursor start_cursor;

  if (rotation == ROTATION_0) {
    start_cursor.x = 0;
    start_cursor.y = 0;
  } else if (rotation == ROTATION_90) {
    start_cursor.x = 0;
    start_cursor.y = 59;
  }

  // Turn off all LEDs on the display.
  clear_screen(led_display, choose_back_frame);

  if (rotation == ROTATION_0) {
    int characters_length = 40;
    char characters[] = "ABCDEFGHIJKLMNOPQRS^TUVWXYZ0123456789!?";
    start_cursor = gfx_printline_characters(led_display, characters, characters_length, start_cursor, rotation, color,
                                            false, color_wheel, choose_back_frame);

    // Now display the Demo text is solid color.
    characters_length = 14;
    char last_characters[] = "^D E M O    0";
    gfx_printline_characters(led_display, last_characters, characters_length, start_cursor, rotation, color, false,
                             false, choose_back_frame);

  } else if (rotation == ROTATION_90) {
    int characters_length = 43;
    char characters[] = "ABCDEFGHIJ^KLMNOPQRS^TUVWXYZ^0123456789^!?";
    start_cursor = gfx_printline_characters(led_display, characters, characters_length, start_cursor, rotation, color,
                                            false, color_wheel, choose_back_frame);

    // Now display the Demo text is solid color.
    characters_length = 15;
    char last_characters[] = "^D E M O    90";
    gfx_printline_characters(led_display, last_characters, characters_length, start_cursor, rotation, color, false,
                             false, choose_back_frame);
  }
}

/*****************************************************************
 * To demo the color wheel, make a new task in main.c and run
 * this in a while loop from 0 to 6.
 *****************************************************************/
void demo_color_wheel(LedMatrixDisplay led_display, uint8_t index, uint16_t milliseconds) {
  bool choose_back_frame = true;

  if (index < 7) {
    fill_screen(led_display, wheel(index), choose_back_frame);
    vTaskDelay(milliseconds);
    fprintf(stderr, "Refreshing LED Display.\n");
    choose_back_frame = gfx_switch_frame();
  }
}

/*****************************************************************
 * Demos shapes. Run this before running the display_frame task.
 *****************************************************************/
void demo_shapes(LedMatrixDisplay led_display, pixel_color color, bool choose_back_frame) {
  // TODO: Make additional shapes such as triangles and circles.
  gfx__cursor location = {0, 0};
  uint8_t display_max_length = led_display.rows;
  uint8_t display_max_width = led_display.columns;

  // Draw the top-left vertical and horizontal lines.
  draw_line(led_display, color, ROTATION_90, location, display_max_length, choose_back_frame);
  draw_line(led_display, color, ROTATION_0, location, display_max_width, choose_back_frame);

  // Draw the top-right vertical line.
  location.x = 0;
  location.y = display_max_width - 1;
  draw_line(led_display, color, ROTATION_90, location, display_max_length, choose_back_frame);

  // Draw the bottom-left horizontal line.
  location.x = display_max_length - 1;
  location.y = 0;
  draw_line(led_display, color, ROTATION_0, location, display_max_width, choose_back_frame);
}