#include "FreeRTOS.h"
#include "task.h"
#include <delay.h>
#include <gpio.h>
#include <ledmatrix.h>
#include <stdint.h>
#include <stdio.h>

#define QUADRANT_ROW 16
#define LENGTH 32
#define WIDTH 64

volatile bool frame_executing = false;             // If true, frame is in the middle of executing.
volatile bool select_front_frame = true;           // If true, choose RGBFrameFront.
static pixel_color RGBFrameBack[LENGTH][WIDTH];    // Holds the data for the RGB Matrix.
static pixel_color RGBFrameFront[LENGTH][WIDTH];   // Holds the data for the RGB Matrix.
volatile led_matrix_pins LedMatrixPins;            // Will be used to configure the pins.
const uint8_t row_adjust[] = {0, 16};              // Specifies the row starting point for each quadrant.
const uint8_t maximum_quadrant_row = QUADRANT_ROW; // The display consists of multiple 16x32 displays with 16 rows each.

/*********************************************************************
 * Initializes GPIO connections for the display.
 *
 * PIN:    DISPLAY:         QUADRANT:                 SUPPORTED:
 * P1.0  - R0 (Left)      - Top Quadrant             - All
 * P1.1  - G0 (Left)      - Top Quadrant             - All
 * P1.4  - B0 (Left)      - Top Quadrant             - All
 * P4.28 - R1 (Left)      - Bottom Quadrant          - 32x32, 32z64
 * P0.6  - G1 (Left)      - Bottom Quadrant          - 32x32, 32z64
 * P0.8  - B1 (Left)      - Bottom Quadrant          - 32x32, 32z64
 * P0.26 - (Data) CLK     - Shared w/ Left & Right   - All
 * P1.31 - (Data) Latch   - Shared w/ Left & Right   - All
 * P1.20 - Output Enable  - Shared w/ Left & Right   - All
 * P1.14 - A              - Shared w/ Left & Right   - All
 * P4.29 - B              - Shared w/ Left & Right   - All
 * P0.7  - C              - Shared w/ Left & Right   - All
 * P0.9  - D              - Shared w/ Left & Right   - 32x32, 32x64
 *********************************************************************/
static led_matrix_pins initialize_display_connections(led_matrix_type display_type) {
  // Configures all the pins on the SJ2 Board mataching above comment to output.
  led_matrix_pins configured_display_pins;

  /********************************************************
   * Below GPIO pins are COMMON for all supported displays.
   ********************************************************/
  // Top-Half R,G,B pins
  gpio__construct_with_function(GPIO__FUNCTION_1, 0, GPIO__FUNCITON_0_IO_PIN);
  configured_display_pins.R0 = gpio__construct_as_output(GPIO__FUNCTION_1, 0); // P1.0
  gpio__construct_with_function(GPIO__FUNCTION_1, 1, GPIO__FUNCITON_0_IO_PIN);
  configured_display_pins.G0 = gpio__construct_as_output(GPIO__FUNCTION_1, 1); // P1.1
  gpio__construct_with_function(GPIO__FUNCTION_1, 4, GPIO__FUNCITON_0_IO_PIN);
  configured_display_pins.B0 = gpio__construct_as_output(GPIO__FUNCTION_1, 4); // P1.4

  // CLK, STB/Latch and OE pins
  gpio__construct_with_function(GPIO__FUNCITON_0_IO_PIN, 26, GPIO__FUNCITON_0_IO_PIN);
  gpio__construct_with_function(GPIO__FUNCTION_1, 31, GPIO__FUNCITON_0_IO_PIN);
  gpio__construct_with_function(GPIO__FUNCTION_1, 20, GPIO__FUNCITON_0_IO_PIN);
  configured_display_pins.CLK = gpio__construct_as_output(GPIO__FUNCITON_0_IO_PIN, 26); // P0.26
  configured_display_pins.STB_LATCH = gpio__construct_as_output(GPIO__FUNCTION_1, 31);  // P1.31
  configured_display_pins.OE = gpio__construct_as_output(GPIO__FUNCTION_1, 20);         // P1.20

  // A,B,C pins
  gpio__construct_with_function(GPIO__FUNCTION_1, 14, GPIO__FUNCITON_0_IO_PIN);
  gpio__construct_with_function(GPIO__FUNCTION_4, 29, GPIO__FUNCITON_0_IO_PIN);
  gpio__construct_with_function(GPIO__FUNCITON_0_IO_PIN, 7, GPIO__FUNCITON_0_IO_PIN);
  configured_display_pins.A = gpio__construct_as_output(GPIO__FUNCTION_1, 14);       // P1.14
  configured_display_pins.B = gpio__construct_as_output(GPIO__FUNCTION_4, 29);       // P4.29
  configured_display_pins.C = gpio__construct_as_output(GPIO__FUNCITON_0_IO_PIN, 7); // P0.7

  /********************************************************
   * Below GPIO pins are only for 32x32 and 32x64 displays._pins.STB_LATCH = gpio__construct_as_output(GPIO__FUNCTION_1,
   *31);  // P1.31
   ********************************************************/
  if (display_type != LED_MATRIX_16x32) {
    // D pin
    gpio__construct_with_function(GPIO__FUNCITON_0_IO_PIN, 9, GPIO__FUNCITON_0_IO_PIN);
    configured_display_pins.D = gpio__construct_as_output(GPIO__FUNCITON_0_IO_PIN, 9); // P0.9

    // Bottom-Half R,G,B pins
    gpio__construct_with_function(GPIO__FUNCTION_4, 28, GPIO__FUNCITON_0_IO_PIN);
    gpio__construct_with_function(GPIO__FUNCITON_0_IO_PIN, 6, GPIO__FUNCITON_0_IO_PIN);
    gpio__construct_with_function(GPIO__FUNCITON_0_IO_PIN, 8, GPIO__FUNCITON_0_IO_PIN);
    configured_display_pins.R1 = gpio__construct_as_output(GPIO__FUNCTION_4, 28);       // P4.28
    configured_display_pins.G1 = gpio__construct_as_output(GPIO__FUNCITON_0_IO_PIN, 6); // P0.6
    configured_display_pins.B1 = gpio__construct_as_output(GPIO__FUNCITON_0_IO_PIN, 8); // P0.8
  }

  return configured_display_pins;
}

/*************************************************************
 * Initialize the LED matrix by creating a 2D Matrix
 * Containing pixel data for the matrix and info about the
 * display.
 *
 * Since we don't want to deal with malloc and free,
 * we will define the maximum frame matrix under CurrentFrame.
 **************************************************************/
LedMatrixDisplay initialize_display(led_matrix_type display_type) {
  LedMatrixDisplay led_matrix;
  led_matrix.type = display_type;

  if (display_type == LED_MATRIX_16x32) {
    led_matrix.rows = 16;
    led_matrix.columns = 32;
    led_matrix.quadrants = 1;

  } else if (display_type == LED_MATRIX_32x32) {
    led_matrix.rows = 32;
    led_matrix.columns = 32;
    led_matrix.quadrants = 2;

  } else {
    led_matrix.rows = 32;
    led_matrix.columns = 64;
    led_matrix.quadrants = 2;
  }

  LedMatrixPins = initialize_display_connections(led_matrix.type);

  // Initialize all the corresponding pins for programming the LED Matrix.
  CLK_low();
  OE_high();
  STB_LATCH_low();

  // TODO: Not sure if pointers will really work when defined inside function. This function may not even work.
  return led_matrix;
}

/************************************************************
 * Given the coordinate on the display, modify the pixel.
 ************************************************************/
pixel_color modify_pixel(LedMatrixDisplay led_matrix_display, pixel_color new_pixel_value, int x, int y,
                         bool load_back_frame) {
  // Modify's the pixel in the current Frame's pixel stack.
  if (x < led_matrix_display.rows && y < led_matrix_display.columns) {
    pixel_color modified_pixel;
    if (!load_back_frame) {
      modified_pixel = RGBFrameFront[x][y];
      RGBFrameFront[x][y] = new_pixel_value;
    } else {
      modified_pixel = RGBFrameBack[x][y];
      RGBFrameBack[x][y] = new_pixel_value;
    }
    return modified_pixel;

  } else {
    // If an invalid pair of indexes are given, return an empty pixel.
    pixel_color empty_pixel = {0, 0};
    return empty_pixel;
  }
}

/**************************************
 * Resets the pixel to black.
 **************************************/
pixel_color reset_pixel(LedMatrixDisplay led_matrix_display, int x, int y, bool load_back_frame) {
  pixel_color empty_pixel = {0, 0, 0};
  empty_pixel.event = BACKGROUND_OBJECT;
  return modify_pixel(led_matrix_display, empty_pixel, x, y, load_back_frame);
}

/********************************************************
 * Moves the pixel from (old_x, old_y) to (new_x, new_y).
 *
 * Usually used when you want to make a GFX object
 * be in motion.
 *
 * NOTE:
 * Moving essentially clears the previous pixel.
 ********************************************************/
pixel_color move_pixel(LedMatrixDisplay led_matrix_display, pixel_color new_pixel_value, int old_x, int old_y,
                       int new_x, int new_y, bool load_back_frame) {
  // Modify the pixel given by (old_x, old_y).
  pixel_color modified_pixel = modify_pixel(led_matrix_display, new_pixel_value, new_x, new_y, load_back_frame);

  // Clear the previous pixel.
  reset_pixel(led_matrix_display, old_x, old_y, load_back_frame);

  return modified_pixel;
}

/**************************************************************************
 * Returns the pixel and its properties at the specified (x,y) coordinate.
 **************************************************************************/
pixel_color get_pixel(uint8_t x, uint8_t y, bool is_back_frame) {
  volatile pixel_color choosen_pixel;
  if (!is_back_frame) {
    choosen_pixel = RGBFrameFront[x][y];
    // printf("RGBFrameFront[%d][%d] = %d,%d,%d\n", x, y, choosen_pixel.Red, choosen_pixel.Blue,
    //       choosen_pixel.Green);
  } else {
    choosen_pixel = RGBFrameBack[x][y];
    // printf("RGBFrameBack[%d][%d] = %d,%d,%d\n", x, y, choosen_pixel.Red, choosen_pixel.Blue,
    //       choosen_pixel.Green);
  }
  return choosen_pixel;
}

/****************************
 * Sets the Data Clock Low.
 ***************************/
static void CLK_low(void) { gpio__reset(LedMatrixPins.CLK); }

/***************************************************************
 * Sets the Data Clock High.
 *
 * On rising edge, the data on all the RGB pins are programmed
 * serially into the display controller MSB first to program
 * the LED sequence on the quadrant row.
 ***************************************************************/
static void CLK_high(void) { gpio__set(LedMatrixPins.CLK); }

/**************************************************************************
 * Sets Latch/STB Low, (Used for initialization of LED programmed states)
 **************************************************************************/
static void STB_LATCH_low(void) { gpio__reset(LedMatrixPins.STB_LATCH); }

/*********************************************************************************
 * Sets Latch/STB High, (Starts all the LED programmed states for the ABCD row #)
 *********************************************************************************/
static void STB_LATCH_high(void) { gpio__set(LedMatrixPins.STB_LATCH); }

/*********************************************************************
 * If OE pin is LOW, Active Low. Setting Low enables the display.
 *********************************************************************/
static void OE_low(void) { gpio__reset(LedMatrixPins.OE); }

/*********************************************************************
 * If OE pin is HIGH, so this should disable the display.
 *********************************************************************/
static void OE_high(void) { gpio__set(LedMatrixPins.OE); }

/******************************************************************
 * Sets IO Pin to HIGH or LOW depending on bit value from 'value'.
 *****************************************************************/
static void extract_value_with_bitmask(gpio_s io_pin, uint8_t value, uint8_t mask) {
  if (value & mask) {
    gpio__set(io_pin);
  } else {
    gpio__reset(io_pin);
  }
}

/********************************************************************************
 * Loads the A,B,C and D bits corresonding to LED Matrix Row # for all Quadrants.
 * ABCD is actually DBCA in terms of representing the Row #.
 *
 * What I mean is that D is the MSB, while A is the LSB.
 * Therefore a Row # of 0001 is: D=0,C=0,B=0,A=1.
 ********************************************************************************/
static bool select_row(LedMatrixDisplay led_matrix_display, uint8_t row_number) {
  if ((led_matrix_display.type != LED_MATRIX_16x32) && (row_number < 16)) {
    uint8_t mask_D = 1 << 3;
    uint8_t mask_C = 1 << 2;
    uint8_t mask_B = 1 << 1;
    uint8_t mask_A = 1 << 0;

    extract_value_with_bitmask(LedMatrixPins.D, row_number, mask_D); // MSB
    extract_value_with_bitmask(LedMatrixPins.C, row_number, mask_C);
    extract_value_with_bitmask(LedMatrixPins.B, row_number, mask_B);
    extract_value_with_bitmask(LedMatrixPins.A, row_number, mask_A); // LSB

    return true;

  } else if ((led_matrix_display.type == LED_MATRIX_16x32) && (row_number < 8)) {
    uint8_t mask_C = 1 << 2;
    uint8_t mask_B = 1 << 1;
    uint8_t mask_A = 1 << 0;

    extract_value_with_bitmask(LedMatrixPins.C, row_number, mask_C); // MSB
    extract_value_with_bitmask(LedMatrixPins.B, row_number, mask_B);
    extract_value_with_bitmask(LedMatrixPins.A, row_number, mask_A); // LSB

    return true;
  } else {
    return false;
  }
}

/*********************************************************************************
 * On each CLK cycle, load the R0,G0,B0 and R1,G1,B1 pins with values stored in
 * the RGBFrame array defined at the top of this library. CLK low and high
 * are set in parent function:
 *********************************************************************************/
static bool RGB_per_clock(LedMatrixDisplay led_matrix_display, uint8_t quadrant_row, uint8_t column,
                          uint8_t rgb_color_bit) {
  // red_pins, green_pins and blue_pins are listed by the
  gpio_s red_pins[] = {LedMatrixPins.R0, LedMatrixPins.R1};
  gpio_s green_pins[] = {LedMatrixPins.G0, LedMatrixPins.G1};
  gpio_s blue_pins[] = {LedMatrixPins.B0, LedMatrixPins.B1};
  uint8_t rgb_mask = 1 << rgb_color_bit;
  uint8_t row;

  // If the row specified is larger than defined by display type, or column is larger than defined by display, do not
  // continue.
  if (quadrant_row < led_matrix_display.rows && column < led_matrix_display.columns) { // Error handling.
    for (int quadrant = 0; quadrant < led_matrix_display.quadrants; ++quadrant) {
      // For the quadrants (top and bottom), load the corresponding pins R0,G0,B0,R1,G1,B1 for the clock cycle.
      // The row # below is the row for the given quadrant. Top starts at 0, while Bottom starts at 16.
      row = quadrant_row + row_adjust[quadrant];

      // For 32x32 and 32x64 displays, the screen is made up of multiple 16x32 displays.
      if (select_front_frame) {
        extract_value_with_bitmask(red_pins[quadrant], RGBFrameFront[row][column].Red, rgb_mask);
        extract_value_with_bitmask(green_pins[quadrant], RGBFrameFront[row][column].Green, rgb_mask);
        extract_value_with_bitmask(blue_pins[quadrant], RGBFrameFront[row][column].Blue, rgb_mask);

      } else {
        extract_value_with_bitmask(red_pins[quadrant], RGBFrameBack[row][column].Red, rgb_mask);
        extract_value_with_bitmask(green_pins[quadrant], RGBFrameBack[row][column].Green, rgb_mask);
        extract_value_with_bitmask(blue_pins[quadrant], RGBFrameBack[row][column].Blue, rgb_mask);
      }
    }
    return true;

  } else {
    return false;
  }
}

/**********************************************************************************
 * For 16x32 displays, only one row at a time is displayed in one latch cycle.
 * For 32x32 and 32x64 displays, 2 rows can be displayed at a time in one latch
 * cycle due to the fact that those displays are made up of multiple 16x32
 * displays connected together.
 *
 * You should see two rows show up at one time on those larger displays.
 *
 * Each clock requires about 40ns of delay. The SJ2 board runs at about 10ns.
 *********************************************************************************/
static bool display_selected_row(LedMatrixDisplay led_matrix_display, uint8_t quadrant_row, uint8_t rgb_color_bit) {
  bool success = false;
  // Initialize the pins used for driving the RGB display logic.
  CLK_low();
  OE_low();
  STB_LATCH_low();

  // Clocks all RGB data by the number of LEDs on each quadrant row (32), for programming the LED states on those rows.
  for (uint8_t column = 0; column < led_matrix_display.columns; ++column) {
    CLK_low();
    success = RGB_per_clock(led_matrix_display, quadrant_row, column, rgb_color_bit);
    if (!success) {
      return success;
    }
    CLK_high();
    delay__us(0.4); // According to the datasheet, the maximum CLK rise + fall time is about 500-1000ns
  }

  // If the RGBs were clocked correctly, set CLK to low, OE to low (enable) and STB_LATCH (enable) to turn on the
  // display.
  CLK_low();
  OE_high();
  STB_LATCH_high();

  // Load ABCD pins corresponding to the Row #.
  select_row(led_matrix_display, quadrant_row);

  // delay__us(1); // Holding the latch longer may increase the brightness of the screen.

  return success;
}

/******************************************************************************************
 * A frame is essentially all the programmed LED states displaying on the LED matrix
 * with the naked eye without flickering.
 ******************************************************************************************/
bool display_frame(LedMatrixDisplay led_matrix_display, uint8_t rgb_color_bit) {
  frame_executing = true;
  bool success = false;
  for (int quadrant_row = 0; quadrant_row < maximum_quadrant_row; ++quadrant_row) {
    success = display_selected_row(led_matrix_display, quadrant_row, rgb_color_bit);
    if (!success) {
      break;
    }
  }
  frame_executing = false;
  return success;
}

/*******************************************
 * Simply turns off all LEDs on the Display
 *******************************************/
void clear_screen(LedMatrixDisplay led_matrix_display, bool load_back_frame) {
  uint8_t rgb_color_bit = 0; // 00000001--> 1 (Bit 0)
  pixel_color empty_pixel = {0, 0, 0};

  // First modifies all the Current Frame's pixel values to 0, to turn off all LEDs.
  for (int row = 0; row < led_matrix_display.rows; ++row) {
    for (int column = 0; column < led_matrix_display.columns; ++column) {
      modify_pixel(led_matrix_display, empty_pixel, row, column, load_back_frame);
    }
  }
}

/***********************************
 * Fills the screen with one color.
 ***********************************/
void fill_screen(LedMatrixDisplay led_matrix_display, pixel_color pixel_value, bool load_back_frame) {
  uint8_t rgb_color_bit = 0; // 00000001--> 1 (Bit 0)

  // First modifies all the Current Frame's pixel values to the desired pixel color.
  for (int row = 0; row < led_matrix_display.rows; ++row) {
    for (int column = 0; column < led_matrix_display.columns; ++column) {
      modify_pixel(led_matrix_display, pixel_value, row, column, load_back_frame);
    }
  }
}

/*******************************************************************************
 * Calling this function switches back and forth between front and back frame.
 *
 * NOTE:
 * If Front frame, this switches to Back frame.
 * Likewise, if Back frame, this switches to Front frame.
 ******************************************************************************/
bool gfx_switch_frame(void) {
  if (select_front_frame) {
    select_front_frame = false;
  } else {
    select_front_frame = true;
  }

  return select_front_frame;
}

/*******************************************************************************
 * Similar to the top function, this allows the developer to swap to the
 * desired frame. This might be necessary on special scenarios.
 ******************************************************************************/
void gfx_choose_frame(bool front_frame_select) {
  if (front_frame_select) {
    select_front_frame = true;
  } else {
    select_front_frame = false;
  }
}