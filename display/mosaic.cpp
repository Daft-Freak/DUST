#include <gba/gba.hpp>

#include "../common.hpp"

#include "../assets/sprites.h"

using namespace gba;

static void mosaic_mode0(uint32_t size_x, uint32_t size_y, int x = 0, int y = 0) {
    reg::dispcnt::write({
        .mode = 0,
        .layer_background_0 = true,
    });
    
    reg::bg0cnt::write(background_control {
        .mosaic = true,
        .screen_base_block = 2,
    });

    reg::mosaic::emplace(mosaic_size {
        .background = {size_x,  size_y}
    });

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

    reg::bg0hofs::write(x);
    reg::bg0vofs::write(y);

    wait_for_exit();
}

void display_mosiac_mode0_0() {
   mosaic_mode0(1, 1);
}

void display_mosiac_mode0_1_1() {
   mosaic_mode0(2, 2);
}

void display_mosiac_mode0_4_4() {
   mosaic_mode0(5, 5);
}

void display_mosiac_mode0_9_9() {
   mosaic_mode0(10, 10);
}

void display_mosiac_mode0_15_15() {
   mosaic_mode0(16, 16);
}

void display_mosiac_mode0_0_15() {
   mosaic_mode0(1, 16);
}

void display_mosiac_mode0_15_0() {
   mosaic_mode0(16, 1);
}

// scroll it a little
void display_mosiac_mode0_4_4_scroll() {
   mosaic_mode0(5, 5, 1, 2);
}

static void mosaic_mode2(uint32_t size_x, uint32_t size_y, bool transform = false) {
    reg::dispcnt::write({
        .mode = 2,
        .layer_background_2 = true,
    });
    
    reg::bg2cnt::write(background_control {
        .mosaic = true,
        .screen_base_block = 2,
    });

    reg::mosaic::emplace(mosaic_size {
        .background = {size_x,  size_y}
    });

    gen_affine_tiles(video_ram, video_ram + 0x800, 16);

    if(transform) {
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
    } else {
        // reset affine
        reg::bg2pa::write(1);
        reg::bg2pb::write(0);
        reg::bg2pc::write(0);
        reg::bg2pd::write(1);
        reg::bg2x::write(0);
        reg::bg2y::write(0);
    }

    wait_for_exit();
}

void display_mosiac_mode2_0() {
   mosaic_mode2(1, 1);
}

void display_mosiac_mode2_1_1() {
   mosaic_mode2(2, 2);
}

void display_mosiac_mode2_4_4() {
   mosaic_mode2(5, 5);
}

void display_mosiac_mode2_9_9() {
   mosaic_mode2(10, 10);
}

void display_mosiac_mode2_15_15() {
   mosaic_mode2(16, 16);
}

void display_mosiac_mode2_0_15() {
   mosaic_mode2(1, 16);
}

void display_mosiac_mode2_15_0() {
   mosaic_mode2(16, 1);
}

// repeat, but transformed
void display_mosiac_mode2_1_1_rotscale() {
   mosaic_mode2(2, 2, true);
}

void display_mosiac_mode2_4_4_rotscale() {
   mosaic_mode2(5, 5, true);
}

void display_mosiac_mode2_9_9_rotscale() {
   mosaic_mode2(10, 10, true);
}

void display_mosiac_mode2_15_15_rotscale() {
   mosaic_mode2(16, 16, true);
}

void display_mosiac_mode2_0_15_rotscale() {
   mosaic_mode2(1, 16, true);
}

void display_mosiac_mode2_15_0_rotscale() {
   mosaic_mode2(16, 1, true);
}

// yes bitmap mosaic?
static void mosaic_mode3(uint32_t size_x, uint32_t size_y) {
    reg::dispcnt::write({
        .mode = 3,
        .layer_background_2 = true,
    });
    
    reg::bg2cnt::write(background_control {
        .mosaic = true,
    });

    reg::mosaic::emplace(mosaic_size {
        .background = {size_x,  size_y}
    });

    reg::bg2pa::write(1);
    reg::bg2pb::write(0);
    reg::bg2pc::write(0);
    reg::bg2pd::write(1);
    reg::bg2x::write(0);
    reg::bg2y::write(0);

    for(int y = 0; y < 160; y++) {
        for(int x = 0; x < 240; x++) {
            video_ram[x + y * 240] = (x / 8) | (y / 5) << 5 | (0x1F - x / 8) << 10;
        }
    }

    wait_for_exit();
}

void display_mosiac_mode3_0() {
   mosaic_mode3(1, 1);
}

void display_mosiac_mode3_1_1() {
   mosaic_mode3(2, 2);
}

void display_mosiac_mode3_15_15() {
   mosaic_mode3(16, 16);
}

void display_mosiac_mode3_0_15() {
   mosaic_mode3(1, 16);
}

void display_mosiac_mode3_15_0() {
   mosaic_mode3(16, 1);
}

static void mosaic_mode4(uint32_t size_x, uint32_t size_y) {
    reg::dispcnt::write({
        .mode = 4,
        .layer_background_2 = true,
    });

    reg::bg2cnt::write(background_control {
        .mosaic = true,
    });

    reg::mosaic::emplace(mosaic_size {
        .background = {size_x,  size_y}
    });

    reg::bg2pa::write(1);
    reg::bg2pb::write(0);
    reg::bg2pc::write(0);
    reg::bg2pd::write(1);
    reg::bg2x::write(0);
    reg::bg2y::write(0);

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

void display_mosiac_mode4_0() {
   mosaic_mode4(1, 1);
}

void display_mosiac_mode4_1_1() {
   mosaic_mode4(2, 2);
}

void display_mosiac_mode4_15_15() {
   mosaic_mode4(16, 16);
}

void display_mosiac_mode4_0_15() {
   mosaic_mode4(1, 16);
}

void display_mosiac_mode4_15_0() {
   mosaic_mode4(16, 1);
}

static void mosaic_mode5(uint32_t size_x, uint32_t size_y) {
    reg::dispcnt::write({
        .mode = 5,
        .layer_background_2 = true,
    });

    reg::bg2cnt::write(background_control {
        .mosaic = true,
    });

    reg::mosaic::emplace(mosaic_size {
        .background = {size_x,  size_y}
    });

    reg::bg2pa::write(1);
    reg::bg2pb::write(0);
    reg::bg2pc::write(0);
    reg::bg2pd::write(1);
    reg::bg2x::write(0);
    reg::bg2y::write(0);

    for(int y = 0; y < 128; y++) {
        for(int x = 0; x < 160; x++) {
            video_ram[x + y * 160] = (x / 5) | (y / 4) << 5 | (0x1F - x / 5) << 10;
        }
    }

    wait_for_exit();
}

void display_mosiac_mode5_0() {
   mosaic_mode5(1, 1);
}

void display_mosiac_mode5_1_1() {
   mosaic_mode5(2, 2);
}

void display_mosiac_mode5_15_15() {
   mosaic_mode5(16, 16);
}

void display_mosiac_mode5_0_15() {
   mosaic_mode5(1, 16);
}

void display_mosiac_mode5_15_0() {
   mosaic_mode5(16, 1);
}

static const int oam_addr = 0x07000000;

static void set_object(int index, object::attr0 attr0, object::attr1_regular attr1, object::attr2 attr2) {
    object_regular object {attr0, attr1, attr2};
    agbabi::memcpy2(reinterpret_cast<void *>(oam_addr + index * sizeof(object)), &object, sizeof(object));
}

static void mosaic_objects(object::mode mode, uint32_t size_x, uint32_t size_y) {
    palette_ram[0] = 0x4210;

    // load sprites
    agbabi::memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    agbabi::memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

    reg::dispcnt::write({
        .mode = 0,
        .layer_object = true,
    });

    reg::mosaic::emplace(mosaic_size {
        .object = {size_x,  size_y}
    });

    // modified a bit from object tests setup_objects
    const int spacing = 9;
    uint16_t x = spacing, y = spacing;

    // size 0

    // 8x8
    set_object(0, object::attr0 {
        .y = y,
        .object_mode = mode,
        .mosaic = true,
        .shape = object::shape::square
    }, object::attr1_regular {
        .x = x,
        .size = 0
    }, object::attr2 {
        .tile_index = 0,
    });
    y += 8 + spacing;

    // 16x8
    set_object(1, object::attr0 {
        .y = y,
        .object_mode = mode,
        .mosaic = true,
        .shape = object::shape::wide
    }, object::attr1_regular {
        .x = x,
        .size = 0
    }, object::attr2 {
        .tile_index = 1,
    });

    y += 8 + spacing;

    // 8x16
    set_object(2, object::attr0 {
        .y = y,
        .object_mode = mode,
        .mosaic = true,
        .shape = object::shape::tall
    }, object::attr1_regular {
        .x = x,
        .size = 0
    }, object::attr2 {
        .tile_index = 32,
    });

    y = spacing;
    x += 16 + spacing;

    // size 1

    // 16x16
    set_object(3, object::attr0 {
        .y = y,
        .object_mode = mode,
        .mosaic = true,
        .shape = object::shape::square
    }, object::attr1_regular {
        .x = x,
        .size = 1
    }, object::attr2 {
        .tile_index = 3,
    });

    y += 16 + spacing;

    // 32x8
    set_object(4, object::attr0 {
        .y = y,
        .object_mode = mode,
        .mosaic = true,
        .shape = object::shape::wide
    }, object::attr1_regular {
        .x = x,
        .size = 1
    }, object::attr2 {
        .tile_index = 5,
    });

    y += 8 + spacing;

    // 8x32
    set_object(5, object::attr0 {
        .y = y,
        .object_mode = mode,
        .mosaic = true,
        .shape = object::shape::tall
    }, object::attr1_regular {
        .x = x,
        .size = 1
    }, object::attr2 {
        .tile_index = 9,
    });

    x += 32 + spacing;
    y = spacing;

    // size 2

    // 32x32
    set_object(6, object::attr0 {
        .y = y,
        .object_mode = mode,
        .mosaic = true,
        .shape = object::shape::square
    }, object::attr1_regular {
        .x = x,
        .size = 2
    }, object::attr2 {
        .tile_index = 10,
    });

    y += 32 + spacing;

    // 32x16
    set_object(7, object::attr0 {
        .y = y,
        .object_mode = mode,
        .mosaic = true,
        .shape = object::shape::wide
    }, object::attr1_regular {
        .x = x,
        .size = 2
    }, object::attr2 {
        .tile_index = 14,
    });

    y += 16 + spacing;

    // 16x32
    set_object(8, object::attr0 {
        .y = y,
        .object_mode = mode,
        .mosaic = true,
        .shape = object::shape::tall
    }, object::attr1_regular {
        .x = x,
        .size = 2
    }, object::attr2 {
        .tile_index = 18,
    });

    x += 32 + spacing;
    y = spacing;

    // size 3

    // 64x64
    set_object(9, object::attr0 {
        .y = y,
        .object_mode = mode,
        .mosaic = true,
        .shape = object::shape::square
    }, object::attr1_regular {
        .x = x,
        .size = 3
    }, object::attr2 {
        .tile_index = 20,
    });

    y += 64 + spacing;

    // 64x32
    set_object(10, object::attr0 {
        .y = y,
        .object_mode = mode,
        .mosaic = true,
        .shape = object::shape::wide
    }, object::attr1_regular {
        .x = x,
        .size = 3
    }, object::attr2 {
        .tile_index = 140,
    });

    // new column, no space left
    y = spacing;
    x += 64 + spacing;

    // 32x64
    set_object(11, object::attr0 {
        .y = y,
        .object_mode = mode,
        .mosaic = true,
        .shape = object::shape::tall
    }, object::attr1_regular {
        .x = x,
        .size = 3
    }, object::attr2 {
        .tile_index = 28,
    });

    // hide everything else off-screen
    for(int i = 12; i < 128; i++)
        set_object(i, {.y = 160}, object::attr1_regular {}, {});

    // just the one transform for now
    static const bios::obj_affine_input input {
        .scale_x = 1.1f,
        .scale_y = 1.1f,
        .rotation = 0x20
    };

    bios::obj_affine_set(&input, reinterpret_cast<object::mat2 *>(oam_addr + 6), 1, 8);

    wait_for_exit();
}

void display_mosiac_objects_regular_1_1() {
   mosaic_objects(object::mode::regular, 2, 2);
}

void display_mosiac_objects_regular_4_4() {
   mosaic_objects(object::mode::regular, 5, 5);
}

void display_mosiac_objects_regular_9_9() {
   mosaic_objects(object::mode::regular, 10, 10);
}

void display_mosiac_objects_regular_15_15() {
   mosaic_objects(object::mode::regular, 16, 16);
}

void display_mosiac_objects_regular_0_15() {
   mosaic_objects(object::mode::regular, 1, 16);
}

void display_mosiac_objects_regular_15_0() {
   mosaic_objects(object::mode::regular, 16, 1);
}

void display_mosiac_objects_affine_1_1() {
   mosaic_objects(object::mode::affine, 2, 2);
}

void display_mosiac_objects_affine_4_4() {
   mosaic_objects(object::mode::affine, 5, 5);
}

void display_mosiac_objects_affine_9_9() {
   mosaic_objects(object::mode::affine, 10, 10);
}

void display_mosiac_objects_affine_15_15() {
   mosaic_objects(object::mode::affine, 16, 16);
}

void display_mosiac_objects_affine_0_15() {
   mosaic_objects(object::mode::affine, 1, 16);
}

void display_mosiac_objects_affine_15_0() {
   mosaic_objects(object::mode::affine, 16, 1);
}