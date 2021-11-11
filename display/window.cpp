#include <gba/gba.hpp>

#include "../common.hpp"

#include "../assets/sprites.h"

using namespace gba;

static void setup_layers() {
    reg::bg0cnt::write(background_control {
        .screen_base_block = 2,
    });

    reg::bg1cnt::write(background_control {
        .screen_base_block = 3,
    });

    reg::bg2cnt::write(background_control {
        .screen_base_block = 4,
    });

    reg::bg3cnt::write(background_control {
        .screen_base_block = 5,
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

    reg::bg0hofs::write(0);
    reg::bg0vofs::write(0);
    reg::bg1hofs::write(0);
    reg::bg1vofs::write(0);
    reg::bg2hofs::write(0);
    reg::bg2vofs::write(0);
    reg::bg3hofs::write(0);
    reg::bg3vofs::write(0);
}

void display_window_win0_bg() {
    reg::dispcnt::write({
        .mode = 0,
        .layer_background_0 = true,
        .layer_background_2 = true,
        .window_0 = true
    });

    reg::win0h::write({
        .end = 240 - 20,
        .begin = 10,
    });

    reg::win0v::write({
        .end = 160 - 20,
        .begin = 10,
    });

    reg::winin::write({
        .win0_bg1 = true, // this layer isn't enabled
        .win0_bg2 = true
    });

    reg::winout::write({
        .win0_bg3 = true // neither is this one, so outside should be the backdrop
    });

    palette_ram[0] = 0x4210;

    setup_layers();

    wait_for_exit();
}

void display_window_win1_bg() {
    // same thing, but win1
    reg::dispcnt::write({
        .mode = 0,
        .layer_background_0 = true,
        .layer_background_2 = true,
        .window_1 = true
    });

    reg::win1h::write({
        .end = 240 - 20,
        .begin = 10,
    });

    reg::win1v::write({
        .end = 160 - 20,
        .begin = 10,
    });

    reg::winin::write({
        .win1_bg1 = true,
        .win1_bg2 = true
    });

    reg::winout::write({
        .win0_bg3 = true
    });

    palette_ram[0] = 0x4210;

    setup_layers();

    wait_for_exit();
}

void display_window_obj_win_bg() {
    // load sprites, but don't bother with the palette
    agbabi::memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));

    // same thing, but object window
    reg::dispcnt::write({
        .mode = 0,
        .layer_background_0 = true,
        .layer_background_2 = true,
        .layer_object = true,
        .window_object = true
    });

    reg::winout::write({
        .win0_bg3 = true,

        // this is the obj window
        .win1_bg1 = true,
        .win1_bg2 = true
    });

    palette_ram[0] = 0x4210;

    object_regular obj {
        {.y = 5, .gfx_mode = object::gfx_mode::windowed},
        {.x = 100, .size = 3},
        {.tile_index = 20}
    };

    agbabi::memcpy2(reinterpret_cast<void*>(0x7000000), &obj, sizeof(obj));

    setup_layers();

    wait_for_exit();
}

void display_window_priority() {
    // load sprites, but don't bother with the palette
    agbabi::memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));

    // enable ALL the windows
    reg::dispcnt::write({
        .mode = 0,
        .layer_background_0 = true,
        .layer_background_1 = true,
        .layer_background_2 = true,
        .layer_background_3 = true,
        .layer_object = true,
        .window_0 = true,
        .window_1 = true,
        .window_object = true
    });

    reg::win0h::write({
        .end = 140,
        .begin = 10,
    });

    reg::win0v::write({
        .end = 100,
        .begin = 10,
    });

    reg::win1h::write({
        .end = 240 - 20,
        .begin = 15,
    });

    reg::win1v::write({
        .end = 160 - 20,
        .begin = 30,
    });

    reg::winin::write({
        .win0_bg3 = true,
        .win1_bg2 = true,
    });

    reg::winout::write({
        .win0_bg0 = true,

        // this is the obj window
        .win1_bg1 = true,
        .win1_obj = true // no objects here except the one making the window, so this does nothing
    });

    palette_ram[0] = 0x4210;

    object_regular obj {
        {.y = 5, .gfx_mode = object::gfx_mode::windowed},
        {.x = 100, .size = 3},
        {.tile_index = 20}
    };

    agbabi::memcpy2(reinterpret_cast<void*>(0x7000000), &obj, sizeof(obj));

    setup_layers();

    wait_for_exit();
}

void display_window_objects() {
    // load sprites
    agbabi::memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    agbabi::memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

    // enable ALL the windows
    reg::dispcnt::write({
        .mode = 0,
        .layer_background_0 = true,
        .layer_background_1 = true,
        .layer_background_2 = true,
        .layer_background_3 = true,
        .layer_object = true,
        .window_0 = true,
        .window_1 = true,
        .window_object = true
    });

    reg::win0h::write({
        .end = 140,
        .begin = 10,
    });

    reg::win0v::write({
        .end = 100,
        .begin = 10,
    });

    reg::win1h::write({
        .end = 240 - 20,
        .begin = 15,
    });

    reg::win1v::write({
        .end = 160 - 20,
        .begin = 30,
    });

    reg::winin::write({
        .win0_bg3 = true,
        .win0_obj = true,

        .win1_bg2 = true,
    });

    reg::winout::write({
        .win0_bg0 = true,

        // this is the obj window
        .win1_bg1 = true,
        .win1_obj = true
    });

    palette_ram[0] = 0x4210;

    // setup sprite for window
    object_regular obj {
        {.y = 5, .gfx_mode = object::gfx_mode::windowed},
        {.x = 100, .size = 3},
        {.tile_index = 20}
    };

    agbabi::memcpy2(reinterpret_cast<void*>(0x7000000), &obj, sizeof(obj));

    // ... and some regular sprites
    obj.attr0.gfx_mode = object::gfx_mode::normal;
    obj.attr0.y += 10;
    obj.attr1.x -= 10;

    agbabi::memcpy2(reinterpret_cast<void*>(0x7000008), &obj, sizeof(obj));

    obj.attr0.y = 50;
    obj.attr1.x = 0;
    agbabi::memcpy2(reinterpret_cast<void*>(0x7000010), &obj, sizeof(obj));

    setup_layers();

    wait_for_exit();
}

// from layer2.cpp
void gen_affine_tiles(uint16_t *char_base, uint16_t *screen_base, int screen_size_tiles);

// affine verion
void display_window_win0_bg_affine() {
    reg::dispcnt::write({
        .mode = 1,
        .layer_background_0 = true,
        .layer_background_2 = true,
        .window_0 = true
    });

    reg::win0h::write({
        .end = 240 - 20,
        .begin = 10,
    });

    reg::win0v::write({
        .end = 160 - 20,
        .begin = 10,
    });

    reg::winin::write({
        .win0_bg1 = true, // this layer isn't enabled
        .win0_bg2 = true
    });

    reg::winout::write({
        .win0_bg0 = true
    });

    palette_ram[0] = 0x4210;

    setup_layers();

    // transform affine bg
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

    // gen affine tiles
    gen_affine_tiles(video_ram, video_ram + 0x1000, 16);

    wait_for_exit();
}

void display_window_invalid_x1() {
    reg::dispcnt::write({
        .mode = 0,
        .layer_background_0 = true,
        .layer_background_1 = true,
        .layer_background_2 = true,
        .layer_background_3 = true,
        .layer_object = true,
        .window_0 = true,
        .window_1 = true,
    });

    reg::win0h::write({
        .end = 140,
        .begin = 150, // start > end
    });

    reg::win0v::write({
        .end = 100,
        .begin = 10,
    });

    reg::win1h::write({
        .end = 220,
        .begin = 222, // start > end
    });

    reg::win1v::write({
        .end = 160 - 20,
        .begin = 30,
    });

    reg::winin::write({
        .win0_bg3 = true,
        .win1_bg2 = true,
    });

    reg::winout::write({
        .win0_bg0 = true,
    });

    palette_ram[0] = 0x4210;

    setup_layers();

    wait_for_exit();
}

void display_window_invalid_x2() {
    reg::dispcnt::write({
        .mode = 0,
        .layer_background_0 = true,
        .layer_background_1 = true,
        .layer_background_2 = true,
        .layer_background_3 = true,
        .layer_object = true,
        .window_0 = true,
        .window_1 = true,
    });

    reg::win0h::write({
        .end = 245, // > 240
        .begin = 10,
    });

    reg::win0v::write({
        .end = 100,
        .begin = 10,
    });

    reg::win1h::write({
        .end = 250, // > 240
        .begin = 15,
    });

    reg::win1v::write({
        .end = 160 - 20,
        .begin = 30,
    });

    reg::winin::write({
        .win0_bg3 = true,
        .win1_bg2 = true,
    });

    reg::winout::write({
        .win0_bg0 = true,
    });

    palette_ram[0] = 0x4210;

    setup_layers();

    wait_for_exit();
}

void display_window_invalid_y1() {
    reg::dispcnt::write({
        .mode = 0,
        .layer_background_0 = true,
        .layer_background_1 = true,
        .layer_background_2 = true,
        .layer_background_3 = true,
        .layer_object = true,
        .window_0 = true,
        .window_1 = true,
    });

    reg::win0h::write({
        .end = 140,
        .begin = 10, 
    });

    reg::win0v::write({
        .end = 100,
        .begin = 120, // start > end
    });

    reg::win1h::write({
        .end = 220,
        .begin = 15,
    });

    reg::win1v::write({
        .end = 120,
        .begin = 150,  // start > end
    });

    reg::winin::write({
        .win0_bg3 = true,
        .win1_bg2 = true,
    });

    reg::winout::write({
        .win0_bg0 = true,
    });

    palette_ram[0] = 0x4210;

    setup_layers();

    wait_for_exit();
}

void display_window_invalid_y2() {
    reg::dispcnt::write({
        .mode = 0,
        .layer_background_0 = true,
        .layer_background_1 = true,
        .layer_background_2 = true,
        .layer_background_3 = true,
        .layer_object = true,
        .window_0 = true,
        .window_1 = true,
    });

    reg::win0h::write({
        .end = 140,
        .begin = 10,
    });

    reg::win0v::write({
        .end = 180, // > 160
        .begin = 10,
    });

    reg::win1h::write({
        .end = 220,
        .begin = 15,
    });

    reg::win1v::write({
        .end = 220,
        .begin = 60,
    });

    reg::winin::write({
        .win0_bg3 = true,
        .win1_bg2 = true,
    });

    reg::winout::write({
        .win0_bg0 = true,
    });

    palette_ram[0] = 0x4210;

    setup_layers();

    wait_for_exit();
}