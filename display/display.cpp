#include <gba/gba.hpp>

#include "../common.hpp"

using namespace gba;

// no layers enabled
void display_layer_none() {

    // nothing enabled
    reg::dispcnt::write({});

    // should get backdrop
    palRAM[0] = 0x7C10;

    // put some junk in the other colours
    for(int i = 1; i < 256; i++)
        palRAM[i] = (i * 0x1F2E3D4C) >> 16;

    // and fill vram with more junk
    for(int i = 0; i < 0x8000; i++)
        videoRAM[i] = i;

    wait_for_exit();
}

void display_cgb_mode() {
    clear_text();
    palRAM[1] = 0x1F;

    // shouldn't be set for AGB mode
    if(reg::dispcnt::read().color_game_boy) {
        write_text(0, 0, "CGB mode is set!");
        return wait_for_exit();
    }

    // shouldn't be writable outside BIOS
    auto dispcnt = reg::dispcnt::read();
    dispcnt.color_game_boy = true;
    reg::dispcnt::write(dispcnt);

    if(reg::dispcnt::read().color_game_boy) {
        write_text(0, 1, "CGB mode is writable!");
        return wait_for_exit();
    }

    palRAM[0] = 0x7C10;

    wait_for_exit();
}

void display_forced_blank() {
    palRAM[0] = 0;

    write_text(0, 10, "     That didn't work...     ");

    // should blank screen (to white)
    auto dispcnt = reg::dispcnt::read();
    dispcnt.force_blank = true;
    reg::dispcnt::write({dispcnt});

    wait_for_exit();
}