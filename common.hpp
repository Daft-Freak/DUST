#pragma once

#include <gba/io/io.hpp>

extern gba::io::keypad_manager keypad_man;

inline auto palRAM = reinterpret_cast<uint16_t *>(0x05000000);
inline auto videoRAM = reinterpret_cast<uint16_t *>(0x06000000);

void write_text(int x, int y, const char *text);

void wait_for_exit();

// test declarations
void display_layer_none();