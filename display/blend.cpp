#include <gba/gba.hpp>

#include "../common.hpp"

#include "../assets/sprites.h"

using namespace gba;

// gba-hpp is 1 bit off
inline constexpr auto BLDALPHA = registral<const_ptr<volatile fixed<u8x2, 4>>(0x4000052)>{};


static void setup_layers() {
    mmio::BG0CNT = {
        .screenblock = 2,
    };

    mmio::BG1CNT = {
        .screenblock = 3,
    };

    mmio::BG2CNT = {
        .screenblock = 4,
    };

    mmio::BG3CNT = {
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
}

void display_blend_mode0() {
    // blend mode 0 == disabled
    mmio::DISPCNT = {
        .video_mode = 0,
        .show_bg0 = true,
        .show_bg1 = true,
        .show_bg2 = true,
        .show_bg3 = true,
    };

    mmio::BLDCNT = bldcnt {
        .target1_bg0 = true, // these don't matter...
        .target1_bg1 = true,
        .target1_bg2 = true,
        .target1_bg3 = true,
        .target1_obj = true,
        .target1_backdrop = true,
        .mode = color_effect::no_effect, // ... as blending is off ...
        .target2_bg0 = true, // ... nope
        .target2_bg1 = true,
        .target2_bg2 = true,
        .target2_bg3 = true,
        .target2_obj = true,
        .target2_backdrop = true
    };

    // pointless
    ::BLDALPHA = {1.0f, 1.0f};

    // no effect
    mmio::BLDY = 1;

    palette_ram[0] = 0x4210;

    setup_layers();

    wait_for_exit();
}

void display_blend_mode1() {
    // blend mode 1 == alpha blend
    mmio::DISPCNT = {
        .video_mode = 0,

        .show_bg1 = true,
        .show_bg2 = true,
    };

    mmio::BLDCNT = bldcnt {
        .target1_bg0 = true, // not enabled so ignored
        .target1_bg1 = true,
        .mode = color_effect::alpha_blend,
        .target2_bg2 = true,
    };

    // 50/50
    ::BLDALPHA = {0.5f, 0.5f};

    // no effect
    mmio::BLDY = 0.5f;

    palette_ram[0] = 0x4210;

    setup_layers();

    wait_for_exit();
}

void display_blend_mode1_all() {
    // blend mode 1 == alpha blend
    // all enabled - first two blend
    mmio::DISPCNT = {
        .video_mode = 0,
        .show_bg0 = true,
        .show_bg1 = true,
        .show_bg2 = true,
        .show_bg3 = true,
    };

    mmio::BLDCNT = bldcnt {
        .target1_bg0 = true,
        .target1_bg1 = true,
        .target1_bg2 = true,
        .target1_bg3 = true,
        .mode = color_effect::alpha_blend,
        .target2_bg0 = true,
        .target2_bg1 = true,
        .target2_bg2 = true,
        .target2_bg3 = true,
    };

    // 50/50
    ::BLDALPHA = {0.5f, 0.5f};

    // no effect
    mmio::BLDY = 0.5f;

    palette_ram[0] = 0x4210;

    setup_layers();

    wait_for_exit();
}

void display_blend_mode1_add() {
    // blend mode 1 == alpha blend
    // both factors set to 1 to add
    mmio::DISPCNT = {
        .video_mode = 0,

        .show_bg1 = true,
        .show_bg2 = true,
    };

    mmio::BLDCNT = bldcnt {
        .target1_bg0 = true, // not enabled so ignored
        .target1_bg1 = true,
        .mode = color_effect::alpha_blend,
        .target2_bg2 = true,
    };

    // 50/50
    ::BLDALPHA = {1.0f, 1.0f};

    // no effect
    mmio::BLDY = 0.5f;

    palette_ram[0] = 0x4210;

    setup_layers();

    wait_for_exit();
}

void display_blend_mode1_reversed() {
    // blend mode 1 == alpha blend
    // layers in wrong order
    mmio::DISPCNT = {
        .video_mode = 0,

        .show_bg1 = true,
        .show_bg2 = true,
    };

    mmio::BLDCNT = bldcnt {
        .target1_bg2 = true,
        .mode = color_effect::alpha_blend,
        .target2_bg1 = true,
    };

    // 50/50
    ::BLDALPHA = {0.5f, 0.5f};

    palette_ram[0] = 0x4210;

    setup_layers();

    wait_for_exit();
}

void display_blend_mode1_no_second() {
    // blend mode 1 == alpha blend
    // missing dest
    mmio::DISPCNT = {
        .video_mode = 0,

        .show_bg1 = true,
        .show_bg2 = true,
    };

    mmio::BLDCNT = bldcnt {
        .target1_bg1 = true,
        .mode = color_effect::alpha_blend,
    };

    // 50/50
    ::BLDALPHA = {0.5f, 0.5f};

    palette_ram[0] = 0x4210;

    setup_layers();

    wait_for_exit();
}

void display_blend_mode1_between() {
    // blend mode 1 == alpha blend
    // layer inbetween the two blended layers
    mmio::DISPCNT = {
        .video_mode = 0,

        .show_bg0 = true,
        .show_bg1 = true,
        .show_bg2 = true,
    };

    mmio::BLDCNT = bldcnt {
        .target1_bg0 = true,
        .mode = color_effect::alpha_blend,
        .target2_bg2 = true,
    };

    // 50/50
    ::BLDALPHA = {0.5f, 0.5f};

    palette_ram[0] = 0x4210;

    setup_layers();

    wait_for_exit();
}

void display_blend_mode1_objects() {
    // blend mode 1 == alpha blend
    // objects don't blend with each other

    // load sprites
    __agbabi_memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    __agbabi_memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

    mmio::DISPCNT = {
        .video_mode = 0,
        .show_bg1 = true,
        .show_obj = true,
    };

    mmio::BLDCNT = bldcnt {
        .target1_obj = true,
        .mode = color_effect::alpha_blend,

        .target2_bg1 = true,
        .target2_obj = true
    };

    // 50/50
    ::BLDALPHA = {0.5f, 0.5f};

    // sprites
    objattr obj {
        {.y = 32},
        {.x = 32, .size = 2},
        {.tile_id = 10}
    };

    __agbabi_memcpy2(reinterpret_cast<void*>(0x7000000), &obj, sizeof(obj));

    // same priority
    obj.y += 8;
    obj.x += 8;
    __agbabi_memcpy2(reinterpret_cast<void*>(0x7000008), &obj, sizeof(obj));

    // lower priority, but still above the background
    obj.y += 8;
    obj.x += 8;
    obj.priority = 1;
    __agbabi_memcpy2(reinterpret_cast<void*>(0x7000010), &obj, sizeof(obj));

    // behind the background
    obj.y += 8;
    obj.x += 8;
    obj.priority = 2;
    __agbabi_memcpy2(reinterpret_cast<void*>(0x7000018), &obj, sizeof(obj));

    // hide the rest
    obj.y = 160;
    for(int i = 4; i < 128; i++)
        __agbabi_memcpy2(reinterpret_cast<void*>(0x7000000 + i * 8), &obj, sizeof(obj));

    palette_ram[0] = 0x4210;

    setup_layers();

    // adjust layer priority
    mmio::BG1CNT = {
        .priority = 1,
        .screenblock = 3
    };

    wait_for_exit();
}

void display_blend_mode2() {
    // blend mode 2 == brighter
    mmio::DISPCNT = {
        .video_mode = 0,

        .show_bg1 = true,
        .show_bg2 = true,
    };

    mmio::BLDCNT = bldcnt {
        .target1_bg0 = true,
        .target1_bg1 = true,
        .mode = color_effect::brighten,
        .target2_bg0 = true, // ... unused for mode 2
        .target2_bg1 = true,
        .target2_backdrop = true
    };

    // pointless
    ::BLDALPHA = {1.0f, 1.0f};

    mmio::BLDY = 0.5f;

    palette_ram[0] = 0x4210;

    setup_layers();

    wait_for_exit();
}

void display_blend_mode2_backdrop() {
    // blend mode 2 == brighter
    // just the backdrop
    mmio::DISPCNT = {
        .video_mode = 0,
    };

    mmio::BLDCNT = bldcnt {
        .target1_backdrop = true,
        .mode = color_effect::brighten,
    };

    // pointless
    ::BLDALPHA = {1.0f, 1.0f};

    mmio::BLDY = 0.5f;

    palette_ram[0] = 0x4210;

    setup_layers();

    wait_for_exit();
}

void display_blend_mode3() {
    // blend mode 3 == darker
    mmio::DISPCNT = {
        .video_mode = 0,
        .show_bg1 = true,
        .show_bg2 = true,
    };

    mmio::BLDCNT = bldcnt {
        .target1_bg0 = true,
        .target1_bg1 = true,
        .mode = color_effect::darken,
        .target2_bg0 = true, // ... unused for mode 3
        .target2_bg1 = true,
        .target2_bg2 = true,
    };

    // pointless
    ::BLDALPHA = {1.0f, 1.0f};

    mmio::BLDY = 0.5f;

    palette_ram[0] = 0x4210;

    setup_layers();

    wait_for_exit();
}

void display_blend_mode3_backdrop() {
    // blend mode 3 == darker
    // just the backdrop
    mmio::DISPCNT = {
        .video_mode = 0,
    };

    mmio::BLDCNT = bldcnt {
        .target1_backdrop = true,
        .mode = color_effect::darken,
    };

    // pointless
    ::BLDALPHA = {1.0f, 1.0f};

    mmio::BLDY = 0.25f;

    palette_ram[0] = 0x4210;

    setup_layers();

    wait_for_exit();
}

void display_blend_window() {
    // load sprites, but don't bother with the palette
    __agbabi_memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));

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

    mmio::BLDCNT = bldcnt {
        .target1_bg0 = true,
        .target1_bg1 = true,
        .target1_bg2 = true,
        .target1_bg3 = true,
        .mode = color_effect::alpha_blend,

        .target2_bg1 = true,
        .target2_bg2 = true,
        .target2_bg3 = true,
        .target2_backdrop = true,
    };

    // 50/50
    ::BLDALPHA = {0.5f, 0.5f};

    mmio::WIN0H = u8x2{140 - 1, 10};
    mmio::WIN0V = u8x2{100 - 1, 10};

    mmio::WIN1H = u8x2{240 - 20 - 1, 15};
    mmio::WIN1V = u8x2{160 - 20 - 1, 30};

    mmio::WININ = {
        .win0_bg0 = true,
        .win0_bg3 = true,
        .win0_effect = true,

        .win1_bg2 = true,
    };

    mmio::WINOUT = {
        .outside_bg0 = true,

        // this is the obj window
        .obj_win_bg1 = true,
        .obj_win_effect = true
    };

    palette_ram[0] = 0x4210;

    objattr obj {
        {.y = 5, .mode = obj_effect::window},
        {.x = 100, .size = 3},
        {.tile_id = 20}
    };

    __agbabi_memcpy2(reinterpret_cast<void*>(0x7000000), &obj, sizeof(obj));

    setup_layers();

    wait_for_exit();
}

void display_blend_object_trans() {
    // semi-transparent object overrides blend mode

    // load sprites
    __agbabi_memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    __agbabi_memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

    mmio::DISPCNT = {
        .video_mode = 0,

        .show_bg1 = true,
        .show_obj = true,
    };

    mmio::BLDCNT = bldcnt {
        .target1_bg1 = true,
        .target1_obj = true,
        .mode = color_effect::darken,
        .target2_bg1 = true, // used for sprite transparency
    };

    ::BLDALPHA = {0.5f, 0.5f};

    mmio::BLDY = 0.75f;

    // sprites
    objattr obj {
        {.y = 32},
        {.x = 32, .size = 2},
        {.tile_id = 10}
    };

    __agbabi_memcpy2(reinterpret_cast<void*>(0x7000000), &obj, sizeof(obj));

    // blended
    obj.mode = obj_effect::semi_transparent;
    obj.x += 40;
    __agbabi_memcpy2(reinterpret_cast<void*>(0x7000008), &obj, sizeof(obj));

    // hide the rest
    obj.y = 160;
    for(int i = 2; i < 128; i++)
        __agbabi_memcpy2(reinterpret_cast<void*>(0x7000000 + i * 8), &obj, sizeof(obj));

    palette_ram[0] = 0x4210;

    setup_layers();

    wait_for_exit();
}

void display_blend_object_trans_enable() {
    // semi-transparent object overrides blend mode, enabling it

    // load sprites
    __agbabi_memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    __agbabi_memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

    mmio::DISPCNT = {
        .video_mode = 0,

        .show_bg1 = true,
        .show_obj = true,
    };

    mmio::BLDCNT = bldcnt {
        .target1_bg1 = true,
        .target1_obj = true,
        .mode = color_effect::no_effect,
        .target2_bg1 = true, // used for sprite transparency
    };

    ::BLDALPHA = {0.5f, 0.5f};

    mmio::BLDY = 0.75f;

    // sprites
    objattr obj {
        {.y = 32},
        {.x = 32, .size = 2},
        {.tile_id = 10}
    };

    __agbabi_memcpy2(reinterpret_cast<void*>(0x7000000), &obj, sizeof(obj));

    // blended
    obj.mode = obj_effect::semi_transparent;
    obj.x += 40;
    __agbabi_memcpy2(reinterpret_cast<void*>(0x7000008), &obj, sizeof(obj));

    // hide the rest
    obj.y = 160;
    for(int i = 2; i < 128; i++)
        __agbabi_memcpy2(reinterpret_cast<void*>(0x7000000 + i * 8), &obj, sizeof(obj));

    palette_ram[0] = 0x4210;

    setup_layers();

    wait_for_exit();
}

void display_blend_object_trans_priority() {
    // load sprites
    __agbabi_memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    __agbabi_memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

    mmio::DISPCNT = {
        .video_mode = 0,

        .show_bg1 = true,
        .show_obj = true,
    };

    mmio::BLDCNT = bldcnt {
        .target1_bg1 = true,
        .target1_obj = true,
        .mode = color_effect::darken,
        .target2_bg1 = true, // used for sprite transparency
    };

    ::BLDALPHA = {0.5f, 0.5f};

    mmio::BLDY = 0.75f;

    // sprites
    objattr obj {
        {.y = 32},
        {.x = 32, .size = 2},
        {.tile_id = 10}
    };

    __agbabi_memcpy2(reinterpret_cast<void*>(0x7000000), &obj, sizeof(obj));


    // blended (partially covered)
    obj.mode = obj_effect::semi_transparent;
    obj.x += 16;
    __agbabi_memcpy2(reinterpret_cast<void*>(0x7000008), &obj, sizeof(obj));

    // and again with different priorities
    obj.y += 40;
    obj.priority = 1;

    __agbabi_memcpy2(reinterpret_cast<void*>(0x7000018), &obj, sizeof(obj));

    obj.mode = obj_effect::normal;
    obj.x -= 16;
    obj.priority = 0;

    __agbabi_memcpy2(reinterpret_cast<void*>(0x7000010), &obj, sizeof(obj));

    // hide the rest
    obj.y = 160;
    for(int i = 4; i < 128; i++)
        __agbabi_memcpy2(reinterpret_cast<void*>(0x7000000 + i * 8), &obj, sizeof(obj));

    palette_ram[0] = 0x4210;

    setup_layers();

    // adjust layer priority
    mmio::BG1CNT = {
        .priority = 1,
        .screenblock = 3
    };

    wait_for_exit();
}
