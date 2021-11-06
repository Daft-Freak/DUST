#include <gba/gba.hpp>

#include "../common.hpp"

#include "../assets/sprites.h"
#include "../assets/sprites_256.h"

using namespace gba;

inline auto oam_reg = reinterpret_cast<object_regular *>(0x07000000);

static void set_object(int index, object::attr0 attr0, object::attr1_regular attr1, object::attr2 attr2) {
    oam_reg[index].attr0 = attr0;
    oam_reg[index].attr1 = attr1;
    oam_reg[index].attr2 = attr2;
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
