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

void display_blend_mode0() {
    // blend mode 0 == disabled
    reg::dispcnt::write({
        .mode = 0,
        .layer_background_0 = true,
        .layer_background_1 = true,
        .layer_background_2 = true,
        .layer_background_3 = true,
    });

    reg::bldcnt::write(blend_control {
        .src_bg0 = true, // these don't matter...
        .src_bg1 = true,
        .src_bg2 = true,
        .src_bg3 = true,
        .src_obj = true,
        .src_backdrop = true,
        .mode = blend_mode::off, // ... as blending is off ...
        .dst_bg0 = true, // ... nope
        .dst_bg1 = true,
        .dst_bg2 = true,
        .dst_bg3 = true,
        .dst_obj = true,
        .dst_backdrop = true
    });

    // pointless
    reg::bldalpha::write({
        .eva = 1,
        .evb = 1,
    });

    // no effect
    reg::bldy::write(1);

    palette_ram[0] = 0x4210;

    setup_layers();

    wait_for_exit();
}

void display_blend_mode1() {
    // blend mode 1 == alpha blend
    reg::dispcnt::write({
        .mode = 0,
        
        .layer_background_1 = true,
        .layer_background_2 = true,
    });

    reg::bldcnt::write(blend_control {
        .src_bg0 = true, // not enabled so ignored
        .src_bg1 = true,
        .mode = blend_mode::alpha,
        .dst_bg2 = true,
    });

    // 50/50
    reg::bldalpha::write({
        .eva = 0.5f,
        .evb = 0.5f,
    });

    // no effect
    reg::bldy::write(0.5f);

    palette_ram[0] = 0x4210;

    setup_layers();

    wait_for_exit();
}

void display_blend_mode1_all() {
    // blend mode 1 == alpha blend
    // all enabled - first two blend
    reg::dispcnt::write({
        .mode = 0,
        .layer_background_0 = true,
        .layer_background_1 = true,
        .layer_background_2 = true,
        .layer_background_3 = true,
    });

    reg::bldcnt::write(blend_control {
        .src_bg0 = true,
        .src_bg1 = true,
        .src_bg2 = true,
        .src_bg3 = true,
        .mode = blend_mode::alpha,
        .dst_bg0 = true,
        .dst_bg1 = true,
        .dst_bg2 = true,
        .dst_bg3 = true,
    });

    // 50/50
    reg::bldalpha::write({
        .eva = 0.5f,
        .evb = 0.5f,
    });

    // no effect
    reg::bldy::write(0.5f);

    palette_ram[0] = 0x4210;

    setup_layers();

    wait_for_exit();
}

void display_blend_mode1_add() {
    // blend mode 1 == alpha blend
    // both factors set to 1 to add
    reg::dispcnt::write({
        .mode = 0,
        
        .layer_background_1 = true,
        .layer_background_2 = true,
    });

    reg::bldcnt::write(blend_control {
        .src_bg0 = true, // not enabled so ignored
        .src_bg1 = true,
        .mode = blend_mode::alpha,
        .dst_bg2 = true,
    });

    // 50/50
    reg::bldalpha::write({
        .eva = 1,
        .evb = 1,
    });

    // no effect
    reg::bldy::write(0.5f);

    palette_ram[0] = 0x4210;

    setup_layers();

    wait_for_exit();
}

void display_blend_mode1_reversed() {
    // blend mode 1 == alpha blend
    // layers in wrong order
    reg::dispcnt::write({
        .mode = 0,
        
        .layer_background_1 = true,
        .layer_background_2 = true,
    });

    reg::bldcnt::write(blend_control {
        .src_bg2 = true,
        .mode = blend_mode::alpha,
        .dst_bg1 = true,
    });

    // 50/50
    reg::bldalpha::write({
        .eva = 0.5f,
        .evb = 0.5f,
    });

    palette_ram[0] = 0x4210;

    setup_layers();

    wait_for_exit();
}

void display_blend_mode1_no_second() {
    // blend mode 1 == alpha blend
    // missing dest
    reg::dispcnt::write({
        .mode = 0,
        
        .layer_background_1 = true,
        .layer_background_2 = true,
    });

    reg::bldcnt::write(blend_control {
        .src_bg1 = true,
        .mode = blend_mode::alpha,
    });

    // 50/50
    reg::bldalpha::write({
        .eva = 0.5f,
        .evb = 0.5f,
    });

    palette_ram[0] = 0x4210;

    setup_layers();

    wait_for_exit();
}

void display_blend_mode1_between() {
    // blend mode 1 == alpha blend
    // layer inbetween the two blended layers
    reg::dispcnt::write({
        .mode = 0,
        
        .layer_background_0 = true,
        .layer_background_1 = true,
        .layer_background_2 = true,
    });

    reg::bldcnt::write(blend_control {
        .src_bg0 = true,
        .mode = blend_mode::alpha,
        .dst_bg2 = true,
    });

    // 50/50
    reg::bldalpha::write({
        .eva = 0.5f,
        .evb = 0.5f,
    });

    palette_ram[0] = 0x4210;

    setup_layers();

    wait_for_exit();
}

void display_blend_mode1_objects() {
    // blend mode 1 == alpha blend
    // objects don't blend with each other

    // load sprites
    agbabi::memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    agbabi::memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

    reg::dispcnt::write({
        .mode = 0,
        .layer_background_1 = true,
        .layer_object = true,
    });

    reg::bldcnt::write(blend_control {
        .src_obj = true,
        .mode = blend_mode::alpha,
        
        .dst_bg1 = true,
        .dst_obj = true
    });

    // 50/50
    reg::bldalpha::write({
        .eva = 0.5f,
        .evb = 0.5f,
    });

    // sprites
    object_regular obj {
        {.y = 32},
        {.x = 32, .size = 2},
        {.tile_index = 10}
    };

    agbabi::memcpy2(reinterpret_cast<void*>(0x7000000), &obj, sizeof(obj));

    // same priority
    obj.attr0.y += 8;
    obj.attr1.x += 8;
    agbabi::memcpy2(reinterpret_cast<void*>(0x7000008), &obj, sizeof(obj));

    // lower priority, but still above the background
    obj.attr0.y += 8;
    obj.attr1.x += 8;
    obj.attr2.priority = 1;
    agbabi::memcpy2(reinterpret_cast<void*>(0x7000010), &obj, sizeof(obj));

    // behind the background
    obj.attr0.y += 8;
    obj.attr1.x += 8;
    obj.attr2.priority = 2;
    agbabi::memcpy2(reinterpret_cast<void*>(0x7000018), &obj, sizeof(obj));

    // hide the rest
    obj.attr0.y = 160;
    for(int i = 4; i < 128; i++)
        agbabi::memcpy2(reinterpret_cast<void*>(0x7000000 + i * 8), &obj, sizeof(obj));

    palette_ram[0] = 0x4210;

    setup_layers();

    // adjust layer priority
    reg::bg1cnt::write(background_control {
        .priority = 1,
        .screen_base_block = 3
    });

    wait_for_exit();
}

void display_blend_mode2() {
    // blend mode 2 == brighter
    reg::dispcnt::write({
        .mode = 0,
        
        .layer_background_1 = true,
        .layer_background_2 = true,
    });

    reg::bldcnt::write(blend_control {
        .src_bg0 = true,
        .src_bg1 = true,
        .mode = blend_mode::white,
        .dst_bg0 = true, // ... unused for mode 2
        .dst_bg1 = true,
        .dst_backdrop = true
    });

    // pointless
    reg::bldalpha::write({
        .eva = 1,
        .evb = 1,
    });

    reg::bldy::write(0.5f);

    palette_ram[0] = 0x4210;

    setup_layers();

    wait_for_exit();
}

void display_blend_mode2_backdrop() {
    // blend mode 2 == brighter
    // just the backdrop
    reg::dispcnt::write({
        .mode = 0,
    });

    reg::bldcnt::write(blend_control {
        .src_backdrop = true,
        .mode = blend_mode::white,
    });

    // pointless
    reg::bldalpha::write({
        .eva = 1,
        .evb = 1,
    });

    reg::bldy::write(0.5f);

    palette_ram[0] = 0x4210;

    setup_layers();

    wait_for_exit();
}

void display_blend_mode3() {
    // blend mode 3 == darker
    reg::dispcnt::write({
        .mode = 0,
        
        .layer_background_1 = true,
        .layer_background_2 = true,
    });

    reg::bldcnt::write(blend_control {
        .src_bg0 = true,
        .src_bg1 = true,
        .mode = blend_mode::black,
        .dst_bg0 = true, // ... unused for mode 3
        .dst_bg1 = true,
        .dst_bg2 = true,
    });

    // pointless
    reg::bldalpha::write({
        .eva = 1,
        .evb = 1,
    });

    reg::bldy::write(0.5f);

    palette_ram[0] = 0x4210;

    setup_layers();

    wait_for_exit();
}

void display_blend_mode3_backdrop() {
    // blend mode 3 == darker
    // just the backdrop
    reg::dispcnt::write({
        .mode = 0,
    });

    reg::bldcnt::write(blend_control {
        .src_backdrop = true,
        .mode = blend_mode::black,
    });

    // pointless
    reg::bldalpha::write({
        .eva = 1,
        .evb = 1,
    });

    reg::bldy::write(0.25f);

    palette_ram[0] = 0x4210;

    setup_layers();

    wait_for_exit();
}

void display_blend_window() {
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

    reg::bldcnt::write(blend_control {
        .src_bg0 = true,
        .src_bg1 = true,
        .src_bg2 = true,
        .src_bg3 = true,
        .mode = blend_mode::alpha,
        
        .dst_bg1 = true,
        .dst_bg2 = true,
        .dst_bg3 = true,
        .dst_backdrop = true,
    });

    // 50/50
    reg::bldalpha::write({
        .eva = 0.5f,
        .evb = 0.5f,
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
        .win0_bg0 = true,
        .win0_bg3 = true,
        .win0_blend = true,
        .win1_bg2 = true,
    });

    reg::winout::write({
        .win0_bg0 = true,

        // this is the obj window
        .win1_bg1 = true,
        .win1_blend = true
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

void display_blend_object_trans() {
    // semi-transparent object overrides blend mode

    // load sprites
    agbabi::memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    agbabi::memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

    reg::dispcnt::write({
        .mode = 0,
        
        .layer_background_1 = true,
        .layer_object = true,
    });

    reg::bldcnt::write(blend_control {
        .src_bg1 = true,
        .src_obj = true,
        .mode = blend_mode::black,
        .dst_bg1 = true, // used for sprite transparency
    });

    reg::bldalpha::write({
        .eva = 0.5f,
        .evb = 0.5f,
    });

    reg::bldy::write(0.75f);

    // sprites
    object_regular obj {
        {.y = 32},
        {.x = 32, .size = 2},
        {.tile_index = 10}
    };

    agbabi::memcpy2(reinterpret_cast<void*>(0x7000000), &obj, sizeof(obj));

    // blended
    obj.attr0.gfx_mode = object::gfx_mode::blending;
    obj.attr1.x += 40;
    agbabi::memcpy2(reinterpret_cast<void*>(0x7000008), &obj, sizeof(obj));

    // hide the rest
    obj.attr0.y = 160;
    for(int i = 2; i < 128; i++)
        agbabi::memcpy2(reinterpret_cast<void*>(0x7000000 + i * 8), &obj, sizeof(obj));

    palette_ram[0] = 0x4210;

    setup_layers();

    wait_for_exit();
}