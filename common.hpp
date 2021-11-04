#pragma once

#include <gba/io/io.hpp>

extern gba::io::keypad_manager keypad_man;

inline auto palRAM = reinterpret_cast<uint16_t *>(0x05000000);
inline auto videoRAM = reinterpret_cast<uint16_t *>(0x06000000);

void clear_text();
void write_text(int x, int y, const char *text);

void wait_for_exit();

// test declarations
void display_layer_none();
void display_cgb_mode();
void display_forced_blank();
void display_priority_default();
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