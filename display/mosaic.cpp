#include <gba/gba.hpp>

#include "../common.hpp"

#include "../assets/sprites.h"

using namespace gba;

static void mosaic_mode0(uint32_t size_x, uint32_t size_y, int x = 0, int y = 0) {
    mmio::DISPCNT = {
        .video_mode = 0,
        .show_bg0 = true,
    };

    mmio::BG0CNT = {
        .mosaic = true,
        .screenblock = 2,
    };

    mmio::MOSAIC = {
        .bg_h_extra = size_x - 1, // TODO: cleanup +/-1
        .bg_v_extra = size_y - 1
    };

    palette_ram[1] = 0x03F0;
    palette_ram[2] = 0x001F;
    palette_ram[3] = 0x7C10;
    palette_ram[4] = 0x7FE0;
    palette_ram[5] = 0x0010;
    palette_ram[6] = 0x0308;
    palette_ram[7] = 0x4200;
    palette_ram[8] = 0x4008;

    for(int y = 0; y < 32; y++) {
        for(int x = 0; x < 32; x++)
            video_ram[0x800 + x + y * 32] = 0x10 + ((x + y) & 3);
    }

    // adjust font colours
    // 0/1 -> 1/2, 3/4, 5/6, 7/8
    for(int i = 0; i < 4; i++) {
        for(int y = 0; y < 8; y++) {
            video_ram[(i + 16) * 16 + y * 2 + 0] += 0x11111111 * (i * 2 + 1);
            video_ram[(i + 16) * 16 + y * 2 + 1] += 0x11111111 * (i * 2 + 1);
        }
    }

    mmio::BG0HOFS = x;
    mmio::BG0VOFS = y;

    wait_for_exit();
}

void display_mosaic_mode0_0() {
   mosaic_mode0(1, 1);
}

void display_mosaic_mode0_1_1() {
   mosaic_mode0(2, 2);
}

void display_mosaic_mode0_4_4() {
   mosaic_mode0(5, 5);
}

void display_mosaic_mode0_9_9() {
   mosaic_mode0(10, 10);
}

void display_mosaic_mode0_15_15() {
   mosaic_mode0(16, 16);
}

void display_mosaic_mode0_0_15() {
   mosaic_mode0(1, 16);
}

void display_mosaic_mode0_15_0() {
   mosaic_mode0(16, 1);
}

// scroll it a little
void display_mosaic_mode0_4_4_scroll() {
   mosaic_mode0(5, 5, 1, 2);
}

static const int bg2pa_addr = 0x4000020;

static void mosaic_mode2(uint32_t size_x, uint32_t size_y, bool transform = false) {
    mmio::DISPCNT = {
        .video_mode = 2,
        .show_bg2 = true,
    };

    mmio::BG2CNT = {
        .mosaic = true,
        .screenblock = 2,
    };

    mmio::MOSAIC = {
        .bg_h_extra = size_x - 1,
        .bg_v_extra = size_y - 1
    };

    gen_affine_tiles(video_ram, video_ram + 0x800, 16);

    if(transform) {
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
    } else {
        // reset affine
        mmio::BG2PA = 1;
        mmio::BG2PB = 0;
        mmio::BG2PC = 0;
        mmio::BG2PD = 1;
        mmio::BG2X = 0;
        mmio::BG2Y = 0;
    }

    wait_for_exit();
}

void display_mosaic_mode2_0() {
   mosaic_mode2(1, 1);
}

void display_mosaic_mode2_1_1() {
   mosaic_mode2(2, 2);
}

void display_mosaic_mode2_4_4() {
   mosaic_mode2(5, 5);
}

void display_mosaic_mode2_9_9() {
   mosaic_mode2(10, 10);
}

void display_mosaic_mode2_15_15() {
   mosaic_mode2(16, 16);
}

void display_mosaic_mode2_0_15() {
   mosaic_mode2(1, 16);
}

void display_mosaic_mode2_15_0() {
   mosaic_mode2(16, 1);
}

// repeat, but transformed
void display_mosaic_mode2_1_1_rotscale() {
   mosaic_mode2(2, 2, true);
}

void display_mosaic_mode2_4_4_rotscale() {
   mosaic_mode2(5, 5, true);
}

void display_mosaic_mode2_9_9_rotscale() {
   mosaic_mode2(10, 10, true);
}

void display_mosaic_mode2_15_15_rotscale() {
   mosaic_mode2(16, 16, true);
}

void display_mosaic_mode2_0_15_rotscale() {
   mosaic_mode2(1, 16, true);
}

void display_mosaic_mode2_15_0_rotscale() {
   mosaic_mode2(16, 1, true);
}

// yes bitmap mosaic?
static void mosaic_mode3(uint32_t size_x, uint32_t size_y) {
    mmio::DISPCNT = {
        .video_mode = 3,
        .show_bg2 = true,
    };

    mmio::BG2CNT = {
        .mosaic = true,
    };

    mmio::MOSAIC = {
        .bg_h_extra = size_x - 1,
        .bg_v_extra = size_y - 1
    };

    mmio::BG2PA = 1;
    mmio::BG2PB = 0;
    mmio::BG2PC = 0;
    mmio::BG2PD = 1;
    mmio::BG2X = 0;
    mmio::BG2Y = 0;

    for(int y = 0; y < 160; y++) {
        for(int x = 0; x < 240; x++) {
            video_ram[x + y * 240] = (x / 8) | (y / 5) << 5 | (0x1F - x / 8) << 10;
        }
    }

    wait_for_exit();
}

void display_mosaic_mode3_0() {
   mosaic_mode3(1, 1);
}

void display_mosaic_mode3_4_4() {
   mosaic_mode3(5, 5);
}

void display_mosaic_mode3_15_15() {
   mosaic_mode3(16, 16);
}

void display_mosaic_mode3_0_15() {
   mosaic_mode3(1, 16);
}

void display_mosaic_mode3_15_0() {
   mosaic_mode3(16, 1);
}

static void mosaic_mode4(uint32_t size_x, uint32_t size_y) {
    mmio::DISPCNT = {
        .video_mode = 4,
        .show_bg2 = true,
    };

    mmio::BG2CNT = {
        .mosaic = true,
    };

    mmio::MOSAIC = {
        .bg_h_extra = size_x - 1,
        .bg_v_extra = size_y - 1
    };

    mmio::BG2PA = 1;
    mmio::BG2PB = 0;
    mmio::BG2PC = 0;
    mmio::BG2PD = 1;
    mmio::BG2X = 0;
    mmio::BG2Y = 0;

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

void display_mosaic_mode4_0() {
   mosaic_mode4(1, 1);
}

void display_mosaic_mode4_4_4() {
   mosaic_mode4(5, 5);
}

void display_mosaic_mode4_15_15() {
   mosaic_mode4(16, 16);
}

void display_mosaic_mode4_0_15() {
   mosaic_mode4(1, 16);
}

void display_mosaic_mode4_15_0() {
   mosaic_mode4(16, 1);
}

static void mosaic_mode5(uint32_t size_x, uint32_t size_y) {
    mmio::DISPCNT = {
        .video_mode = 5,
        .show_bg2 = true,
    };

    mmio::BG2CNT = {
        .mosaic = true,
    };

    mmio::MOSAIC = {
        .bg_h_extra = size_x - 1,
        .bg_v_extra = size_y - 1
    };

    mmio::BG2PA = 1;
    mmio::BG2PB = 0;
    mmio::BG2PC = 0;
    mmio::BG2PD = 1;
    mmio::BG2X = 0;
    mmio::BG2Y = 0;

    for(int y = 0; y < 128; y++) {
        for(int x = 0; x < 160; x++) {
            video_ram[x + y * 160] = (x / 5) | (y / 4) << 5 | (0x1F - x / 5) << 10;
        }
    }

    wait_for_exit();
}

void display_mosaic_mode5_0() {
   mosaic_mode5(1, 1);
}

void display_mosaic_mode5_4_4() {
   mosaic_mode5(5, 5);
}

void display_mosaic_mode5_15_15() {
   mosaic_mode5(16, 16);
}

void display_mosaic_mode5_0_15() {
   mosaic_mode5(1, 16);
}

void display_mosaic_mode5_15_0() {
   mosaic_mode5(16, 1);
}

static const int oam_addr = 0x07000000;

static void set_object(int index, objattr0 attr0, objattr1 attr1, objattr2 attr2) {
    objattr object {attr0, attr1, attr2};
    __agbabi_memcpy2(reinterpret_cast<void *>(oam_addr + index * sizeof(objattr8)), &object, sizeof(object));
}

static void mosaic_objects(obj_display mode, uint32_t size_x, uint32_t size_y) {
    palette_ram[0] = 0x4210;

    // load sprites
    __agbabi_memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    __agbabi_memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

    mmio::DISPCNT = {
        .video_mode = 0,
        .show_obj = true,
    };

    mmio::MOSAIC = {
        .obj_h_extra = size_x - 1,
        .obj_v_extra = size_y - 1
    };

    // modified a bit from object tests setup_objects
    const int spacing = 9;
    uint16_t x = spacing, y = spacing;

    // size 0

    // 8x8
    set_object(0, objattr0 {
        .y = y,
        .style = mode,
        .mosaic = true,
        .shape = obj_shape::square
    }, objattr1 {
        .x = x,
        .size = 0
    }, objattr2 {
        .tile_id = 0,
    });
    y += 8 + spacing;

    // 16x8
    set_object(1, objattr0 {
        .y = y,
        .style = mode,
        .mosaic = true,
        .shape = obj_shape::horizontal
    }, objattr1 {
        .x = x,
        .size = 0
    }, objattr2 {
        .tile_id = 1,
    });

    y += 8 + spacing;

    // 8x16
    set_object(2, objattr0 {
        .y = y,
        .style = mode,
        .mosaic = true,
        .shape = obj_shape::vertical
    }, objattr1 {
        .x = x,
        .size = 0
    }, objattr2 {
        .tile_id = 32,
    });

    y = spacing;
    x += 16 + spacing;

    // size 1

    // 16x16
    set_object(3, objattr0 {
        .y = y,
        .style = mode,
        .mosaic = true,
        .shape = obj_shape::square
    }, objattr1 {
        .x = x,
        .size = 1
    }, objattr2 {
        .tile_id = 3,
    });

    y += 16 + spacing;

    // 32x8
    set_object(4, objattr0 {
        .y = y,
        .style = mode,
        .mosaic = true,
        .shape = obj_shape::horizontal
    }, objattr1 {
        .x = x,
        .size = 1
    }, objattr2 {
        .tile_id = 5,
    });

    y += 8 + spacing;

    // 8x32
    set_object(5, objattr0 {
        .y = y,
        .style = mode,
        .mosaic = true,
        .shape = obj_shape::vertical
    }, objattr1 {
        .x = x,
        .size = 1
    }, objattr2 {
        .tile_id = 9,
    });

    x += 32 + spacing;
    y = spacing;

    // size 2

    // 32x32
    set_object(6, objattr0 {
        .y = y,
        .style = mode,
        .mosaic = true,
        .shape = obj_shape::square
    }, objattr1 {
        .x = x,
        .size = 2
    }, objattr2 {
        .tile_id = 10,
    });

    y += 32 + spacing;

    // 32x16
    set_object(7, objattr0 {
        .y = y,
        .style = mode,
        .mosaic = true,
        .shape = obj_shape::horizontal
    }, objattr1 {
        .x = x,
        .size = 2
    }, objattr2 {
        .tile_id = 14,
    });

    y += 16 + spacing;

    // 16x32
    set_object(8, objattr0 {
        .y = y,
        .style = mode,
        .mosaic = true,
        .shape = obj_shape::vertical
    }, objattr1 {
        .x = x,
        .size = 2
    }, objattr2 {
        .tile_id = 18,
    });

    x += 32 + spacing;
    y = spacing;

    // size 3

    // 64x64
    set_object(9, objattr0 {
        .y = y,
        .style = mode,
        .mosaic = true,
        .shape = obj_shape::square
    }, objattr1 {
        .x = x,
        .size = 3
    }, objattr2 {
        .tile_id = 20,
    });

    y += 64 + spacing;

    // 64x32
    set_object(10, objattr0 {
        .y = y,
        .style = mode,
        .mosaic = true,
        .shape = obj_shape::horizontal
    }, objattr1 {
        .x = x,
        .size = 3
    }, objattr2 {
        .tile_id = 140,
    });

    // new column, no space left
    y = spacing;
    x += 64 + spacing;

    // 32x64
    set_object(11, objattr0 {
        .y = y,
        .style = mode,
        .mosaic = true,
        .shape = obj_shape::vertical
    }, objattr1 {
        .x = x,
        .size = 3
    }, objattr2 {
        .tile_id = 28,
    });

    // hide everything else off-screen
    for(int i = 12; i < 128; i++)
        set_object(i, objattr0 {.y = 160}, objattr1 {}, {});

    // just the one transform for now
    static const bios::obj_affine_src input {
        .sx = 1.099f,
        .sy = 1.099f,
        .alpha = 0x2000
    };

    bios::ObjAffineSet(&input, reinterpret_cast<fixed<short, 8> *>(oam_addr + 6), 1, 8);

    wait_for_exit();
}

void display_mosaic_objects_regular_1_1() {
   mosaic_objects(obj_display::normal, 2, 2);
}

void display_mosaic_objects_regular_4_4() {
   mosaic_objects(obj_display::normal, 5, 5);
}

void display_mosaic_objects_regular_9_9() {
   mosaic_objects(obj_display::normal, 10, 10);
}

void display_mosaic_objects_regular_15_15() {
   mosaic_objects(obj_display::normal, 16, 16);
}

void display_mosaic_objects_regular_0_15() {
   mosaic_objects(obj_display::normal, 1, 16);
}

void display_mosaic_objects_regular_15_0() {
   mosaic_objects(obj_display::normal, 16, 1);
}

void display_mosaic_objects_affine_1_1() {
   mosaic_objects(obj_display::affine, 2, 2);
}

void display_mosaic_objects_affine_4_4() {
   mosaic_objects(obj_display::affine, 5, 5);
}

void display_mosaic_objects_affine_9_9() {
   mosaic_objects(obj_display::affine, 10, 10);
}

void display_mosaic_objects_affine_15_15() {
   mosaic_objects(obj_display::affine, 16, 16);
}

void display_mosaic_objects_affine_0_15() {
   mosaic_objects(obj_display::affine, 1, 16);
}

void display_mosaic_objects_affine_15_0() {
   mosaic_objects(obj_display::affine, 16, 1);
}

void display_mosaic_window() {
    // load sprites, but don't bother with the palette
    __agbabi_memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));

    mmio::MOSAIC = {
        .bg_h_extra = 6 - 1,
        .bg_v_extra = 9 - 1
    };

    // enable ALL the windows
    mmio::DISPCNT = {
        .video_mode = 0,
        .show_bg0 = true,
        .show_bg1 = true,
        .show_bg2 = true,
        .show_bg3 = true,
        .show_obj = true,
        .enable_win0 = true,
        .enable_win1 = true,
        .enable_obj_win = true
    };

    mmio::WIN0H = u8x2{140 - 1, 10};
    mmio::WIN0V = u8x2{100 - 1, 10};

    mmio::WIN1H = u8x2{240 - 20 - 1, 15};
    mmio::WIN1V = u8x2{160 - 20 - 1, 30};

    mmio::WININ = {
        .win0_bg3 = true,

        .win1_bg2 = true,
    };

    mmio::WINOUT = {
        .outside_bg0 = true,

        // this is the obj window
        .obj_win_bg1 = true,
    };

    palette_ram[0] = 0x4210;

    objattr obj {
        {.y = 5, .mode = obj_effect::window},
        {.x = 100, .size = 3},
        {.tile_id = 20}
    };

    __agbabi_memcpy2(reinterpret_cast<void*>(0x7000000), &obj, sizeof(obj));

    // setup_layers();
    mmio::BG0CNT = {
        .mosaic = true,
        .screenblock = 2,
    };

    mmio::BG1CNT = {
        .mosaic = true,
        .screenblock = 3,
    };

    mmio::BG2CNT = {
        .screenblock = 4,
    };

    mmio::BG3CNT = {
        .mosaic = true,
        .screenblock = 5,
    };

    palette_ram[1] = 0x03F0;
    palette_ram[2] = 0x001F;
    palette_ram[3] = 0x7C10;
    palette_ram[4] = 0x7FE0;
    palette_ram[5] = 0x0010;
    palette_ram[6] = 0x0308;
    palette_ram[7] = 0x4200;
    palette_ram[8] = 0x4008;

    // fill each block with a digit
    __agbabi_wordset4(video_ram + 0x0800, 0x800, 0x00100010);
    __agbabi_wordset4(video_ram + 0x0C00, 0x800, 0x00110011);
    __agbabi_wordset4(video_ram + 0x1000, 0x800, 0x00120012);
    __agbabi_wordset4(video_ram + 0x1400, 0x800, 0x00130013);

    // adjust font colours
    // 0/1 -> 1/2, 3/4, 5/6, 7/8
    for(int i = 0; i < 4; i++) {
        for(int y = 0; y < 8; y++) {
            video_ram[(i + 16) * 16 + y * 2 + 0] += 0x11111111 * (i * 2 + 1);
            video_ram[(i + 16) * 16 + y * 2 + 1] += 0x11111111 * (i * 2 + 1);
        }
    }

    mmio::BG0HOFS = 0;
    mmio::BG0VOFS = 0;
    mmio::BG1HOFS = 0;
    mmio::BG1VOFS = 0;
    mmio::BG2HOFS = 0;
    mmio::BG2VOFS = 0;
    mmio::BG3HOFS = 0;
    mmio::BG3VOFS = 0;

    wait_for_exit();
}
