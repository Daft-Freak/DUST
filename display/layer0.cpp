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
    agbabi::memcpy2(video_ram + 0x2000, video_ram, 0x4000);
    // erase font data in block 0
    agbabi::wordset4(video_ram, 0x4000, 0);

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
    palette_ram[17] = 0x03F0;
    palette_ram[18] = 0x001F;
    palette_ram[19] = 0x7C10;
    palette_ram[20] = 0x7FE0;

    // palette 3
    palette_ram[49] = 0x0308;
    palette_ram[50] = 0x0010;
    palette_ram[51] = 0x4008;
    palette_ram[52] = 0x4200;

    // file some screen/char data
    for(int i = 0; i < 0x4000;) {
        // this is two tiles but also eight pixels as both bases are set to 0
        video_ram[i++] = 0x1122;
        video_ram[i++] = 0x3344;
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
    palette_ram[17] = 0x0010;

    palette_ram[34] = 0x0308;

    palette_ram[51] = 0x4200;

    palette_ram[67] = 0x4008;
    palette_ram[68] = 0x4008;

    // file some screen/char data
    for(int i = 0; i < 0x8000;) {
        // this is four tiles but also eight pixels as both bases are set to 0
        video_ram[i++] = 0x1111;
        video_ram[i++] = 0x2222;
        video_ram[i++] = 0x3333;
        video_ram[i++] = 0x4344; // avoid setting H flip
    }

    wait_for_exit();
}

static void size_test(screen_size size, uint16_t mode = 0) {
    reg::dispcnt::write({
        .mode = mode,
        .layer_background_0 = true,
    });

    reg::bg0cnt::write(background_control {
        .screen_base_block = 2,
        .screen_size = size
    });

    palette_ram[1] = 0x03F0;
    palette_ram[2] = 0x001F;
    palette_ram[3] = 0x7C10;
    palette_ram[4] = 0x7FE0;
    palette_ram[5] = 0x0010;
    palette_ram[6] = 0x0308;
    palette_ram[7] = 0x4200;
    palette_ram[8] = 0x4008;

    // fill each block with a digit
    agbabi::wordset4(video_ram + 0x0800, 0x800, 0x00100010);
    agbabi::wordset4(video_ram + 0x0C00, 0x800, 0x00110011);
    agbabi::wordset4(video_ram + 0x1000, 0x800, 0x00120012);
    agbabi::wordset4(video_ram + 0x1400, 0x800, 0x00130013);

    // adjust font colours
    // 0/1 -> 1/2, 3/4, 5/6, 7/8
    for(int i = 0; i < 4; i++) {
        for(int y = 0; y < 8; y++) {
            video_ram[(i + 16) * 16 + y * 2 + 0] += 0x11111111 * (i * 2 + 1);
            video_ram[(i + 16) * 16 + y * 2 + 1] += 0x11111111 * (i * 2 + 1);
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

void display_layer0_flip() {
    reg::dispcnt::write({
        .mode = 0,
        .layer_background_0 = true,
    });

    reg::bg0cnt::write(background_control {
        .screen_base_block = 2,
    });

    // make a pattern from the '/' character
    for(int i = 0; i < 32; i+= 2) {
        agbabi::wordset4(video_ram + 0x0800 + (i + 0) * 32, 32 * 2, 0x040F000F);
        agbabi::wordset4(video_ram + 0x0800 + (i + 1) * 32, 32 * 2, 0x0C0F080F);
    }

    reg::bg0hofs::write(0);
    reg::bg0vofs::write(0);

    wait_for_exit();
}

// same as mode 0
void display_layer0_mode1() {
    size_test(screen_size::regular_64x64, 1);
}

// not used in these modes
void display_layer0_mode2() {
    palette_ram[0] = 0x7FFF;
    size_test(screen_size::regular_64x64, 2);
}

void display_layer0_mode3() {
    palette_ram[0] = 0x7FFF;
    size_test(screen_size::regular_64x64, 3);
}

void display_layer0_mode4() {
    palette_ram[0] = 0x7FFF;
    size_test(screen_size::regular_64x64, 4);
}

void display_layer0_mode5() {
    palette_ram[0] = 0x7FFF;
    size_test(screen_size::regular_64x64, 5);
}

// now for the tests that do Bad Things(TM)

// GBA hardware has some "interesting" behaviour for these, DS just reads 0s
void display_layer0_char_base_invalid() {
    // sets char base to 0xC000, causing some tiles to be in the sprite area
    reg::dispcnt::write({
        .mode = 0,
        .layer_background_0 = true,
    });

    reg::bg0cnt::write(background_control {
        .character_base_block = 3,
        .screen_base_block = 0,
    });

    // fill entire palette
    for(int i = 0; i < 512; i++)
        palette_ram[i] = (i & 0x7) << 2 | (i & 0x38) << 4 | (i & 0x1C0) << 6;

    agbabi::wordset4(video_ram + 0xC000 / 2, 0x8000, 0x11223344);

    //? fill in most of the first 16k
    agbabi::wordset4(video_ram + 0x400, 0x4000 - 0x800, 0x44332211);

    for(int i = 0; i < 32 * 32; i++)
        video_ram[i] = i;

    reg::bg0hofs::write(0);
    reg::bg0vofs::write(256 - 160); // scroll to the bottom

    wait_for_exit();
}

void display_layer0_screen_base_invalid() {
    // two of the screen blocks are in the sprite area
    reg::dispcnt::write({
        .mode = 0,
        .layer_background_0 = true,
    });

    reg::bg0cnt::write(background_control {
        .screen_base_block = 30,
        .screen_size = screen_size::regular_64x64
    });

    // fill entire palette
    for(int i = 0; i < 512; i++)
        palette_ram[i] = (i & 0x7) << 2 | (i & 0x38) << 4 | (i & 0x1C0) << 6;

    // fill each block with a digit
    agbabi::wordset4(video_ram + 0x7800, 0x800, 0x00100010);
    agbabi::wordset4(video_ram + 0x7C00, 0x800, 0x00110011);
    agbabi::wordset4(video_ram + 0x8000, 0x800, 0x00120012);
    agbabi::wordset4(video_ram + 0x8400, 0x800, 0x00130013);

    //put some stuff near the start too
    //agbabi::wordset4(videoRAM + 0x0000, 0x800, 0x00140014);
    agbabi::wordset4(video_ram + 0x0400, 0x800, 0x00150015);

    // adjust font colours
    // 0/1 -> 1/2, 3/4, 5/6, 7/8
    for(int i = 0; i < 64; i++) {
        for(int y = 0; y < 8; y++) {
            video_ram[i * 16 + y * 2 + 0] += 0x11111111 * ((i % 4) * 2 + 1);
            video_ram[i * 16 + y * 2 + 1] += 0x11111111 * ((i % 4) * 2 + 1);
        }
    }

    // scroll so that edge is in the middle of the screen
    reg::bg0hofs::emplace(int16_t(256 - 120));
    reg::bg0vofs::emplace(int16_t(256 - 80));

    wait_for_exit();
}
