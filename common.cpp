#include "common.hpp"

void write_text(int x, int y, const char *text) {
    // char block 2
    auto ptr = videoRAM + 0x800 + x + y * 32;

    for(; *text; text++)
        *ptr++ = *text - ' ';
}