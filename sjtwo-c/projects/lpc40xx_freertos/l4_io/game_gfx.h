#pragma once
#include <game_engine.h>
#include <led_gfx.h>
#include <ledmatrix.h>
#include <stdbool.h>
#include <stdint.h>

bubbleshooter_input_direction place_shooter(LedMatrixDisplay led_display, gfx__cursor launcher_position,
                                            bubbleshooter_input_direction position_change_intent,
                                            bool choose_back_frame);
void place_launcher(LedMatrixDisplay led_display, pixel_color rgb, gfx__cursor start_cursor, bool choose_back_frame);
gfx__cursor place_ball_queue(LedMatrixDisplay led_display, pixel_color rgb, gfx__cursor start_cursor,
                             bool choose_back_frame);
void gfx_load_ball_queue(LedMatrixDisplay led_display, pixel_color *ball_queue, gfx__cursor queue_position,
                         bool choose_back_frame);
void place_ball(LedMatrixDisplay led_display, pixel_color rgb, gfx__cursor start_cursor, bool is_player,
                bool choose_back_frame);
gfx__cursor draw_fail_line(LedMatrixDisplay led_display, game_level_state game_state, bool choose_back_frame);
void draw_bouncy_walls(LedMatrixDisplay led_display, pixel_color bouncy_wall_color, bool choose_back_frame);
gfx__cursor draw_falling_wall(LedMatrixDisplay led_display, bool choose_back_frame);

void remove_ball(LedMatrixDisplay led_display, gfx__cursor start_cursor, bool choose_back_frame);
void draw_level_stats(LedMatrixDisplay led_display, game_level_state current_game_state, bool new_data,
                      bool choose_back_frame);
game_level_state game_level_base_player(LedMatrixDisplay led_display, game_level_state current_game_state,
                                        bool choose_back_frame);
void game_level_hazard_wall(LedMatrixDisplay led_display, bool choose_back_frame);
game_level_state game_level_base(LedMatrixDisplay led_display, pixel_color border_color,
                                 game_level_state current_game_state, bool choose_back_frame);
void game_show_high_score_details(LedMatrixDisplay led_display, gfx__cursor location, pixel_color text_color,
                                  uint16_t highscore, bool choose_back_frame);

// -----------------------> Levels & Menus <--------------------------------
game_level_state game_splash_screen(LedMatrixDisplay led_display, game_level_state current_game_state,
                                    bool choose_back_frame);
void game_gameover(LedMatrixDisplay led_display, uint16_t highscore, bool choose_back_frame);
void game_pause_menu(LedMatrixDisplay led_display, pause_menu_actions pause_action, bool choose_back_frame);
void game_prepare_for_next_level(LedMatrixDisplay led_display, uint8_t level_number, bool choose_back_frame);
game_level_state game_level1(LedMatrixDisplay led_display, game_level_state current_game_state, bool choose_back_frame);
game_level_state game_level2(LedMatrixDisplay led_display, game_level_state current_game_state, bool choose_back_frame);
game_level_state game_level3(LedMatrixDisplay led_display, game_level_state current_game_state, bool choose_back_frame);
void game_won(LedMatrixDisplay led_display, uint16_t highscore, bool choose_back_frame);
void easter_egg(LedMatrixDisplay led_display, bool choose_back_frame);