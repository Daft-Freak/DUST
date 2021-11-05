#include <gba/gba.hpp>
#include <gba/ext/agbabi.hpp>

#include "common.hpp"

void clear_text() {
    // clear char block 2-3
    gba::agbabi::wordset4(video_ram + 0x800, 0x800 * 2, 0);
}

void write_text(int x, int y, const char *text) {
    // char block 2
    auto ptr = video_ram + 0x800 + x + y * 32;

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