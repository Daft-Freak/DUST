#include <gba/gba.hpp>

#include "../common.hpp"

using namespace gba;

// layer2, "text" (mode 0), affine (mode 1-2), bitmap (mode 5-5)

void display_layer2_mode0() {
    // basic test, mode 1 is identical for all layers

    reg::dispcnt::write({
        .mode = 0,
        .layer_background_2 = true,
    });
    reg::bg2cnt::write(background_control {
        .screen_base_block = 2,
    });

    reg::bg2hofs::write(0);
    reg::bg2vofs::write(0);

    clear_text();
    write_text(5, 10, "Hello from layer 2!");

    wait_for_exit();
}

static void gen_affine_tiles(uint16_t *char_base, uint16_t *screen_base, int screen_size_tiles) {
    palRAM[0] = 0x4210;
    palRAM[1] = 0x001F;
    palRAM[2] = 0x03F0;
    palRAM[3] = 0x7FE0;
    palRAM[4] = 0x7C10;

    palRAM[5] = 0x0010;
    palRAM[6] = 0x0308;
    palRAM[7] = 0x4200;
    palRAM[8] = 0x4008;

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
        agbabi::memcpy2(char_base + i * 32, tile, 64);
    }

    for(int y = 0; y < screen_size_tiles; y++) {
        for(int x = 0; x < screen_size_tiles; x += 2) {
            screen_base[(y * screen_size_tiles + x) / 2] = (x + y) % 4 | ((x + y + 1) % 4) << 8;
        }
    }
}

static void reset_affine() {
    reg::bg2pa::write(1);
    reg::bg2pb::write(0);
    reg::bg2pc::write(0);
    reg::bg2pd::write(1);
    reg::bg2x::write(0);
    reg::bg2y::write(0);
}

void display_layer2_mode1() {
    // affine
    reg::dispcnt::write({
        .mode = 1,
        .layer_background_2 = true,
    });
    reg::bg2cnt::write(background_control {
        .screen_base_block = 2,
    });

    // these have no effect
    reg::bg2hofs::write(100);
    reg::bg2vofs::write(100);

    reset_affine();

    // generate some 8bpp tiles
    gen_affine_tiles(videoRAM, videoRAM + 0x800, 16);

    wait_for_exit();
}

void display_layer2_mode1_char_base() {
    // same thing, different char base
    reg::dispcnt::write({
        .mode = 1,
        .layer_background_2 = true,
    });
    reg::bg2cnt::write(background_control {
        .character_base_block = 1,
        .screen_base_block = 2,
    });

    reset_affine();

    // generate some 8bpp tiles
    gen_affine_tiles(videoRAM + 0x2000, videoRAM + 0x800, 16);

    wait_for_exit();
}

void display_layer2_mode1_wrap() {
    reg::dispcnt::write({
        .mode = 1,
        .layer_background_2 = true,
    });
    reg::bg2cnt::write(background_control {
        .screen_base_block = 2,
        .affine_background_wrap = affine_background_wrap::wrap
    });

    // center it
    reset_affine();
    reg::bg2x::write(-56);
    reg::bg2y::write(-16);

    // generate some 8bpp tiles
    gen_affine_tiles(videoRAM, videoRAM + 0x800, 16);

    wait_for_exit();
}

static void size_test(screen_size size) {
    reg::dispcnt::write({
        .mode = 1,
        .layer_background_2 = true,
    });
    reg::bg2cnt::write(background_control {
        .screen_base_block = 2,
        .affine_background_wrap = affine_background_wrap::wrap,
        .screen_size = size
    });

    int size_px = 128 << static_cast<int>(size);

    // center it
    reset_affine();
    reg::bg2x::write((size_px - 240) / 2);
    reg::bg2y::write((size_px - 160) / 2);

    // generate some 8bpp tiles
    gen_affine_tiles(videoRAM, videoRAM + 0x800, size_px / 8);

    wait_for_exit();
}

void display_layer2_mode1_size1() {
    size_test(screen_size::affine_32x32);
}

void display_layer2_mode1_size2() {
    size_test(screen_size::affine_64x64);
}

void display_layer2_mode1_size3() {
    size_test(screen_size::affine_128x128);
}

void display_layer2_mode1_rotscale() {
    reg::dispcnt::write({
        .mode = 1,
        .layer_background_2 = true,
    });
    reg::bg2cnt::write(background_control {
        .screen_base_block = 2
    });

    // transform it
    static const bios::bg_affine_input input {
        .origin_x = 64,
        .origin_y = 64,
        .display_x = 120,
        .display_y = 80,
        .scale_x = 0.5f,
        .scale_y = 0.5f,
        .rotation = 0x20 // 1/8
    };

    auto output = reinterpret_cast<bios::bg_affine_output *>(reg::bg2pa::address);

    bios::bg_affine_set(&input, output, 1);

    // generate some 8bpp tiles
    gen_affine_tiles(videoRAM, videoRAM + 0x800, 16);

    wait_for_exit();
}

void display_layer2_mode2() {
    // mode 2 is the same as mode 1, so this is the rotscale test again

    reg::dispcnt::write({
        .mode = 2,
        .layer_background_2 = true,
    });
    reg::bg2cnt::write(background_control {
        .screen_base_block = 2
    });

    // transform it
    static const bios::bg_affine_input input {
        .origin_x = 64,
        .origin_y = 64,
        .display_x = 120,
        .display_y = 80,
        .scale_x = 0.5f,
        .scale_y = 0.5f,
        .rotation = 0x20 // 1/8
    };

    auto output = reinterpret_cast<bios::bg_affine_output *>(reg::bg2pa::address);

    bios::bg_affine_set(&input, output, 1);

    // generate some 8bpp tiles
    gen_affine_tiles(videoRAM, videoRAM + 0x800, 16);

    wait_for_exit();
}

// bitmap modes

void display_layer2_mode3() {
    reg::dispcnt::write({
        .mode = 3,
        .layer_background_2 = true,
    });

    reg::bg2cnt::write({});

    // nope
    reg::bg2hofs::write(100);
    reg::bg2vofs::write(100);

    reset_affine();

    for(int y = 0; y < 160; y++) {
        for(int x = 0; x < 240; x++) {
            videoRAM[x + y * 240] = (x / 8) | (y / 5) << 5 | (0x1F - x / 8) << 10;
        }
    }

    wait_for_exit();
}

void display_layer2_mode3_rotscale() {
    // 240x160 bgr555
    reg::dispcnt::write({
        .mode = 3,
        .layer_background_2 = true,
    });

    // this has no effect
    reg::bg2cnt::write({.affine_background_wrap = affine_background_wrap::wrap});

    // transform it
    static const bios::bg_affine_input input {
        .origin_x = 120,
        .origin_y = 80,
        .display_x = 120,
        .display_y = 80,
        .scale_x = 2.0f,
        .scale_y = 2.0f,
        .rotation = 0x20 // 1/8
    };

    auto output = reinterpret_cast<bios::bg_affine_output *>(reg::bg2pa::address);
    bios::bg_affine_set(&input, output, 1);

    for(int y = 0; y < 160; y++) {
        for(int x = 0; x < 240; x++) {
            videoRAM[x + y * 240] = (x / 8) | (y / 5) << 5 | (0x1F - x / 8) << 10;
        }
    }

    wait_for_exit();
}

void display_layer2_mode4() {
    // 240x160 paletted
    reg::dispcnt::write({
        .mode = 4,
        .layer_background_2 = true,
    });

    reg::bg2cnt::write({});

    // nope
    reg::bg2hofs::write(100);
    reg::bg2vofs::write(100);

    reset_affine();

    for(int i = 1; i < 256; i++)
        palRAM[i] = i >> 3 | (0x1F - (i >> 3)) << 10;

    for(int y = 0; y < 160; y++) {
        for(int x = 0; x < 240; x += 2) {
            int pal_index = (x + y + 1);
            if(pal_index >= 256)
                pal_index = 255 - (pal_index & 0xFF);

            int pal_index2 = pal_index == 255 ? 255 : pal_index + 1;

            videoRAM[(x + y * 240) / 2] = pal_index | (pal_index2) << 8;
        }
    }

    wait_for_exit();
}

void display_layer2_mode4_pageflip() {
    reg::dispcnt::write({
        .mode = 4,
        .page = 1,
        .layer_background_2 = true,
    });

    reg::bg2cnt::write({});

    reset_affine();

    for(int i = 1; i < 256; i++)
        palRAM[i] = i >> 3 | (0x1F - (i >> 3)) << 10;

    for(int y = 0; y < 160; y++) {
        for(int x = 0; x < 240; x += 2) {
            int pal_index = (x + y + 1);
            if(pal_index >= 256)
                pal_index = 255 - (pal_index & 0xFF);

            int pal_index2 = pal_index == 255 ? 255 : pal_index + 1;

            videoRAM[(x + y * 240) / 2] = 0xFFFF; // fill page 0 with solid red

            videoRAM[(x + y * 240 + 0xA000) / 2] = pal_index | (pal_index2) << 8;
        }
    }

    wait_for_exit();
}

void display_layer2_mode4_rotscale() {
    reg::dispcnt::write({
        .mode = 4,
        .layer_background_2 = true,
    });

    reg::bg2cnt::write({.affine_background_wrap = affine_background_wrap::wrap});

    // transform it
    static const bios::bg_affine_input input {
        .origin_x = 120,
        .origin_y = 80,
        .display_x = 120,
        .display_y = 80,
        .scale_x = 2.0f,
        .scale_y = 2.0f,
        .rotation = 0x20 // 1/8
    };

    auto output = reinterpret_cast<bios::bg_affine_output *>(reg::bg2pa::address);
    bios::bg_affine_set(&input, output, 1);

    for(int i = 1; i < 256; i++)
        palRAM[i] = i >> 3 | (0x1F - (i >> 3)) << 10;

    for(int y = 0; y < 160; y++) {
        for(int x = 0; x < 240; x += 2) {
            int pal_index = (x + y + 1);
            if(pal_index >= 256)
                pal_index = 255 - (pal_index & 0xFF);

            int pal_index2 = pal_index == 255 ? 255 : pal_index + 1;

            videoRAM[(x + y * 240) / 2] = pal_index | (pal_index2) << 8;
        }
    }

    wait_for_exit();
}

void display_layer2_mode5() {
    // 160x128 rgb555
    reg::dispcnt::write({
        .mode = 5,
        .layer_background_2 = true,
    });

    reg::bg2cnt::write({});

    // nope
    reg::bg2hofs::write(100);
    reg::bg2vofs::write(100);

    reset_affine();

    for(int y = 0; y < 128; y++) {
        for(int x = 0; x < 160; x++) {
            videoRAM[x + y * 160] = (x / 5) | (y / 4) << 5 | (0x1F - x / 5) << 10;
        }
    }

    wait_for_exit();
}

void display_layer2_mode5_pageflip() {
    // 160x128 rgb555
    reg::dispcnt::write({
        .mode = 5,
        .page = 1,
        .layer_background_2 = true,
    });

    reg::bg2cnt::write({});

    reset_affine();

    for(int y = 0; y < 128; y++) {
        for(int x = 0; x < 160; x++) {
            videoRAM[x + y * 160] = 0x001F; // fill page 0 with solid red
            videoRAM[x + y * 160 + 0xA000 / 2] = (x / 5) | (y / 4) << 5 | (0x1F - x / 5) << 10;
        }
    }

    wait_for_exit();
}

void display_layer2_mode5_rotscale() {
    reg::dispcnt::write({
        .mode = 5,
        .layer_background_2 = true,
    });

    reg::bg2cnt::write({.affine_background_wrap = affine_background_wrap::wrap});

    // transform it
    static const bios::bg_affine_input input {
        .origin_x = 80,
        .origin_y = 64,
        .display_x = 120,
        .display_y = 80,
        .scale_x = 2.0f,
        .scale_y = 2.0f,
        .rotation = 0x20 // 1/8
    };

    auto output = reinterpret_cast<bios::bg_affine_output *>(reg::bg2pa::address);
    bios::bg_affine_set(&input, output, 1);

    for(int y = 0; y < 128; y++) {
        for(int x = 0; x < 160; x++) {
            videoRAM[x + y * 160] = (x / 5) | (y / 4) << 5 | (0x1F - x / 5) << 10;
        }
    }

    wait_for_exit();
}