#pragma once

#include <gba/io/io.hpp>

extern gba::io::keypad_manager keypad_man;

inline auto palette_ram = reinterpret_cast<uint16_t *>(0x05000000);
inline auto video_ram = reinterpret_cast<uint16_t *>(0x06000000);

void clear_text();
void write_text(int x, int y, const char *text);

void wait_for_exit();

// test declarations
void display_layer_none();
void display_cgb_mode();
void display_forced_blank();
void display_priority_default();
void display_stat_flags();
void display_layer0_char_base();
void display_layer0_4bpp();
void display_layer0_8bpp();
void display_layer0_size0();
void display_layer0_size1();
void display_layer0_size2();
void display_layer0_size3();
void display_layer0_flip();
void display_layer0_mode1();
void display_layer0_mode2();
void display_layer0_mode3();
void display_layer0_mode4();
void display_layer0_mode5();
void display_layer0_char_base_invalid();
void display_layer0_screen_base_invalid();
void display_layer2_mode0();
void display_layer2_mode1();
void display_layer2_mode1_char_base();
void display_layer2_mode1_wrap();
void display_layer2_mode1_size1();
void display_layer2_mode1_size2();
void display_layer2_mode1_size3();
void display_layer2_mode1_rotscale();
void display_layer2_mode2();
void display_layer2_mode3();
void display_layer2_mode3_rotscale();
void display_layer2_mode4();
void display_layer2_mode4_pageflip();
void display_layer2_mode4_rotscale();
void display_layer2_mode5();
void display_layer2_mode5_pageflip();
void display_layer2_mode5_rotscale();
void display_obj_regular();
void display_obj_affine();
void display_obj_affine_double();
void display_obj_hidden();
void display_obj_regular_8bpp();
void display_obj_affine_8bpp();
void display_obj_regular_hflip();
void display_obj_regular_vflip();
void display_obj_regular_hvflip();
void display_obj_affine_rotscale();
void display_obj_affine_double_rotscale();
void display_obj_regular_4bpp_pal();
void display_obj_wrap_x();
void display_obj_wrap_y();
void display_obj_wrap_y_bug();
void display_obj_bmp_char_base();
void display_obj_line_limit_regular_size0();
void display_obj_line_limit_regular_size1();
void display_obj_line_limit_regular_size2();
void display_obj_line_limit_regular_size3();
void display_obj_line_limit_affine_size0();
void display_obj_line_limit_affine_size1();
void display_obj_line_limit_affine_size2();
void display_obj_line_limit_affine_size3();
void display_obj_line_limit_affine_double_size0();
void display_obj_line_limit_affine_double_size1();
void display_obj_line_limit_affine_double_size2();
void display_obj_line_limit_affine_double_size3();
void display_obj_line_limit_regular_offscreen();
void display_obj_line_limit_regular_hidden();
void display_obj_line_limit_regular_size0_hblank_access();
void display_obj_line_limit_regular_size2_hblank_access();