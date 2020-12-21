#pragma once

#include <ledmatrix.h>
#include <stdbool.h>
#include <stdint.h>
#define SHAPE_LENGTH 7
#define SHAPE_WIDTH 7

/*****************************************************************************
 * LED Matrix GFX.
 *
 * This driver contains GFX that can be displayed on the LED matrix such as:
 * - Alphanumeric characters (Az09)
 * - Shapes: Square, Triangle, Circle, Line
 * - Animations: Ball appear, Ball disappear.
 *
 * The placement of the object is determined by the cursor.
 ****************************************************************************/
typedef enum { ROTATION_0 = 0, ROTATION_90 } gfx_rotation;

typedef struct {
  uint8_t x;
  uint8_t y;
} gfx__cursor;

typedef struct {
  pixel_color shape[SHAPE_LENGTH][SHAPE_WIDTH];
  gfx__cursor cursor;
} gfx__character;

pixel_color wheel(int index);
static gfx__character construct_character(LedMatrixDisplay led_matrix_display, char c, pixel_color rgb,
                                          gfx_rotation rotation);
gfx__cursor gfx_print_character(LedMatrixDisplay led_matrix_display, char c, pixel_color rgb, gfx_rotation rotation,
                                gfx__cursor location, bool load_back_frame);
gfx__cursor gfx_printline_characters(LedMatrixDisplay led_display, char *characters, int size, gfx__cursor start_cursor,
                                     gfx_rotation rotation, pixel_color rgb, bool add_space, bool color_wheel,
                                     bool choose_back_frame);
void draw_line(LedMatrixDisplay led_display, pixel_color color, gfx_rotation rotation, gfx__cursor start_cursor,
               uint8_t end_point, bool load_back_frame);
void draw_area(LedMatrixDisplay led_display, pixel_color rgb, uint8_t length, uint8_t width, gfx__cursor start_pos,
               bool choose_back_frame);
void move_area(LedMatrixDisplay led_display, uint8_t length, uint8_t width, gfx__cursor start_pos, gfx__cursor end_pos,
               bool choose_back_frame);
void demo_gfx_all_characters(LedMatrixDisplay led_display, pixel_color color, gfx_rotation rotation, bool color_wheel);
void demo_color_wheel(LedMatrixDisplay led_display, uint8_t index, uint16_t milliseconds);
void demo_shapes(LedMatrixDisplay led_display, pixel_color color, bool load_back_frame);