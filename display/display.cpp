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

void display_priority_default() {
    // all four "text" layers
    reg::dispcnt::write({
        .mode = 0,
        .layer_background_0 = true,
        .layer_background_1 = true,
        .layer_background_2 = true,
        .layer_background_3 = true,
    });

    reg::bg0cnt::write(background_control {
        .character_base_block = 0,
        .screen_base_block = 1,
    });

    reg::bg1cnt::write(background_control {
        .character_base_block = 0,
        .screen_base_block = 2,
    });

    reg::bg2cnt::write(background_control {
        .character_base_block = 0,
        .screen_base_block = 3,
    });

    reg::bg3cnt::write(background_control {
        .character_base_block = 0,
        .screen_base_block = 4,
    });

    // palette
    palRAM[0] = 0x4210;
    palRAM[1] = 0x03F0;
    palRAM[2] = 0x001F;
    palRAM[3] = 0x7C10;
    palRAM[4] = 0x7FE0;

    // fill each layer with a single tile
    agbabi::wordset4(videoRAM + 0x0400, 0x800, 0x00010001);
    agbabi::wordset4(videoRAM + 0x0800, 0x800, 0x00020002);
    agbabi::wordset4(videoRAM + 0x0C00, 0x800, 0x00030003);
    agbabi::wordset4(videoRAM + 0x1000, 0x800, 0x00040004);

    // generate some tiles
    int i = 0;

    for(int y = 0; y < 8; y++) {
        videoRAM[i++] = 0x0000; videoRAM[i++] = 0x0000;
    }

    for(int y = 0; y < 8; y++) {
        videoRAM[i++] = 0x0010; videoRAM[i++] = 0x0000;
    }

    for(int y = 0; y < 8; y++) {
        videoRAM[i++] = 0x0022; videoRAM[i++] = 0x0000;
    }

    for(int y = 0; y < 8; y++) {
        videoRAM[i++] = 0x3033; videoRAM[i++] = 0x0000;
    }

    for(int y = 0; y < 8; y++) {
        videoRAM[i++] = 0x4444; videoRAM[i++] = 0x0000;
    }

    wait_for_exit();
}