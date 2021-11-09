#include <gba/gba.hpp>

#include "../common.hpp"

#include "../assets/sprites.h"
#include "../assets/sprites_256.h"

using namespace gba;

inline auto oam_reg = reinterpret_cast<object_regular *>(0x07000000);
inline auto oam_aff = reinterpret_cast<object_affine *>(0x07000000);

static void set_object(int index, object::attr0 attr0, object::attr1_regular attr1, object::attr2 attr2) {
    oam_reg[index].attr0 = attr0;
    oam_reg[index].attr1 = attr1;
    oam_reg[index].attr2 = attr2;
}

static void set_object(int index, object::attr0 attr0, object::attr1_affine attr1, object::attr2 attr2) {
    oam_aff[index].attr0 = attr0;
    oam_aff[index].attr1 = attr1;
    oam_aff[index].attr2 = attr2;
}

static void setup_objects(object::mode mode, color_depth depth = color_depth::bpp_4, bool flip_h = false, bool flip_v = false, uint16_t pal_num = 0) {
    const int spacing = 10;
    uint16_t x = spacing, y = spacing;

    // tile mapping is diffent for 8bpp, so sheet is rearranged
    bool is_8 = depth == color_depth::bpp_8;

    // size 0

    // 8x8
    set_object(0, object::attr0 {
        .y = y,
        .object_mode = mode,
        .color_mode = depth,
        .shape = object::shape::square
    }, object::attr1_regular {
        .x = x,
        .flip_horizontal = flip_h,
        .flip_vertical = flip_v,
        .size = 0
    }, object::attr2 {
        .tile_index = 0,
        .palette_bank = pal_num
    });
    y += 8 + spacing;

    // 16x8
    set_object(1, object::attr0 {
        .y = y,
        .object_mode = mode,
        .color_mode = depth,
        .shape = object::shape::wide
    }, object::attr1_regular {
        .x = x,
        .flip_horizontal = flip_h,
        .flip_vertical = flip_v,
        .size = 0
    }, object::attr2 {
        .tile_index = uint16_t(is_8 ? 2 : 1),
        .palette_bank = pal_num
    });

    y += 8 + spacing;

    // 8x16
    set_object(2, object::attr0 {
        .y = y,
        .object_mode = mode,
        .color_mode = depth,
        .shape = object::shape::tall
    }, object::attr1_regular {
        .x = x,
        .flip_horizontal = flip_h,
        .flip_vertical = flip_v,
        .size = 0
    }, object::attr2 {
        .tile_index = 32,
        .palette_bank = pal_num
    });

    y = spacing;
    x += 16 + spacing;

    // size 1

    // 16x16
    set_object(3, object::attr0 {
        .y = y,
        .object_mode = mode,
        .color_mode = depth,
        .shape = object::shape::square
    }, object::attr1_regular {
        .x = x,
        .flip_horizontal = flip_h,
        .flip_vertical = flip_v,
        .size = 1
    }, object::attr2 {
        .tile_index = uint16_t(is_8 ? 6 : 3),
        .palette_bank = pal_num
    });

    y += 16 + spacing;

    // 32x8
    set_object(4, object::attr0 {
        .y = y,
        .object_mode = mode,
        .color_mode = depth,
        .shape = object::shape::wide
    }, object::attr1_regular {
        .x = x,
        .flip_horizontal = flip_h,
        .flip_vertical = flip_v,
        .size = 1
    }, object::attr2 {
        .tile_index = uint16_t(is_8 ? 10 : 5),
        .palette_bank = pal_num
    });

    y += 8 + spacing;

    // 8x32
    set_object(5, object::attr0 {
        .y = y,
        .object_mode = mode,
        .color_mode = depth,
        .shape = object::shape::tall
    }, object::attr1_regular {
        .x = x,
        .flip_horizontal = flip_h,
        .flip_vertical = flip_v,
        .size = 1
    }, object::attr2 {
        .tile_index = uint16_t(is_8 ? 18 : 9),
        .palette_bank = pal_num
    });

    x += 32 + spacing;
    y = spacing;

    // size 2

    // 32x32
    set_object(6, object::attr0 {
        .y = y,
        .object_mode = mode,
        .color_mode = depth,
        .shape = object::shape::square
    }, object::attr1_regular {
        .x = x,
        .flip_horizontal = flip_h,
        .flip_vertical = flip_v,
        .size = 2
    }, object::attr2 {
        .tile_index = uint16_t(is_8 ? 20 : 10),
        .palette_bank = pal_num
    });

    y += 32 + spacing;

    // 32x16
    set_object(7, object::attr0 {
        .y = y,
        .object_mode = mode,
        .color_mode = depth,
        .shape = object::shape::wide
    }, object::attr1_regular {
        .x = x,
        .flip_horizontal = flip_h,
        .flip_vertical = flip_v,
        .size = 2
    }, object::attr2 {
        .tile_index = uint16_t(is_8 ? 42 : 14),
        .palette_bank = pal_num
    });

    y += 16 + spacing;

    // 16x32
    set_object(8, object::attr0 {
        .y = y,
        .object_mode = mode,
        .color_mode = depth,
        .shape = object::shape::tall
    }, object::attr1_regular {
        .x = x,
        .flip_horizontal = flip_h,
        .flip_vertical = flip_v,
        .size = 2
    }, object::attr2 {
        .tile_index = uint16_t(is_8 ? 28 : 18),
        .palette_bank = pal_num
    });

    x += 32 + spacing;
    y = spacing;

    // size 3

    // 64x64
    set_object(9, object::attr0 {
        .y = y,
        .object_mode = mode,
        .color_mode = depth,
        .shape = object::shape::square
    }, object::attr1_regular {
        .x = x,
        .flip_horizontal = flip_h,
        .flip_vertical = flip_v,
        .size = 3
    }, object::attr2 {
        .tile_index = uint16_t(is_8 ? 264 : 20),
        .palette_bank = pal_num
    });

    y += 64 + spacing;

    // 64x32
    set_object(10, object::attr0 {
        .y = y,
        .object_mode = mode,
        .color_mode = depth,
        .shape = object::shape::wide
    }, object::attr1_regular {
        .x = x,
        .flip_horizontal = flip_h,
        .flip_vertical = flip_v,
        .size = 3
    }, object::attr2 {
        .tile_index = uint16_t(is_8 ? 144 : 140),
        .palette_bank = pal_num
    });

    // new column, no space left
    y = spacing;
    x += 64 + spacing;

    // 32x64
    set_object(11, object::attr0 {
        .y = y,
        .object_mode = mode,
        .color_mode = depth,
        .shape = object::shape::tall
    }, object::attr1_regular {
        .x = x,
        .flip_horizontal = flip_h,
        .flip_vertical = flip_v,
        .size = 3
    }, object::attr2 {
        .tile_index = uint16_t(is_8 ? 280 : 28),
        .palette_bank = pal_num
    });

    // hide everything else off-screen
    for(int i = 12; i < 128; i++) {
        oam_reg[i].attr0 = {.y = 160};
    }
}

// mostly copy/paste... but sets affine_index
static void setup_affine_objects(object::mode mode, color_depth depth = color_depth::bpp_4) {
    const int spacing = 10;
    uint16_t x = spacing, y = spacing;

    // tile mapping is diffent for 8bpp, so sheet is rearranged
    bool is_8 = depth == color_depth::bpp_8;

    // size 0

    // 8x8
    set_object(0, object::attr0 {
        .y = y,
        .object_mode = mode,
        .color_mode = depth,
        .shape = object::shape::square
    }, object::attr1_affine {
        .x = x,
        .affine_index = 0,
        .size = 0
    }, object::attr2 {
        .tile_index = 0
    });
    y += 8 + spacing;

    // 16x8
    set_object(1, object::attr0 {
        .y = y,
        .object_mode = mode,
        .color_mode = depth,
        .shape = object::shape::wide
    }, object::attr1_affine {
        .x = x,
        .affine_index = 1,
        .size = 0
    }, object::attr2 {
        .tile_index = uint16_t(is_8 ? 2 : 1),
    });

    y += 8 + spacing;

    // 8x16
    set_object(2, object::attr0 {
        .y = y,
        .object_mode = mode,
        .color_mode = depth,
        .shape = object::shape::tall
    }, object::attr1_affine {
        .x = x,
        .affine_index = 2,
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
        .color_mode = depth,
        .shape = object::shape::square
    }, object::attr1_affine {
        .x = x,
        .affine_index = 3,
        .size = 1
    }, object::attr2 {
        .tile_index = uint16_t(is_8 ? 6 : 3),
    });

    y += 16 + spacing;

    // 32x8
    set_object(4, object::attr0 {
        .y = y,
        .object_mode = mode,
        .color_mode = depth,
        .shape = object::shape::wide
    }, object::attr1_affine {
        .x = x,
        .affine_index = 4,
        .size = 1
    }, object::attr2 {
        .tile_index = uint16_t(is_8 ? 10 : 5),
    });

    y += 8 + spacing;

    // 8x32
    set_object(5, object::attr0 {
        .y = y,
        .object_mode = mode,
        .color_mode = depth,
        .shape = object::shape::tall
    }, object::attr1_affine {
        .x = x,
        .affine_index = 5,
        .size = 1
    }, object::attr2 {
        .tile_index = uint16_t(is_8 ? 18 : 9),
    });

    x += 32 + spacing;
    y = spacing;

    // size 2

    // 32x32
    set_object(6, object::attr0 {
        .y = y,
        .object_mode = mode,
        .color_mode = depth,
        .shape = object::shape::square
    }, object::attr1_affine {
        .x = x,
        .affine_index = 6,
        .size = 2
    }, object::attr2 {
        .tile_index = uint16_t(is_8 ? 20 : 10),
    });

    y += 32 + spacing;

    // 32x16
    set_object(7, object::attr0 {
        .y = y,
        .object_mode = mode,
        .color_mode = depth,
        .shape = object::shape::wide
    }, object::attr1_affine {
        .x = x,
        .affine_index = 7,
        .size = 2
    }, object::attr2 {
        .tile_index = uint16_t(is_8 ? 42 : 14),
    });

    y += 16 + spacing;

    // 16x32
    set_object(8, object::attr0 {
        .y = y,
        .object_mode = mode,
        .color_mode = depth,
        .shape = object::shape::tall
    }, object::attr1_affine {
        .x = x,
        .affine_index = 8,
        .size = 2
    }, object::attr2 {
        .tile_index = uint16_t(is_8 ? 28 : 18),
    });

    x += 32 + spacing;
    y = spacing;

    // size 3

    // 64x64
    set_object(9, object::attr0 {
        .y = y,
        .object_mode = mode,
        .color_mode = depth,
        .shape = object::shape::square
    }, object::attr1_affine {
        .x = x,
        .affine_index = 9,
        .size = 3
    }, object::attr2 {
        .tile_index = uint16_t(is_8 ? 264 : 20),
    });

    y += 64 + spacing;

    // 64x32
    set_object(10, object::attr0 {
        .y = y,
        .object_mode = mode,
        .color_mode = depth,
        .shape = object::shape::wide
    }, object::attr1_affine {
        .x = x,
        .affine_index = 10,
        .size = 3
    }, object::attr2 {
        .tile_index = uint16_t(is_8 ? 144 : 140),
    });

    // new column, no space left
    y = spacing;
    x += 64 + spacing;

    // 32x64
    set_object(11, object::attr0 {
        .y = y,
        .object_mode = mode,
        .color_mode = depth,
        .shape = object::shape::tall
    }, object::attr1_affine {
        .x = x,
        .affine_index = 11,
        .size = 3
    }, object::attr2 {
        .tile_index = uint16_t(is_8 ? 280 : 28),
    });

    // hide everything else off-screen
    for(int i = 12; i < 128; i++) {
        oam_reg[i].attr0 = {.y = 160};
    }
}


void display_obj_regular() {
    // "regular" non-affine sprites, every size/shape

    palette_ram[0] = 0x4210;

    // load sprites
    agbabi::memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    agbabi::memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

    reg::dispcnt::write({
        .mode = 0,
        .layer_object = true,
    });

    setup_objects(object::mode::regular);

    wait_for_exit();
}

void display_obj_affine() {
    // affine sprites, every size/shape

    palette_ram[0] = 0x4210;

    // load sprites
    agbabi::memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    agbabi::memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

    reg::dispcnt::write({
        .mode = 0,
        .layer_object = true,
    });

    setup_objects(object::mode::affine);

    // init affine params
    oam_reg[0].attr3 = 1 << 8;
    oam_reg[1].attr3 = 0 << 8;
    oam_reg[2].attr3 = 0 << 8;
    oam_reg[3].attr3 = 1 << 8;

    wait_for_exit();
}

void display_obj_affine_double() {
    // affine double-size sprites, every size/shape

    palette_ram[0] = 0x4210;

    // load sprites
    agbabi::memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    agbabi::memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

    reg::dispcnt::write({
        .mode = 0,
        .layer_object = true,
    });

    setup_objects(object::mode::affine_double);

    // init affine params
    oam_reg[0].attr3 = 1 << 8;
    oam_reg[1].attr3 = 0 << 8;
    oam_reg[2].attr3 = 0 << 8;
    oam_reg[3].attr3 = 1 << 8;

    wait_for_exit();
}

void display_obj_hidden() {
    // disabled sprites, every size/shape
    // ...which doesn't matter as they're hidden

    palette_ram[0] = 0x4210;

    // load sprites
    agbabi::memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    agbabi::memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

    reg::dispcnt::write({
        .mode = 0,
        .layer_object = true,
    });

    setup_objects(object::mode::hidden);

    wait_for_exit();
}

void display_obj_regular_8bpp() {
    palette_ram[0] = 0x4210;

    // load sprites
    agbabi::memcpy2(video_ram + 0x10000 / 2, sprites_256_tile_data, sizeof(sprites_256_tile_data));
    agbabi::memcpy2(palette_ram + 0x200 / 2, sprites_256_palette, sizeof(sprites_256_palette));

    reg::dispcnt::write({
        .mode = 0,
        .layer_object = true,
    });

    setup_objects(object::mode::regular, color_depth::bpp_8);

    wait_for_exit();
}

void display_obj_affine_8bpp() {
    palette_ram[0] = 0x4210;

    // load sprites
    agbabi::memcpy2(video_ram + 0x10000 / 2, sprites_256_tile_data, sizeof(sprites_256_tile_data));
    agbabi::memcpy2(palette_ram + 0x200 / 2, sprites_256_palette, sizeof(sprites_256_palette));

    reg::dispcnt::write({
        .mode = 0,
        .layer_object = true,
    });

    setup_objects(object::mode::affine, color_depth::bpp_8);

    // init affine params
    oam_reg[0].attr3 = 1 << 8;
    oam_reg[1].attr3 = 0 << 8;
    oam_reg[2].attr3 = 0 << 8;
    oam_reg[3].attr3 = 1 << 8;

    wait_for_exit();
}

void display_obj_regular_hflip() {
    // "regular" non-affine sprites, every size/shape, h-flipped

    palette_ram[0] = 0x4210;

    // load sprites
    agbabi::memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    agbabi::memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

    reg::dispcnt::write({
        .mode = 0,
        .layer_object = true,
    });

    setup_objects(object::mode::regular, color_depth::bpp_4, true);

    wait_for_exit();
}

void display_obj_regular_vflip() {
    // "regular" non-affine sprites, every size/shape, v-flipped

    palette_ram[0] = 0x4210;

    // load sprites
    agbabi::memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    agbabi::memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

    reg::dispcnt::write({
        .mode = 0,
        .layer_object = true,
    });

    setup_objects(object::mode::regular, color_depth::bpp_4, false, true);

    wait_for_exit();
}

void display_obj_regular_hvflip() {
    // "regular" non-affine sprites, every size/shape, h and v-flipped

    palette_ram[0] = 0x4210;

    // load sprites
    agbabi::memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    agbabi::memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

    reg::dispcnt::write({
        .mode = 0,
        .layer_object = true,
    });

    setup_objects(object::mode::regular, color_depth::bpp_4, true, true);

    wait_for_exit();
}

static void setup_affine_params() {
    for(int i = 0; i < 12; i++) {
        oam_reg[i * 4 + 0].attr3 = 1 << 8;
        oam_reg[i * 4 + 1].attr3 = 0 << 8;
        oam_reg[i * 4 + 2].attr3 = 0 << 8;
        oam_reg[i * 4 + 3].attr3 = 1 << 8;

        float scale = (i / 3 + 1) / 3.0f;

        if(i & 1)
            scale = 1.0f / scale;

        bios::obj_affine_input input {
            .scale_x = scale,
            .scale_y = scale,
            .rotation = (i + 1) * 10
        };

        // hack to avoid input getting optimised out...
        volatile uint32_t buf[2];
        memcpy((void *)buf, &input, sizeof(input));

        bios::obj_affine_set(&input, reinterpret_cast<object::mat2 *>(&oam_reg[i * 4].attr3), 1, 8);
    }
}

void display_obj_affine_rotscale() {
    palette_ram[0] = 0x4210;

    // load sprites
    agbabi::memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    agbabi::memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

    reg::dispcnt::write({
        .mode = 0,
        .layer_object = true,
    });

    setup_affine_objects(object::mode::affine);
    setup_affine_params();

    wait_for_exit();
}

void display_obj_affine_double_rotscale() {
    palette_ram[0] = 0x4210;

    // load sprites
    agbabi::memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    agbabi::memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

    reg::dispcnt::write({
        .mode = 0,
        .layer_object = true,
    });

    setup_affine_objects(object::mode::affine_double);
    setup_affine_params();

    wait_for_exit();
}

void display_obj_regular_4bpp_pal() {
    palette_ram[0] = 0x4210;

    // load sprites
    agbabi::memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    agbabi::memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

    palette_ram[256 + 17] = 0x000F;
    palette_ram[256 + 18] = 0x1084;
    palette_ram[256 + 19] = 0x0007;

    reg::dispcnt::write({
        .mode = 0,
        .layer_object = true,
    });

    setup_objects(object::mode::regular, color_depth::bpp_4, false, false, 1);

    wait_for_exit();
}

void display_obj_regular_1d() {
    // "regular" non-affine sprites, every size/shape

    palette_ram[0] = 0x4210;

    // load sprites
    agbabi::memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    agbabi::memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

    reg::dispcnt::write({
        .mode = 0,
        .object_tile_map = object::tile_map::one_dimensional,
        .layer_object = true,
    });

    setup_objects(object::mode::regular);

    wait_for_exit();
}

void display_obj_regular_8bpp_1d() {
    palette_ram[0] = 0x4210;

    // load sprites
    agbabi::memcpy2(video_ram + 0x10000 / 2, sprites_256_tile_data, sizeof(sprites_256_tile_data));
    agbabi::memcpy2(palette_ram + 0x200 / 2, sprites_256_palette, sizeof(sprites_256_palette));

    reg::dispcnt::write({
        .mode = 0,
        .object_tile_map = object::tile_map::one_dimensional,
        .layer_object = true,
    });

    setup_objects(object::mode::regular, color_depth::bpp_8);

    wait_for_exit();
}


void display_obj_wrap_x() {
    palette_ram[0] = 0x4210;

    // load sprites
    agbabi::memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    agbabi::memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

    reg::dispcnt::write({
        .mode = 0,
        .layer_object = true,
    });

    // right edge of the screen, no wrapping

    // 8x8
    set_object(0, object::attr0 {
        .y = 0,
    }, object::attr1_regular {
        .x = 240 - 4,
    }, object::attr2 {
        .tile_index = 0
    });

    // 16x16
    set_object(1, object::attr0 {
        .y = 8,
    }, object::attr1_regular {
        .x = 240 - 8,
        .size = 1
    }, object::attr2 {
        .tile_index = 3
    });

    // 32x32
    set_object(2, object::attr0 {
        .y = 24,
    }, object::attr1_regular {
        .x = 240 - 16,
        .size = 2
    }, object::attr2 {
        .tile_index = 10
    });

    // 64x64
    set_object(3, object::attr0 {
        .y = 56,
    }, object::attr1_regular {
        .x = 240 - 32,
        .size = 3
    }, object::attr2 {
        .tile_index = 20
    });

    // 128x128 (64x64 x2)
    set_object(4, object::attr0 {
        .y = 0,
        .object_mode = object::mode::affine_double
    }, object::attr1_regular {
        .x = 240 - 64,
        .size = 3
    }, object::attr2 {
        .tile_index = 20
    });

    // wrap back to left

    // 8x8
    set_object(5, object::attr0 {
        .y = 152,
    }, object::attr1_regular {
        .x = 512 - 4,
    }, object::attr2 {
        .tile_index = 0
    });

    // 16x16
    set_object(6, object::attr0 {
        .y = 136,
    }, object::attr1_regular {
        .x = 512 - 8,
        .size = 1
    }, object::attr2 {
        .tile_index = 3
    });

    // 32x32
    set_object(7, object::attr0 {
        .y = 104,
    }, object::attr1_regular {
        .x = 512 - 16,
        .size = 2
    }, object::attr2 {
        .tile_index = 10
    });

    // 64x64
    set_object(8, object::attr0 {
        .y = 40,
    }, object::attr1_regular {
        .x = 512 - 32,
        .size = 3
    }, object::attr2 {
        .tile_index = 20 
    });

    // 128x128 (64x64 x2)
    set_object(9, object::attr0 {
        .y = 32,
        .object_mode = object::mode::affine_double
    }, object::attr1_regular {
        .x = 512 - 64,
        .size = 3
    }, object::attr2 {
        .tile_index = 20
    });

    // hide everything else off-screen
    for(int i = 10; i < 128; i++) {
        oam_reg[i].attr0 = {.y = 160};
    }

    // 2x scale
    oam_reg[0].attr3 = 0x80;
    oam_reg[1].attr3 = 0;
    oam_reg[2].attr3 = 0;
    oam_reg[3].attr3 = 0x80;

    wait_for_exit();
}

void display_obj_wrap_y() {
    palette_ram[0] = 0x4210;

    // load sprites
    agbabi::memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    agbabi::memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

    reg::dispcnt::write({
        .mode = 0,
        .layer_object = true,
    });

    // bottom edge of the screen, no wrapping

    // 8x8
    set_object(0, object::attr0 {
        .y = 160 - 4,
    }, object::attr1_regular {
        .x = 0,
    }, object::attr2 {
        .tile_index = 0
    });

    // 16x16
    set_object(1, object::attr0 {
        .y = 160 - 8,
    }, object::attr1_regular {
        .x = 8,
        .size = 1
    }, object::attr2 {
        .tile_index = 3
    });

    // 32x32
    set_object(2, object::attr0 {
        .y = 160 - 16,
    }, object::attr1_regular {
        .x = 24,
        .size = 2
    }, object::attr2 {
        .tile_index = 10
    });

    // 64x64
    set_object(3, object::attr0 {
        .y = 160 - 32,
    }, object::attr1_regular {
        .x = 56,
        .size = 3
    }, object::attr2 {
        .tile_index = 20
    });

    // 128x128 (64x64 x2)
    set_object(4, object::attr0 {
        .y = 160 - 64,
        .object_mode = object::mode::affine_double
    }, object::attr1_regular {
        .x = 120,
        .size = 3
    }, object::attr2 {
        .tile_index = 20
    });

    // wrap back to top

    // 8x8
    set_object(5, object::attr0 {
        .y = 256 - 4,
    }, object::attr1_regular {
        .x = 232,
    }, object::attr2 {
        .tile_index = 0
    });

    // 16x16
    set_object(6, object::attr0 {
        .y = 256 - 8,
    }, object::attr1_regular {
        .x = 216,
        .size = 1
    }, object::attr2 {
        .tile_index = 3
    });

    // 32x32
    set_object(7, object::attr0 {
        .y = 256 - 16,
    }, object::attr1_regular {
        .x = 184,
        .size = 2
    }, object::attr2 {
        .tile_index = 10
    });

    // 64x64
    set_object(8, object::attr0 {
        .y = 256 - 32,
    }, object::attr1_regular {
        .x = 120,
        .size = 3
    }, object::attr2 {
        .tile_index = 20 
    });

    // 128x128 (64x64 x2)
    set_object(9, object::attr0 {
        .y = 256 - 64,
        .object_mode = object::mode::affine_double
    }, object::attr1_regular {
        .x = 504, // -8
        .size = 3
    }, object::attr2 {
        .tile_index = 20
    });

    // hide everything else off-screen
    for(int i = 10; i < 128; i++) {
        oam_reg[i].attr0 = {.y = 160};
    }

    // 2x scale
    oam_reg[0].attr3 = 0x80;
    oam_reg[1].attr3 = 0;
    oam_reg[2].attr3 = 0;
    oam_reg[3].attr3 = 0x80;

    wait_for_exit();
}

void display_obj_wrap_y_bug() {
    palette_ram[0] = 0x4210;

    // load sprites
    agbabi::memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    agbabi::memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

    reg::dispcnt::write({
        .mode = 0,
        .layer_object = true,
    });

    // 128x128 (64x64 x2)
    set_object(0, object::attr0 {
        .y = 128,
        .object_mode = object::mode::affine_double
    }, object::attr1_regular {
        .x = 120,
        .size = 3
    }, object::attr2 {
        .tile_index = 20
    });

    // should have 31 pixels visivle at the bottom and wrap around to the top
    // instead only shows at the top
    set_object(1, object::attr0 {
        .y = 129,
        .object_mode = object::mode::affine_double
    }, object::attr1_regular {
        .x = 504, // -8
        .size = 3
    }, object::attr2 {
        .tile_index = 20
    });

    // hide everything else off-screen
    for(int i = 2; i < 128; i++) {
        oam_reg[i].attr0 = {.y = 160};
    }

    // 2x scale
    oam_reg[0].attr3 = 0x80;
    oam_reg[1].attr3 = 0;
    oam_reg[2].attr3 = 0;
    oam_reg[3].attr3 = 0x80;

    wait_for_exit();
}

void display_obj_bmp_char_base() {
    palette_ram[0] = 0x4210;

    // load sprites
    // first 16 is used by background so these can't be used
    agbabi::memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    // this should be okay
    agbabi::memcpy2(video_ram + 0x14000 / 2, sprites_tile_data, sizeof(sprites_tile_data));

    agbabi::memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

    reg::dispcnt::write({
        .mode = 3, // bitmap
        .layer_object = true,
    });

    // this sprite won't be shown
    set_object(0, object::attr0 {
        .y = 16,
    }, object::attr1_regular {
        .x = 16,
        .size = 3
    }, object::attr2 {
        .tile_index = 20
    });

    // this one will
    set_object(1, object::attr0 {
        .y = 16,
    }, object::attr1_regular {
        .x = 64 + 32,
        .size = 3
    }, object::attr2 {
        .tile_index = 20 + 512
    });

    // hide everything else off-screen
    for(int i = 2; i < 128; i++) {
        oam_reg[i].attr0 = {.y = 160};
    }

    wait_for_exit();
}

static void object_limit_test(uint16_t size, object::mode mode = object::mode::regular, int x_off = 0, bool partial_hidden = false) {
    palette_ram[0] = 0x4210;

    // load sprites
    agbabi::memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    agbabi::memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

    reg::dispcnt::write({
        .mode = 0,
        .layer_object = true,
    });

    const uint16_t tiles[]{0, 3, 10, 20};

    int y_mask = (4 << size) - 1;

    for(int i = 0; i < 128; i++) {
        set_object(i, object::attr0 {
            .y = uint16_t(i & y_mask),
            .object_mode = partial_hidden && i < 64 ? object::mode::hidden : mode
        }, object::attr1_regular {
            .x = uint16_t(i + x_off),
            .size = size
        }, object::attr2 {
            .tile_index = tiles[size]
        });
    }

    oam_reg[0].attr3 = mode == object::mode::affine_double ? 0x80 : 0x100;
    oam_reg[1].attr3 = 0;
    oam_reg[2].attr3 = 0;
    oam_reg[3].attr3 = mode == object::mode::affine_double ? 0x80 : 0x100;
}

void display_obj_line_limit_regular_size0() {
    object_limit_test(0);
    wait_for_exit();
}

void display_obj_line_limit_regular_size1() {
    object_limit_test(1);
    wait_for_exit();
}

void display_obj_line_limit_regular_size2() {
    object_limit_test(2);
    wait_for_exit();
}

void display_obj_line_limit_regular_size3() {
    object_limit_test(3);
    wait_for_exit();
}

void display_obj_line_limit_affine_size0() {
    object_limit_test(0, object::mode::affine);
    wait_for_exit();
}

void display_obj_line_limit_affine_size1() {
    object_limit_test(1, object::mode::affine);
    wait_for_exit();
}

void display_obj_line_limit_affine_size2() {
    object_limit_test(2, object::mode::affine);
    wait_for_exit();
}

void display_obj_line_limit_affine_size3() {
    object_limit_test(3, object::mode::affine);
    wait_for_exit();
}

void display_obj_line_limit_affine_double_size0() {
    object_limit_test(0, object::mode::affine_double);
    wait_for_exit();
}

void display_obj_line_limit_affine_double_size1() {
    object_limit_test(1, object::mode::affine_double);
    wait_for_exit();
}

void display_obj_line_limit_affine_double_size2() {
    object_limit_test(2, object::mode::affine_double);
    wait_for_exit();
}

void display_obj_line_limit_affine_double_size3() {
    object_limit_test(3, object::mode::affine_double);
    wait_for_exit();
}

// not testing every variant because there are already too many tests here...
void display_obj_line_limit_regular_offscreen() {
    // move some of the sprites off the screen
    object_limit_test(2, object::mode::regular, -64);
    wait_for_exit();
}

void display_obj_line_limit_regular_hidden() {
    // first half set to hidden
    object_limit_test(2, object::mode::regular, 0, true);
    wait_for_exit();
}

void display_obj_line_limit_regular_size0_hblank_access() {
    object_limit_test(0);

    reg::dispcnt::write({
        .mode = 0,
        .oam_hblank_access = oam_hblank_access::unlocked,
        .layer_object = true,
    });
    wait_for_exit();
}

void display_obj_line_limit_regular_size2_hblank_access() {
    object_limit_test(2);

    reg::dispcnt::write({
        .mode = 0,
        .oam_hblank_access = oam_hblank_access::unlocked,
        .layer_object = true,
    });
    wait_for_exit();
}
