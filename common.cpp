#include <gba/gba.hpp>

#include "common.hpp"

void write_text(int x, int y, const char *text) {
    // char block 2
    auto ptr = videoRAM + 0x800 + x + y * 32;

    for(; *text; text++)
        *ptr++ = *text - ' ';
}

void wait_for_exit() {
    while(true) {
        keypad_man.poll();

        gba::bios::vblank_intr_wait();

        if(keypad_man.switched_up(gba::key::button_b))
            return;
    }
}