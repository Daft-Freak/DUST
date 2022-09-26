#include <gba/gba.hpp>

#include "../common.hpp"

using namespace gba;

// layer2, "text" (mode 0), affine (mode 1-2), bitmap (mode 5-5)

void display_layer2_mode0() {
    // basic test, mode 1 is identical for all layers

    mmio::DISPCNT = {
        .video_mode = 0,
        .show_bg2 = true,
    };
    mmio::BG2CNT = {
        .screenblock = 2,
    };

    mmio::BG2HOFS = 0;
    mmio::BG2VOFS = 0;

    clear_text();
    write_text(5, 10, "Hello from layer 2!");

    wait_for_exit();
}

static void reset_affine() {
    mmio::BG2PA = 1;
    mmio::BG2PB = 0;
    mmio::BG2PC = 0;
    mmio::BG2PD = 1;
    mmio::BG2X = 0;
    mmio::BG2Y = 0;
}

void display_layer2_mode1() {
    // affine
    mmio::DISPCNT = {
        .video_mode = 1,
        .show_bg2 = true,
    };
    mmio::BG2CNT = {
        .screenblock = 2,
    };

    // these have no effect
    mmio::BG2HOFS = 100;
    mmio::BG2VOFS = 100;

    reset_affine();

    // generate some 8bpp tiles
    gen_affine_tiles(video_ram, video_ram + 0x800, 16);

    wait_for_exit();
}

void display_layer2_mode1_char_base() {
    // same thing, different char base
    mmio::DISPCNT = {
        .video_mode = 1,
        .show_bg2 = true,
    };
    mmio::BG2CNT = {
        .charblock = 1,
        .screenblock = 2,
    };

    reset_affine();

    // generate some 8bpp tiles
    gen_affine_tiles(video_ram + 0x2000, video_ram + 0x800, 16);

    wait_for_exit();
}

void display_layer2_mode1_wrap() {
    mmio::DISPCNT = {
        .video_mode = 1,
        .show_bg2 = true,
    };
    mmio::BG2CNT = {
        .screenblock = 2,
        .is_affine_wrapping = true
    };

    // center it
    reset_affine();
    mmio::BG2X = -56;
    mmio::BG2Y = -16;

    // generate some 8bpp tiles
    gen_affine_tiles(video_ram, video_ram + 0x800, 16);

    wait_for_exit();
}

static void size_test(uint16_t size) {
    mmio::DISPCNT = {
        .video_mode = 1,
        .show_bg2 = true,
    };
    mmio::BG2CNT = {
        .screenblock = 2,
        .is_affine_wrapping = true,
        .size = size
    };

    int size_px = 128 << static_cast<int>(size);

    // center it
    reset_affine();
    mmio::BG2X = (size_px - 240 / 2);
    mmio::BG2Y = (size_px - 160 / 2);

    // generate some 8bpp tiles
    gen_affine_tiles(video_ram, video_ram + 0x800, size_px / 8);

    wait_for_exit();
}

void display_layer2_mode1_size1() {
    size_test(1);
}

void display_layer2_mode1_size2() {
    size_test(2);
}

void display_layer2_mode1_size3() {
    size_test(3);
}

static const int bg2pa_addr = 0x4000020;

void display_layer2_mode1_rotscale() {
    mmio::DISPCNT = {
        .video_mode = 1,
        .show_bg2 = true,
    };
    mmio::BG2CNT = {
        .screenblock = 2
    };

    // transform it
    static const bios::bg_affine_src input {
        .tex_x = 64,
        .tex_y = 64,
        .scr_x = 120,
        .scr_y = 80,
        .sx = 0.5f,
        .sy = 0.5f,
        .alpha = 0x2000 // 1/8
    };

    auto output = reinterpret_cast<bios::bg_affine_dest *>(bg2pa_addr);

    bios::BgAffineSet(&input, output, 1);

    // generate some 8bpp tiles
    gen_affine_tiles(video_ram, video_ram + 0x800, 16);

    wait_for_exit();
}

void display_layer2_mode2() {
    // mode 2 is the same as mode 1, so this is the rotscale test again

    mmio::DISPCNT = {
        .video_mode = 2,
        .show_bg2 = true,
    };
    mmio::BG2CNT = {
        .screenblock = 2
    };

    // transform it
    static const bios::bg_affine_src input {
        .tex_x = 64,
        .tex_y = 64,
        .scr_x = 120,
        .scr_y = 80,
        .sx = 0.5f,
        .sy = 0.5f,
        .alpha = 0x2000 // 1/8
    };

    auto output = reinterpret_cast<bios::bg_affine_dest *>(bg2pa_addr);

    bios::BgAffineSet(&input, output, 1);

    // generate some 8bpp tiles
    gen_affine_tiles(video_ram, video_ram + 0x800, 16);

    wait_for_exit();
}

// bitmap modes

void display_layer2_mode3() {
    mmio::DISPCNT = {
        .video_mode = 3,
        .show_bg2 = true,
    };

    mmio::BG2CNT = {};

    // nope
    mmio::BG2HOFS = 100;
    mmio::BG2VOFS = 100;

    reset_affine();

    for(int y = 0; y < 160; y++) {
        for(int x = 0; x < 240; x++) {
            video_ram[x + y * 240] = (x / 8) | (y / 5) << 5 | (0x1F - x / 8) << 10;
        }
    }

    wait_for_exit();
}

void display_layer2_mode3_rotscale() {
    // 240x160 bgr555
    mmio::DISPCNT = {
        .video_mode = 3,
        .show_bg2 = true,
    };

    // this has no effect
    mmio::BG2CNT = {.is_affine_wrapping = true};

    // transform it
    static const bios::bg_affine_src input {
        .tex_x = 120,
        .tex_y = 80,
        .scr_x = 120,
        .scr_y = 80,
        .sx = 2.0f,
        .sy = 2.0f,
        .alpha = 0x2000 // 1/8
    };

    auto output = reinterpret_cast<bios::bg_affine_dest *>(bg2pa_addr);
    bios::BgAffineSet(&input, output, 1);


    for(int y = 0; y < 160; y++) {
        for(int x = 0; x < 240; x++) {
            video_ram[x + y * 240] = (x / 8) | (y / 5) << 5 | (0x1F - x / 8) << 10;
        }
    }

    wait_for_exit();
}

void display_layer2_mode4() {
    // 240x160 paletted
    mmio::DISPCNT = {
        .video_mode = 4,
        .show_bg2 = true,
    };

    mmio::BG2CNT = {};

    // nope
    mmio::BG2HOFS = 100;
    mmio::BG2VOFS = 100;

    reset_affine();

    for(int i = 1; i < 256; i++)
        palette_ram[i] = i >> 3 | (0x1F - (i >> 3)) << 10;

    for(int y = 0; y < 160; y++) {
        for(int x = 0; x < 240; x += 2) {
            int pal_index = (x + y + 1);
            if(pal_index >= 256)
                pal_index = 255 - (pal_index & 0xFF);

            int pal_index2 = pal_index == 255 ? 255 : pal_index + 1;

            video_ram[(x + y * 240) / 2] = pal_index | (pal_index2) << 8;
        }
    }

    wait_for_exit();
}

void display_layer2_mode4_pageflip() {
    mmio::DISPCNT = {
        .video_mode = 4,
        .page = 1,
        .show_bg2 = true,
    };

    mmio::BG2CNT = {};

    reset_affine();

    for(int i = 1; i < 256; i++)
        palette_ram[i] = i >> 3 | (0x1F - (i >> 3)) << 10;

    for(int y = 0; y < 160; y++) {
        for(int x = 0; x < 240; x += 2) {
            int pal_index = (x + y + 1);
            if(pal_index >= 256)
                pal_index = 255 - (pal_index & 0xFF);

            int pal_index2 = pal_index == 255 ? 255 : pal_index + 1;

            video_ram[(x + y * 240) / 2] = 0xFFFF; // fill page 0 with solid red

            video_ram[(x + y * 240 + 0xA000) / 2] = pal_index | (pal_index2) << 8;
        }
    }

    wait_for_exit();
}

void display_layer2_mode4_rotscale() {
    mmio::DISPCNT = {
        .video_mode = 4,
        .show_bg2 = true,
    };

    mmio::BG2CNT = {.is_affine_wrapping = true};

    // transform it
    static const bios::bg_affine_src input {
        .tex_x = 120,
        .tex_y = 80,
        .scr_x = 120,
        .scr_y = 80,
        .sx = 2.0f,
        .sy = 2.0f,
        .alpha = 0x2000 // 1/8
    };

    auto output = reinterpret_cast<bios::bg_affine_dest *>(bg2pa_addr);
    bios::BgAffineSet(&input, output, 1);

    for(int i = 1; i < 256; i++)
        palette_ram[i] = i >> 3 | (0x1F - (i >> 3)) << 10;

    for(int y = 0; y < 160; y++) {
        for(int x = 0; x < 240; x += 2) {
            int pal_index = (x + y + 1);
            if(pal_index >= 256)
                pal_index = 255 - (pal_index & 0xFF);

            int pal_index2 = pal_index == 255 ? 255 : pal_index + 1;

            video_ram[(x + y * 240) / 2] = pal_index | (pal_index2) << 8;
        }
    }

    wait_for_exit();
}

void display_layer2_mode5() {
    // 160x128 rgb555
    mmio::DISPCNT = {
        .video_mode = 5,
        .show_bg2 = true,
    };

    mmio::BG2CNT = {};

    // nope
    mmio::BG2HOFS = 100;
    mmio::BG2VOFS = 100;

    reset_affine();

    for(int y = 0; y < 128; y++) {
        for(int x = 0; x < 160; x++) {
            video_ram[x + y * 160] = (x / 5) | (y / 4) << 5 | (0x1F - x / 5) << 10;
        }
    }

    wait_for_exit();
}

void display_layer2_mode5_pageflip() {
    // 160x128 rgb555
    mmio::DISPCNT = {
        .video_mode = 5,
        .page = 1,
        .show_bg2 = true,
    };

    mmio::BG2CNT = {};

    reset_affine();

    for(int y = 0; y < 128; y++) {
        for(int x = 0; x < 160; x++) {
            video_ram[x + y * 160] = 0x001F; // fill page 0 with solid red
            video_ram[x + y * 160 + 0xA000 / 2] = (x / 5) | (y / 4) << 5 | (0x1F - x / 5) << 10;
        }
    }

    wait_for_exit();
}

void display_layer2_mode5_rotscale() {
    mmio::DISPCNT = {
        .video_mode = 5,
        .show_bg2 = true,
    };

    mmio::BG2CNT = {.is_affine_wrapping = true};

    // transform it
    static const bios::bg_affine_src input {
        .tex_x = 80,
        .tex_y = 64,
        .scr_x = 120,
        .scr_y = 80,
        .sx = 2.0f,
        .sy = 2.0f,
        .alpha = 0x2000 // 1/8
    };

    auto output = reinterpret_cast<bios::bg_affine_dest *>(bg2pa_addr);
    bios::BgAffineSet(&input, output, 1);


    for(int y = 0; y < 128; y++) {
        for(int x = 0; x < 160; x++) {
            video_ram[x + y * 160] = (x / 5) | (y / 4) << 5 | (0x1F - x / 5) << 10;
        }
    }

    wait_for_exit();
}
