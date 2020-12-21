#pragma once

#include <gpio.h>
#include <stdbool.h>
#include <stdint.h>

/*****************************************************************************************
 *  LED 32x64 Matrix by Sparkfun.
 *
 * Does not require SPI or I2C as it is not designed to work as a slave device.
 * Instead we must design our own CLK which will run for 32 cycles to control 32 LEDs
 * for each LED Matrix quadrant. On a 32x16 board, for example there is one quadrant,
 * for a 32x32, there are 2, and 32x64 there are four.
 *
 * Programming LEDs on the LED Matrix:
 * ------------------------------------
 * Only 2 full rows can be accessed and modified at one time when STB/Latch goes high,
 * and OE goes low. The Row # is represented by DCBA pins, where D is MSB and A is LSB.
 *
 * Latch/STB pin:
 * --------------
 * The STB/Latch pin is Active High. When programming the Row LED states, STB/Latch
 * must be low, and only after the last clock does it's final rising edge, we set
 * STB/Latch to high, followed by Clock Low.
 *
 * OE pin:
 * --------------
 * According to some limited documentation available online, the displays from Sparkfun
 * utilize an array of MBI5026 chips which use OE' or Active Low. This means that while
 * programming to the Row LED states, OE pin must be high, Latch Low. Once the last clock
 * is finished OE then goes low, followed by Latch going high.
 *
 *****************************************************************************************/

/********************************************************************************
 * Name the object that will be stored at the coordinate which has a pixel_color.
 *
 * For other games, you can change these to represent your game states.
 ********************************************************************************/
typedef enum {
  BACKGROUND_OBJECT = 0,
  STICKY_WALL,
  BOUNCY_WALL,
  FAILURE_LINE,
  STICKY_BALL,
  PLAYER_BALL,
  DESTROYED
} game_event;

typedef struct {
  uint8_t Red;
  uint8_t Green;
  uint8_t Blue;
  game_event event;
} pixel_color;

typedef enum { LED_MATRIX_16x32 = 0, LED_MATRIX_32x32, LED_MATRIX_32x64 } led_matrix_type;

typedef struct {
  led_matrix_type type;
  uint8_t rows;
  uint8_t columns;
  uint8_t quadrants;
} LedMatrixDisplay;

typedef struct {
  gpio_s R0;
  gpio_s G0;
  gpio_s B0;
  gpio_s R1;
  gpio_s G1;
  gpio_s B1;
  gpio_s A;
  gpio_s B;
  gpio_s C;
  gpio_s D;
  gpio_s E;
  gpio_s CLK;
  gpio_s STB_LATCH;
  gpio_s OE;
} led_matrix_pins;

static led_matrix_pins initialize_display_connections(led_matrix_type display_type);
LedMatrixDisplay initialize_display(led_matrix_type display_type);
pixel_color modify_pixel(LedMatrixDisplay led_matrix_display, pixel_color new_pixel_value, int x, int y,
                         bool load_back_frame);
pixel_color reset_pixel(LedMatrixDisplay led_matrix_display, int x, int y, bool load_back_frame);
pixel_color move_pixel(LedMatrixDisplay led_matrix_display, pixel_color new_pixel_value, int old_x, int old_y,
                       int new_x, int new_y, bool load_back_frame);
pixel_color get_pixel(uint8_t x, uint8_t y, bool is_back_frame);
static void CLK_low(void);
static void CLK_high(void);
static void STB_LATCH_low(void);
static void STB_LATCH_high(void);
static void OE_low(void);
static void OE_high(void);
static void extract_value_with_bitmask(gpio_s io_pin, uint8_t value, uint8_t mask);
static bool select_row(LedMatrixDisplay led_matrix_display, uint8_t row_number);
static bool RGB_per_clock(LedMatrixDisplay led_matrix_display, uint8_t quadrant_row, uint8_t column,
                          uint8_t rgb_color_bit);
static bool display_selected_row(LedMatrixDisplay led_matrix_display, uint8_t quadrant_row, uint8_t rgb_color_bit);
bool display_frame(LedMatrixDisplay led_matrix_display, uint8_t rgb_color_bit);
void clear_screen(LedMatrixDisplay led_matrix_display, bool load_back_frame);
void fill_screen(LedMatrixDisplay led_matrix_display, pixel_color pixel_value, bool load_back_frame);
bool gfx_switch_frame(void);
void gfx_choose_frame(bool front_frame_select);