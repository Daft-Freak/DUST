#include <gba/gba.hpp>

#include "../common.hpp"

using namespace gba;

// layer0, always "text" (mode 0-1)

void display_layer0_char_base() {
    reg::dispcnt::write({
        .mode = 0,
        .layer_background_0 = true,
    });
    reg::bg0cnt::write(background_control {
        .character_base_block = 1,
        .screen_base_block = 2,
    });

    clear_text();

    // copy font data
    agbabi::memcpy2(videoRAM + 0x2000, videoRAM, 0x4000);
    // erase font data in block 0
    agbabi::wordset4(videoRAM, 0x4000, 0);

    write_text(3, 10, "Hello from char block 1!");

    wait_for_exit();
}

void display_layer0_4bpp() {
    reg::dispcnt::write({
        .mode = 0,
        .layer_background_0 = true,
    });

    // setup minimal 4bpp layer (everything zeroed)
    reg::bg0cnt::write({});

    // setup some colours

    // pallete 1
    palRAM[17] = 0x03F0;
    palRAM[18] = 0x001F;
    palRAM[19] = 0x7C10;
    palRAM[20] = 0x7FE0;

    // palette 3
    palRAM[49] = 0x0308;
    palRAM[50] = 0x0010;
    palRAM[51] = 0x4008;
    palRAM[52] = 0x4200;

    // file some screen/char data
    for(int i = 0; i < 0x4000;) {
        // this is two tiles but also eight pixels as both bases are set to 0
        videoRAM[i++] = 0x1122;
        videoRAM[i++] = 0x3344;
    }

    wait_for_exit();
}

void display_layer0_8bpp() {
    reg::dispcnt::write({
        .mode = 0,
        .layer_background_0 = true,
    });

    // setup minimal 8bpp layer (everything else zeroed)
    reg::bg0cnt::write({
        .color_depth = color_depth::bpp_8
    });

    // setup some colours
    palRAM[17] = 0x0010;

    palRAM[34] = 0x0308;

    palRAM[51] = 0x4200;

    palRAM[67] = 0x4008;
    palRAM[68] = 0x4008;

    // file some screen/char data
    for(int i = 0; i < 0x8000;) {
        // this is four tiles but also eight pixels as both bases are set to 0
        videoRAM[i++] = 0x1111;
        videoRAM[i++] = 0x2222;
        videoRAM[i++] = 0x3333;
        videoRAM[i++] = 0x4344; // avoid setting H flip
    }

    wait_for_exit();
}

static void size_test(screen_size size) {
  reg::dispcnt::write({
        .mode = 0,
        .layer_background_0 = true,
    });

    reg::bg0cnt::write(background_control {
        .screen_base_block = 2,
        .screen_size = size
    });

    palRAM[1] = 0x03F0;
    palRAM[2] = 0x001F;
    palRAM[3] = 0x7C10;
    palRAM[4] = 0x7FE0;
    palRAM[5] = 0x0010;
    palRAM[6] = 0x0308;
    palRAM[7] = 0x4200;
    palRAM[8] = 0x4008;

    // fill each block with a digit
    agbabi::wordset4(videoRAM + 0x0800, 0x800, 0x00100010);
    agbabi::wordset4(videoRAM + 0x0C00, 0x800, 0x00110011);
    agbabi::wordset4(videoRAM + 0x1000, 0x800, 0x00120012);
    agbabi::wordset4(videoRAM + 0x1400, 0x800, 0x00130013);

    // adjust font colours
    // 0/1 -> 1/2, 3/4, 5/6, 7/8
    for(int i = 0; i < 4; i++) {
        for(int y = 0; y < 8; y++) {
            videoRAM[(i + 16) * 16 + y * 2 + 0] += 0x11111111 * (i * 2 + 1);
            videoRAM[(i + 16) * 16 + y * 2 + 1] += 0x11111111 * (i * 2 + 1);
        }
    }

    // scroll so that edge is in the middle of the screen
    reg::bg0hofs::emplace(int16_t(256 - 120));
    reg::bg0vofs::emplace(int16_t(256 - 80));

    wait_for_exit();
}

void display_layer0_size0() {
    size_test(screen_size::regular_32x32);
}

void display_layer0_size1() {
    size_test(screen_size::regular_64x32);
}

void display_layer0_size2() {
    size_test(screen_size::regular_32x64);
}

void display_layer0_size3() {
    size_test(screen_size::regular_64x64);
}