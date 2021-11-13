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

void gen_affine_tiles(uint16_t *char_base, uint16_t *screen_base, int screen_size_tiles) {
    palette_ram[0] = 0x4210;
    palette_ram[1] = 0x001F;
    palette_ram[2] = 0x03F0;
    palette_ram[3] = 0x7FE0;
    palette_ram[4] = 0x7C10;

    palette_ram[5] = 0x0010;
    palette_ram[6] = 0x0308;
    palette_ram[7] = 0x4200;
    palette_ram[8] = 0x4008;

    for(int i = 0; i < 4; i++) {
        uint8_t a = i + 1;
        uint8_t b = (i + 1) * 2;

        uint8_t tile[]{
            b, a, a, a, a, a, a, b,
            a, b, a, a, a, a, b, a,
            a, a, b, a, a, b, a, a,
            a, a, a, b, b, a, a, a,
            a, a, a, b, b, a, a, a,
            a, a, b, a, a, b, a, a,
            a, b, a, a, a, a, b, a,
            b, a, a, a, a, a, a, b,
        };
        gba::agbabi::memcpy2(char_base + i * 32, tile, 64);
    }

    for(int y = 0; y < screen_size_tiles; y++) {
        for(int x = 0; x < screen_size_tiles; x += 2) {
            screen_base[(y * screen_size_tiles + x) / 2] = (x + y) % 4 | ((x + y + 1) % 4) << 8;
        }
    }
}