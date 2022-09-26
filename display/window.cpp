#include <gba/gba.hpp>

#include "../common.hpp"

#include "../assets/sprites.h"

using namespace gba;

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
    mmio::BG0HOFS = 0;
    mmio::BG1HOFS = 0;
    mmio::BG1HOFS = 0;
    mmio::BG2HOFS = 0;
    mmio::BG2HOFS = 0;
    mmio::BG3HOFS = 0;
    mmio::BG3HOFS = 0;
}

void display_window_win0_bg() {
    mmio::DISPCNT = {
        .video_mode = 0,
        .show_bg0 = true,
        .show_bg2 = true,
        .enable_win0 = true
    };

    mmio::WIN0H = u8x2{240 - 20 - 1, 10};

    mmio::WIN0V = u8x2{160 - 20 - 1, 10};

    mmio::WININ = {
        .win0_bg1 = true, // this layer isn't enabled
        .win0_bg2 = true
    };

    mmio::WINOUT = {
        .outside_bg3 = true // neither is this one, so outside should be the backdrop
    };

    palette_ram[0] = 0x4210;

    setup_layers();

    wait_for_exit();
}

void display_window_win1_bg() {
    // same thing, but win1
    mmio::DISPCNT = {
        .video_mode = 0,
        .show_bg0 = true,
        .show_bg2 = true,
        .enable_win1 = true
    };

    mmio::WIN1H = u8x2{240 - 20 - 1, 10};

    mmio::WIN1V = u8x2{160 - 20 - 1, 10};

    mmio::WININ = {
        .win1_bg1 = true,
        .win1_bg2 = true
    };

    mmio::WINOUT = {
        .outside_bg3 = true
    };

    palette_ram[0] = 0x4210;

    setup_layers();

    wait_for_exit();
}

void display_window_obj_win_bg() {
    // load sprites, but don't bother with the palette
    __agbabi_memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));

    // same thing, but object window
    mmio::DISPCNT = {
        .video_mode = 0,
        .show_bg0 = true,
        .show_bg2 = true,
        .show_obj = true,
        .enable_obj_win = true
    };

    mmio::WINOUT = {
        .outside_bg3 = true,

        .obj_win_bg1 = true,
        .obj_win_bg2 = true
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

void display_window_priority() {
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
        .obj_win_obj = true // no objects here except the one making the window, so this does nothing
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

void display_window_objects() {
    // load sprites
    __agbabi_memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    __agbabi_memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

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
        .win0_obj = true,

        .win1_bg2 = true,
    };

    mmio::WINOUT = {
        .outside_bg0 = true,

        // this is the obj window
        .obj_win_bg1 = true,
        .obj_win_obj = true
    };


    palette_ram[0] = 0x4210;

    // setup sprite for window
    objattr obj {
        {.y = 5, .mode = obj_effect::window},
        {.x = 100, .size = 3},
        {.tile_id = 20}
    };

    __agbabi_memcpy2(reinterpret_cast<void*>(0x7000000), &obj, sizeof(obj));

    // ... and some regular sprites
    obj.mode = obj_effect::normal;
    obj.y += 10;
    obj.x -= 10;

    __agbabi_memcpy2(reinterpret_cast<void*>(0x7000008), &obj, sizeof(obj));

    obj.y = 50;
    obj.x = 0;
    __agbabi_memcpy2(reinterpret_cast<void*>(0x7000010), &obj, sizeof(obj));

    setup_layers();

    wait_for_exit();
}

static const int bg2pa_addr = 0x4000020;

// affine verion
void display_window_win0_bg_affine() {
    mmio::DISPCNT = {
        .video_mode = 1,
        .show_bg0 = true,
        .show_bg2 = true,
        .enable_win0 = true
    };

    mmio::WIN0H = u8x2{240 - 20 - 1, 10};

    mmio::WIN0V = u8x2{160 - 20 - 1, 10};

    mmio::WININ = {
        .win0_bg1 = true, // this layer isn't enabled
        .win0_bg2 = true
    };

    mmio::WINOUT = {
        .outside_bg0 = true,
    };

    palette_ram[0] = 0x4210;

    setup_layers();

    // transform affine bg
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

    // gen affine tiles
    gen_affine_tiles(video_ram, video_ram + 0x1000, 16);

    wait_for_exit();
}

static void window_hblank_irq_handler(int mask) {
    auto vcount = *mmio::VCOUNT + 1;

    if(vcount > 160)
        return;

    if(vcount == 160)
        vcount = 0;

    if(vcount < 80) {
        mmio::WIN0H = u8x2{uint8_t(120 + vcount), uint8_t(120 - vcount)};
    } else {
        mmio::WIN0H = u8x2{uint8_t(278 - vcount), uint8_t(vcount - 40)};
    }
}

void display_window_hblank_irq() {
    // changing window x start/end in a hblank irq
    mmio::DISPCNT = {
        .video_mode = 0,
        .show_bg0 = true,
        .show_bg1 = true,
        .show_bg2 = true,
        .show_bg3 = true,
        .show_obj = true,
        .enable_win0 = true,
    };

    mmio::WIN0H = u8x2{120 - 1, 120};

    mmio::WIN0V = u8x2{150 - 1, 10};

    mmio::WININ = {
        .win0_bg3 = true
    };

    mmio::WINOUT = {
        .outside_bg0 = true,
    };

    palette_ram[0] = 0x4210;

    setup_layers();

    // setup irq
    mmio::IRQ_HANDLER = agbabi::irq_user(window_hblank_irq_handler);

    mmio::DISPSTAT = {
        .irq_vblank = true,
        .irq_hblank = true
    };

    mmio::IE = {
        .vblank = true,
        .hblank = true,
    };

    wait_for_exit();

    mmio::IRQ_HANDLER = agbabi::irq_empty;

    mmio::DISPSTAT = {
        .irq_vblank = true
    };
}

// a hexagon
static const u8x2 hex_win_dims[160] {
    {143, 134},
    {144, 130},
    {145, 127},
    {146, 123},
    {147, 119},
    {148, 116},
    {149, 112},
    {150, 108},
    {151, 104},
    {152, 101},
    {153, 97},
    {154, 93},
    {155, 89},
    {156, 86},
    {157, 82},
    {158, 78},
    {159, 75},
    {160, 71},
    {161, 67},
    {162, 63},
    {163, 61},
    {164, 61},
    {165, 60},
    {166, 60},
    {167, 60},
    {168, 60},
    {169, 59},
    {170, 59},
    {171, 59},
    {172, 59},
    {173, 58},
    {174, 58},
    {175, 58},
    {176, 57},
    {177, 57},
    {178, 57},
    {179, 57},
    {180, 56},
    {181, 56},
    {182, 56},
    {183, 56},
    {184, 55},
    {185, 55},
    {186, 55},
    {187, 55},
    {188, 54},
    {189, 54},
    {190, 54},
    {191, 53},
    {192, 53},
    {193, 53},
    {194, 53},
    {195, 52},
    {196, 52},
    {197, 52},
    {198, 52},
    {199, 51},
    {200, 51},
    {200, 51},
    {199, 51},
    {199, 50},
    {199, 50},
    {199, 50},
    {198, 49},
    {198, 49},
    {198, 49},
    {197, 49},
    {197, 48},
    {197, 48},
    {197, 48},
    {196, 48},
    {196, 47},
    {196, 47},
    {196, 47},
    {195, 46},
    {195, 46},
    {195, 46},
    {194, 46},
    {194, 45},
    {194, 45},
    {194, 45},
    {193, 45},
    {193, 44},
    {193, 44},
    {193, 44},
    {192, 44},
    {192, 43},
    {192, 43},
    {192, 43},
    {191, 42},
    {191, 42},
    {191, 42},
    {190, 42},
    {190, 41},
    {190, 41},
    {190, 41},
    {189, 41},
    {189, 40},
    {189, 40},
    {189, 40},
    {188, 40},
    {188, 41},
    {188, 42},
    {188, 43},
    {187, 44},
    {187, 45},
    {187, 46},
    {186, 47},
    {186, 48},
    {186, 49},
    {186, 50},
    {185, 51},
    {185, 52},
    {185, 53},
    {185, 54},
    {184, 55},
    {184, 56},
    {184, 57},
    {183, 58},
    {183, 59},
    {183, 60},
    {183, 61},
    {182, 62},
    {182, 63},
    {182, 64},
    {182, 65},
    {181, 66},
    {181, 67},
    {181, 68},
    {181, 69},
    {180, 70},
    {180, 71},
    {180, 72},
    {179, 73},
    {179, 74},
    {179, 75},
    {179, 76},
    {178, 77},
    {174, 78},
    {171, 79},
    {167, 80},
    {163, 81},
    {160, 82},
    {156, 83},
    {152, 84},
    {148, 85},
    {145, 86},
    {141, 87},
    {137, 88},
    {133, 89},
    {130, 90},
    {126, 91},
    {122, 92},
    {118, 93},
    {115, 94},
    {111, 95},
    {107, 96},
    {104, 97},
    {100, 98},
    {142, 138}, // first
};

static void window_vblank_irq_handler(int mask) {
    mmio::DMA3_CONTROL.reset();

    mmio::DMA3_SRC = hex_win_dims;

    mmio::DMA3_CONTROL = dmacnt_h {
        .dest_control = dest_addr::fixed,
        .src_control = src_addr::increment,
        .repeat = true,
        .start_time = start::hblank,
        .enabled = true
    };
}

static const int win0h_addr = 0x4000040;

void display_window_hblank_dma() {
    // changing window x start/end in a hblank dma
    mmio::DISPCNT = {
        .video_mode = 0,
        .show_bg0 = true,
        .show_bg1 = true,
        .show_bg2 = true,
        .show_bg3 = true,
        .show_obj = true,
        .enable_win0 = true,
    };

    mmio::WIN0H = u8x2{120 - 1, 10};

    mmio::WIN0V = u8x2{161 - 1, 0};

    mmio::WININ = {
        .win0_bg3 = true
    };

    mmio::WINOUT = {
        .outside_bg0 = true,
    };

    palette_ram[0] = 0x4210;

    setup_layers();

    mmio::DMA3_DEST = reinterpret_cast<void *>(win0h_addr);
    mmio::DMA3_COUNT = 1;

    // need to reset every frame
    mmio::IRQ_HANDLER = agbabi::irq_user(window_vblank_irq_handler);

    wait_for_exit();

    mmio::IRQ_HANDLER = agbabi::irq_empty;
    mmio::DMA3_CONTROL.reset();
}

void display_window_invalid_x1() {
    mmio::DISPCNT = {
        .video_mode = 0,
        .show_bg0 = true,
        .show_bg1 = true,
        .show_bg2 = true,
        .show_bg3 = true,
        .show_obj = true,
        .enable_win0 = true,
        .enable_win1 = true,
    };

    mmio::WIN0H = u8x2{
        140 - 1,
        150, // start > end
    };

    mmio::WIN0V = u8x2{100 - 1, 10};

    mmio::WIN1H = u8x2{
        220 - 1,
        222, // start > end
    };

    mmio::WIN1V = u8x2{160 - 20 - 1, 30};

    mmio::WININ = {
        .win0_bg3 = true,

        .win1_bg2 = true
    };

    mmio::WINOUT = {
        .outside_bg0 = true,
    };

    palette_ram[0] = 0x4210;

    setup_layers();

    wait_for_exit();
}

void display_window_invalid_x2() {
    mmio::DISPCNT = {
        .video_mode = 0,
        .show_bg0 = true,
        .show_bg1 = true,
        .show_bg2 = true,
        .show_bg3 = true,
        .show_obj = true,
        .enable_win0 = true,
        .enable_win1 = true,
    };

    mmio::WIN0H = u8x2{
        245 - 1, // > 240
        10,
    };

    mmio::WIN0V = u8x2{100 - 1, 10};

    mmio::WIN1H = u8x2{
        250 - 1, // > 240
        15,
    };

    mmio::WIN1V = u8x2{160 - 20 - 1, 30};

    mmio::WININ = {
        .win0_bg3 = true,

        .win1_bg2 = true
    };

    mmio::WINOUT = {
        .outside_bg0 = true,
    };

    palette_ram[0] = 0x4210;

    setup_layers();

    wait_for_exit();
}

void display_window_invalid_y1() {
    mmio::DISPCNT = {
        .video_mode = 0,
        .show_bg0 = true,
        .show_bg1 = true,
        .show_bg2 = true,
        .show_bg3 = true,
        .show_obj = true,
        .enable_win0 = true,
        .enable_win1 = true,
    };

    mmio::WIN0H = u8x2{140 - 1, 10};

    mmio::WIN0V = u8x2{
        100 - 1,
        120, // start > end
    };

    mmio::WIN1H = u8x2{220 - 1, 15};

    mmio::WIN1V = u8x2{
        120 - 1,
        150,  // start > end
    };


    mmio::WININ = {
        .win0_bg3 = true,

        .win1_bg2 = true
    };

    mmio::WINOUT = {
        .outside_bg0 = true,
    };

    palette_ram[0] = 0x4210;

    setup_layers();

    wait_for_exit();
}

void display_window_invalid_y2() {
    mmio::DISPCNT = {
        .video_mode = 0,
        .show_bg0 = true,
        .show_bg1 = true,
        .show_bg2 = true,
        .show_bg3 = true,
        .show_obj = true,
        .enable_win0 = true,
        .enable_win1 = true,
    };

    mmio::WIN0H = u8x2{140 - 1, 10};

    mmio::WIN0V = u8x2{
        180 - 1, // > 160
        10,
    };

    mmio::WIN1H = u8x2{220 - 1, 15};

    mmio::WIN1V = u8x2{220 - 1, 60};

    mmio::WININ = {
        .win0_bg3 = true,

        .win1_bg2 = true
    };

    mmio::WINOUT = {
        .outside_bg0 = true,
    };

    palette_ram[0] = 0x4210;

    setup_layers();

    wait_for_exit();
}

void display_window_invalid_y2_no_disable() {
    // sets Y2 to > 228, causing the window to never disable
    mmio::DISPCNT = {
        .video_mode = 0,
        .show_bg0 = true,
        .show_bg1 = true,
        .show_bg2 = true,
        .show_bg3 = true,
        .show_obj = true,
        .enable_win0 = true,
    };

    mmio::WIN0H = u8x2{140 - 1, 10};

    mmio::WIN0V = u8x2{
        229 - 1, // past the end of vblank, never triggers disable
        140,
    };

    mmio::WININ = {
        .win0_bg3 = true
    };

    mmio::WINOUT = {
        .outside_bg0 = true,
    };

    palette_ram[0] = 0x4210;

    setup_layers();

    wait_for_exit();
}
