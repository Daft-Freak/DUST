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

static void window_hblank_irq_handler(interrupt_mask mask) {
    auto vcount = reg::vcount::read() + 1;

    if(vcount > 160)
        return;

    if(vcount == 160)
        vcount = 0;

    if(vcount < 80) {
        reg::win0h::write(window_dimension {
            .end = uint8_t(121 + vcount),
            .begin = uint8_t(120 - vcount),
        });
    } else {
        reg::win0h::write(window_dimension {
            .end = uint8_t(279 - vcount),
            .begin = uint8_t(vcount - 40),
        });
    }
}

void display_window_hblank_irq() {
    // changing window x start/end in a hblank irq
    reg::dispcnt::write({
        .mode = 0,
        .layer_background_0 = true,
        .layer_background_1 = true,
        .layer_background_2 = true,
        .layer_background_3 = true,
        .layer_object = true,
        .window_0 = true,
    });

    reg::win0h::write({
        .end = 120,
        .begin = 120,
    });

    reg::win0v::write({
        .end = 150,
        .begin = 10,
    });

    reg::winin::write({
        .win0_bg3 = true,
    });

    reg::winout::write({
        .win0_bg0 = true,
    });

    palette_ram[0] = 0x4210;

    setup_layers();

    // setup irq
    agbabi::interrupt_handler::set(window_hblank_irq_handler);

    reg::dispstat::write({
        .vblank_irq = true,
        .hblank_irq = true
    });

    reg::ie::write({
        .vblank = true,
        .hblank = true,
    });

    wait_for_exit();

    agbabi::interrupt_handler::set(nullptr);

    reg::dispstat::write({
        .vblank_irq = true
    });
}

// a hexagon
static const window_dimension hex_win_dims[160] {
    {144, 134},
    {145, 130},
    {146, 127},
    {147, 123},
    {148, 119},
    {149, 116},
    {150, 112},
    {151, 108},
    {152, 104},
    {153, 101},
    {154, 97},
    {155, 93},
    {156, 89},
    {157, 86},
    {158, 82},
    {159, 78},
    {160, 75},
    {161, 71},
    {162, 67},
    {163, 63},
    {164, 61},
    {165, 61},
    {166, 60},
    {167, 60},
    {168, 60},
    {169, 60},
    {170, 59},
    {171, 59},
    {172, 59},
    {173, 59},
    {174, 58},
    {175, 58},
    {176, 58},
    {177, 57},
    {178, 57},
    {179, 57},
    {180, 57},
    {181, 56},
    {182, 56},
    {183, 56},
    {184, 56},
    {185, 55},
    {186, 55},
    {187, 55},
    {188, 55},
    {189, 54},
    {190, 54},
    {191, 54},
    {192, 53},
    {193, 53},
    {194, 53},
    {195, 53},
    {196, 52},
    {197, 52},
    {198, 52},
    {199, 52},
    {200, 51},
    {201, 51},
    {201, 51},
    {200, 51},
    {200, 50},
    {200, 50},
    {200, 50},
    {199, 49},
    {199, 49},
    {199, 49},
    {198, 49},
    {198, 48},
    {198, 48},
    {198, 48},
    {197, 48},
    {197, 47},
    {197, 47},
    {197, 47},
    {196, 46},
    {196, 46},
    {196, 46},
    {195, 46},
    {195, 45},
    {195, 45},
    {195, 45},
    {194, 45},
    {194, 44},
    {194, 44},
    {194, 44},
    {193, 44},
    {193, 43},
    {193, 43},
    {193, 43},
    {192, 42},
    {192, 42},
    {192, 42},
    {191, 42},
    {191, 41},
    {191, 41},
    {191, 41},
    {190, 41},
    {190, 40},
    {190, 40},
    {190, 40},
    {189, 40},
    {189, 41},
    {189, 42},
    {189, 43},
    {188, 44},
    {188, 45},
    {188, 46},
    {187, 47},
    {187, 48},
    {187, 49},
    {187, 50},
    {186, 51},
    {186, 52},
    {186, 53},
    {186, 54},
    {185, 55},
    {185, 56},
    {185, 57},
    {184, 58},
    {184, 59},
    {184, 60},
    {184, 61},
    {183, 62},
    {183, 63},
    {183, 64},
    {183, 65},
    {182, 66},
    {182, 67},
    {182, 68},
    {182, 69},
    {181, 70},
    {181, 71},
    {181, 72},
    {180, 73},
    {180, 74},
    {180, 75},
    {180, 76},
    {179, 77},
    {175, 78},
    {172, 79},
    {168, 80},
    {164, 81},
    {161, 82},
    {157, 83},
    {153, 84},
    {149, 85},
    {146, 86},
    {142, 87},
    {138, 88},
    {134, 89},
    {131, 90},
    {127, 91},
    {123, 92},
    {119, 93},
    {116, 94},
    {112, 95},
    {108, 96},
    {105, 97},
    {101, 98},
    {143, 138}, // first
};

static void window_vblank_irq_handler(interrupt_mask mask) {
    reg::dma3cnt_h::write({});

    reg::dma3sad::write(reinterpret_cast<uint32_t>(hex_win_dims));

    reg::dma3cnt_h::write(dma_control {
        .destination_control = dma_control::destination_address::fixed,
        .source_control = dma_control::source_address::increment,
        .repeat = true,
        .type = dma_control::type::half,
        .start_condition = dma_control::start::next_hblank,
        .enable = true
    });
}

void display_window_hblank_dma() {
    // changing window x start/end in a hblank dma
    reg::dispcnt::write({
        .mode = 0,
        .layer_background_0 = true,
        .layer_background_1 = true,
        .layer_background_2 = true,
        .layer_background_3 = true,
        .layer_object = true,
        .window_0 = true,
    });

    reg::win0h::write({
        .end = 120,
        .begin = 10,
    });

    reg::win0v::write({
        .end = 161,
        .begin = 0,
    });

    reg::winin::write({
        .win0_bg3 = true,
    });

    reg::winout::write({
        .win0_bg0 = true,
    });

    palette_ram[0] = 0x4210;

    setup_layers();

    reg::dma3dad::write(reg::win0h::address);
    reg::dma3cnt_l::write(1);

    // need to reset every frame
    agbabi::interrupt_handler::set(window_vblank_irq_handler);

    wait_for_exit();

    agbabi::interrupt_handler::set(nullptr);
    reg::dma3cnt_h::write({});
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

void display_window_invalid_y2_no_disable() {
    // sets Y2 to > 228, causing the window to never disable
    reg::dispcnt::write({
        .mode = 0,
        .layer_background_0 = true,
        .layer_background_1 = true,
        .layer_background_2 = true,
        .layer_background_3 = true,
        .layer_object = true,
        .window_0 = true,
    });

    reg::win0h::write({
        .end = 140,
        .begin = 10,
    });

    reg::win0v::write({
        .end = 229, // past the end of vblank, never triggers disable
        .begin = 140,
    });
    reg::winin::write({
        .win0_bg3 = true,
    });

    reg::winout::write({
        .win0_bg0 = true,
    });

    palette_ram[0] = 0x4210;

    setup_layers();

    wait_for_exit();
}