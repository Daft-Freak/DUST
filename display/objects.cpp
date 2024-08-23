#include <gba/gba.hpp>

#include "../common.hpp"

#include "../assets/sprites.h"
#include "../assets/sprites_256.h"

using namespace gba;

static const int oam_addr = 0x07000000;

static void set_object(int index, objattr0 attr0, objattr1 attr1, objattr2 attr2) {
    objattr object {attr0, attr1, attr2};
    __agbabi_memcpy2(reinterpret_cast<void *>(oam_addr + index * sizeof(objattr8)), &object, sizeof(object));
}

static void set_object(int index, objattr0 attr0, objattr1_affine attr1, objattr2 attr2) {
    objattr_affine object{attr0, attr1, attr2};
    __agbabi_memcpy2(reinterpret_cast<void *>(oam_addr + index * sizeof(objattr8)), &object, sizeof(object));
}

static void set_affine_params(int index, uint16_t a, uint16_t b, uint16_t c, uint16_t d) {
    auto ptr = reinterpret_cast<uint16_t *>(oam_addr + index * 8 * 4) + 3;

    ptr[0] = a;
    ptr[4] = b;
    ptr[8] = c;
    ptr[12] = d;
}

static void setup_objects(obj_display mode, bool depth_8 = false, bool flip_h = false, bool flip_v = false, uint16_t pal_num = 0) {
    const int spacing = 10;
    uint16_t x = spacing, y = spacing;

    // tile mapping is different for 8bpp, so sheet is rearranged

    // size 0

    // 8x8
    set_object(0, objattr0 {
        .y = y,
        .style = mode,
        .bpp8 = depth_8,
        .shape = obj_shape::square
    }, objattr1 {
        .x = x,
        .hflip = flip_h,
        .vflip = flip_v,
        .size = 0
    }, objattr2 {
        .tile_id = 0,
        .palbank = pal_num
    });
    y += 8 + spacing;

    // 16x8
    set_object(1, objattr0 {
        .y = y,
        .style = mode,
        .bpp8 = depth_8,
        .shape = obj_shape::horizontal
    }, objattr1 {
        .x = x,
        .hflip = flip_h,
        .vflip = flip_v,
        .size = 0
    }, objattr2 {
        .tile_id = uint16_t(depth_8 ? 2 : 1),
        .palbank = pal_num
    });

    y += 8 + spacing;

    // 8x16
    set_object(2, objattr0 {
        .y = y,
        .style = mode,
        .bpp8 = depth_8,
        .shape = obj_shape::vertical
    }, objattr1 {
        .x = x,
        .hflip = flip_h,
        .vflip = flip_v,
        .size = 0
    }, objattr2 {
        .tile_id = 32,
        .palbank = pal_num
    });

    y = spacing;
    x += 16 + spacing;

    // size 1

    // 16x16
    set_object(3, objattr0 {
        .y = y,
        .style = mode,
        .bpp8 = depth_8,
        .shape = obj_shape::square
    }, objattr1 {
        .x = x,
        .hflip = flip_h,
        .vflip = flip_v,
        .size = 1
    }, objattr2 {
        .tile_id = uint16_t(depth_8 ? 6 : 3),
        .palbank = pal_num
    });

    y += 16 + spacing;

    // 32x8
    set_object(4, objattr0 {
        .y = y,
        .style = mode,
        .bpp8 = depth_8,
        .shape = obj_shape::horizontal
    }, objattr1 {
        .x = x,
        .hflip = flip_h,
        .vflip = flip_v,
        .size = 1
    }, objattr2 {
        .tile_id = uint16_t(depth_8 ? 10 : 5),
        .palbank = pal_num
    });

    y += 8 + spacing;

    // 8x32
    set_object(5, objattr0 {
        .y = y,
        .style = mode,
        .bpp8 = depth_8,
        .shape = obj_shape::vertical
    }, objattr1 {
        .x = x,
        .hflip = flip_h,
        .vflip = flip_v,
        .size = 1
    }, objattr2 {
        .tile_id = uint16_t(depth_8 ? 18 : 9),
        .palbank = pal_num
    });

    x += 32 + spacing;
    y = spacing;

    // size 2

    // 32x32
    set_object(6, objattr0 {
        .y = y,
        .style = mode,
        .bpp8 = depth_8,
        .shape = obj_shape::square
    }, objattr1 {
        .x = x,
        .hflip = flip_h,
        .vflip = flip_v,
        .size = 2
    }, objattr2 {
        .tile_id = uint16_t(depth_8 ? 20 : 10),
        .palbank = pal_num
    });

    y += 32 + spacing;

    // 32x16
    set_object(7, objattr0 {
        .y = y,
        .style = mode,
        .bpp8 = depth_8,
        .shape = obj_shape::horizontal
    }, objattr1 {
        .x = x,
        .hflip = flip_h,
        .vflip = flip_v,
        .size = 2
    }, objattr2 {
        .tile_id = uint16_t(depth_8 ? 42 : 14),
        .palbank = pal_num
    });

    y += 16 + spacing;

    // 16x32
    set_object(8, objattr0 {
        .y = y,
        .style = mode,
        .bpp8 = depth_8,
        .shape = obj_shape::vertical
    }, objattr1 {
        .x = x,
        .hflip = flip_h,
        .vflip = flip_v,
        .size = 2
    }, objattr2 {
        .tile_id = uint16_t(depth_8 ? 28 : 18),
        .palbank = pal_num
    });

    x += 32 + spacing;
    y = spacing;

    // size 3

    // 64x64
    set_object(9, objattr0 {
        .y = y,
        .style = mode,
        .bpp8 = depth_8,
        .shape = obj_shape::square
    }, objattr1 {
        .x = x,
        .hflip = flip_h,
        .vflip = flip_v,
        .size = 3
    }, objattr2 {
        .tile_id = uint16_t(depth_8 ? 264 : 20),
        .palbank = pal_num
    });

    y += 64 + spacing;

    // 64x32
    set_object(10, objattr0 {
        .y = y,
        .style = mode,
        .bpp8 = depth_8,
        .shape = obj_shape::horizontal
    }, objattr1 {
        .x = x,
        .hflip = flip_h,
        .vflip = flip_v,
        .size = 3
    }, objattr2 {
        .tile_id = uint16_t(depth_8 ? 144 : 140),
        .palbank = pal_num
    });

    // new column, no space left
    y = spacing;
    x += 64 + spacing;

    // 32x64
    set_object(11, objattr0 {
        .y = y,
        .style = mode,
        .bpp8 = depth_8,
        .shape = obj_shape::vertical
    }, objattr1 {
        .x = x,
        .hflip = flip_h,
        .vflip = flip_v,
        .size = 3
    }, objattr2 {
        .tile_id = uint16_t(depth_8 ? 280 : 28),
        .palbank = pal_num
    });

    // hide everything else off-screen
    for(int i = 12; i < 128; i++)
        set_object(i, objattr0 {.y = 160}, objattr1 {}, {});
}

// mostly copy/paste... but sets affine_index
static void setup_affine_objects(obj_display mode, bool depth_8 = false) {
    const int spacing = 10;
    uint16_t x = spacing, y = spacing;

    // tile mapping is diffent for 8bpp, so sheet is rearranged

    // size 0

    // 8x8
    set_object(0, objattr0 {
        .y = y,
        .style = mode,
        .bpp8 = depth_8,
        .shape = obj_shape::square
    }, objattr1_affine {
        .x = x,
        .affine_index = 0,
        .size = 0
    }, objattr2 {
        .tile_id = 0
    });
    y += 8 + spacing;

    // 16x8
    set_object(1, objattr0 {
        .y = y,
        .style = mode,
        .bpp8 = depth_8,
        .shape = obj_shape::horizontal
    }, objattr1_affine {
        .x = x,
        .affine_index = 1,
        .size = 0
    }, objattr2 {
        .tile_id = uint16_t(depth_8 ? 2 : 1),
    });

    y += 8 + spacing;

    // 8x16
    set_object(2, objattr0 {
        .y = y,
        .style = mode,
        .bpp8 = depth_8,
        .shape = obj_shape::vertical
    }, objattr1_affine {
        .x = x,
        .affine_index = 2,
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
        .bpp8 = depth_8,
        .shape = obj_shape::square
    }, objattr1_affine {
        .x = x,
        .affine_index = 3,
        .size = 1
    }, objattr2 {
        .tile_id = uint16_t(depth_8 ? 6 : 3),
    });

    y += 16 + spacing;

    // 32x8
    set_object(4, objattr0 {
        .y = y,
        .style = mode,
        .bpp8 = depth_8,
        .shape = obj_shape::horizontal
    }, objattr1_affine {
        .x = x,
        .affine_index = 4,
        .size = 1
    }, objattr2 {
        .tile_id = uint16_t(depth_8 ? 10 : 5),
    });

    y += 8 + spacing;

    // 8x32
    set_object(5, objattr0 {
        .y = y,
        .style = mode,
        .bpp8 = depth_8,
        .shape = obj_shape::vertical
    }, objattr1_affine {
        .x = x,
        .affine_index = 5,
        .size = 1
    }, objattr2 {
        .tile_id = uint16_t(depth_8 ? 18 : 9),
    });

    x += 32 + spacing;
    y = spacing;

    // size 2

    // 32x32
    set_object(6, objattr0 {
        .y = y,
        .style = mode,
        .bpp8 = depth_8,
        .shape = obj_shape::square
    }, objattr1_affine {
        .x = x,
        .affine_index = 6,
        .size = 2
    }, objattr2 {
        .tile_id = uint16_t(depth_8 ? 20 : 10),
    });

    y += 32 + spacing;

    // 32x16
    set_object(7, objattr0 {
        .y = y,
        .style = mode,
        .bpp8 = depth_8,
        .shape = obj_shape::horizontal
    }, objattr1_affine {
        .x = x,
        .affine_index = 7,
        .size = 2
    }, objattr2 {
        .tile_id = uint16_t(depth_8 ? 42 : 14),
    });

    y += 16 + spacing;

    // 16x32
    set_object(8, objattr0 {
        .y = y,
        .style = mode,
        .bpp8 = depth_8,
        .shape = obj_shape::vertical
    }, objattr1_affine {
        .x = x,
        .affine_index = 8,
        .size = 2
    }, objattr2 {
        .tile_id = uint16_t(depth_8 ? 28 : 18),
    });

    x += 32 + spacing;
    y = spacing;

    // size 3

    // 64x64
    set_object(9, objattr0 {
        .y = y,
        .style = mode,
        .bpp8 = depth_8,
        .shape = obj_shape::square
    }, objattr1_affine {
        .x = x,
        .affine_index = 9,
        .size = 3
    }, objattr2 {
        .tile_id = uint16_t(depth_8 ? 264 : 20),
    });

    y += 64 + spacing;

    // 64x32
    set_object(10, objattr0 {
        .y = y,
        .style = mode,
        .bpp8 = depth_8,
        .shape = obj_shape::horizontal
    }, objattr1_affine {
        .x = x,
        .affine_index = 10,
        .size = 3
    }, objattr2 {
        .tile_id = uint16_t(depth_8 ? 144 : 140),
    });

    // new column, no space left
    y = spacing;
    x += 64 + spacing;

    // 32x64
    set_object(11, objattr0 {
        .y = y,
        .style = mode,
        .bpp8 = depth_8,
        .shape = obj_shape::vertical
    }, objattr1_affine {
        .x = x,
        .affine_index = 11,
        .size = 3
    }, objattr2 {
        .tile_id = uint16_t(depth_8 ? 280 : 28),
    });

    // hide everything else off-screen
    for(int i = 12; i < 128; i++)
        set_object(i, objattr0 {.y = 160}, objattr1 {}, {});
}


void display_obj_regular() {
    // "regular" non-affine sprites, every size/shape

    palette_ram[0] = 0x4210;

    // load sprites
    __agbabi_memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    __agbabi_memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

    mmio::DISPCNT = {
        .video_mode = 0,
        .show_obj = true,
    };

    setup_objects(obj_display::normal);

    wait_for_exit();
}

void display_obj_affine() {
    // affine sprites, every size/shape

    palette_ram[0] = 0x4210;

    // load sprites
    __agbabi_memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    __agbabi_memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

    mmio::DISPCNT = {
        .video_mode = 0,
        .show_obj = true,
    };

    setup_objects(obj_display::affine);

    // init affine params
    set_affine_params(0, 1 << 8, 0, 0, 1 << 8);

    wait_for_exit();
}

void display_obj_affine_double() {
    // affine double-size sprites, every size/shape

    palette_ram[0] = 0x4210;

    // load sprites
    __agbabi_memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    __agbabi_memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

    mmio::DISPCNT = {
        .video_mode = 0,
        .show_obj = true,
    };

    setup_objects(obj_display::affine_double);

    // init affine params
    set_affine_params(0, 1 << 8, 0, 0, 1 << 8);

    wait_for_exit();
}

void display_obj_hidden() {
    // disabled sprites, every size/shape
    // ...which doesn't matter as they're hidden

    palette_ram[0] = 0x4210;

    // load sprites
    __agbabi_memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    __agbabi_memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

    mmio::DISPCNT = {
        .video_mode = 0,
        .show_obj = true,
    };

    setup_objects(obj_display::hidden);

    wait_for_exit();
}

void display_obj_regular_8bpp() {
    palette_ram[0] = 0x4210;

    // load sprites
    __agbabi_memcpy2(video_ram + 0x10000 / 2, sprites_256_tile_data, sizeof(sprites_256_tile_data));
    __agbabi_memcpy2(palette_ram + 0x200 / 2, sprites_256_palette, sizeof(sprites_256_palette));

    mmio::DISPCNT = {
        .video_mode = 0,
        .show_obj = true,
    };

    setup_objects(obj_display::normal, true);

    wait_for_exit();
}

void display_obj_affine_8bpp() {
    palette_ram[0] = 0x4210;

    // load sprites
    __agbabi_memcpy2(video_ram + 0x10000 / 2, sprites_256_tile_data, sizeof(sprites_256_tile_data));
    __agbabi_memcpy2(palette_ram + 0x200 / 2, sprites_256_palette, sizeof(sprites_256_palette));

    mmio::DISPCNT = {
        .video_mode = 0,
        .show_obj = true,
    };

    setup_objects(obj_display::affine, true);

    // init affine params
    set_affine_params(0, 1 << 8, 0, 0, 1 << 8);

    wait_for_exit();
}

void display_obj_regular_hflip() {
    // "regular" non-affine sprites, every size/shape, h-flipped

    palette_ram[0] = 0x4210;

    // load sprites
    __agbabi_memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    __agbabi_memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

    mmio::DISPCNT = {
        .video_mode = 0,
        .show_obj = true,
    };

    setup_objects(obj_display::normal, false, true);

    wait_for_exit();
}

void display_obj_regular_vflip() {
    // "regular" non-affine sprites, every size/shape, v-flipped

    palette_ram[0] = 0x4210;

    // load sprites
    __agbabi_memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    __agbabi_memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

    mmio::DISPCNT = {
        .video_mode = 0,
        .show_obj = true,
    };

    setup_objects(obj_display::normal, false, false, true);

    wait_for_exit();
}

void display_obj_regular_hvflip() {
    // "regular" non-affine sprites, every size/shape, h and v-flipped

    palette_ram[0] = 0x4210;

    // load sprites
    __agbabi_memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    __agbabi_memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

    mmio::DISPCNT = {
        .video_mode = 0,
        .show_obj = true,
    };

    setup_objects(obj_display::normal, false, true, true);

    wait_for_exit();
}

void display_obj_regular_8bpp_hflip() {
    // "regular" non-affine sprites, every size/shape, h-flipped

    palette_ram[0] = 0x4210;

    // load sprites
    __agbabi_memcpy2(video_ram + 0x10000 / 2, sprites_256_tile_data, sizeof(sprites_256_tile_data));
    __agbabi_memcpy2(palette_ram + 0x200 / 2, sprites_256_palette, sizeof(sprites_256_palette));

    mmio::DISPCNT = {
        .video_mode = 0,
        .show_obj = true,
    };

    setup_objects(obj_display::normal, true, true);

    wait_for_exit();
}

void display_obj_regular_8bpp_vflip() {
    // "regular" non-affine sprites, every size/shape, v-flipped

    palette_ram[0] = 0x4210;

    // load sprites
    __agbabi_memcpy2(video_ram + 0x10000 / 2, sprites_256_tile_data, sizeof(sprites_256_tile_data));
    __agbabi_memcpy2(palette_ram + 0x200 / 2, sprites_256_palette, sizeof(sprites_256_palette));

    mmio::DISPCNT = {
        .video_mode = 0,
        .show_obj = true,
    };

    setup_objects(obj_display::normal, true, false, true);

    wait_for_exit();
}

void display_obj_regular_8bpp_hvflip() {
    // "regular" non-affine sprites, every size/shape, h and v-flipped

    palette_ram[0] = 0x4210;

    // load sprites
    __agbabi_memcpy2(video_ram + 0x10000 / 2, sprites_256_tile_data, sizeof(sprites_256_tile_data));
    __agbabi_memcpy2(palette_ram + 0x200 / 2, sprites_256_palette, sizeof(sprites_256_palette));

    mmio::DISPCNT = {
        .video_mode = 0,
        .show_obj = true,
    };

    setup_objects(obj_display::normal, true, true, true);

    wait_for_exit();
}

static void setup_affine_params() {
    for(int i = 0; i < 12; i++) {
        float scale = (i / 3 + 1) / 3.0f;

        if(i & 1)
            scale = 1.0f / scale;

        bios::obj_affine_src input {
            .alpha = ((i + 1) * 10) << 8
        };

        input.sx.m_data = input.sy.m_data = scale * (1 << 8);

        bios::ObjAffineSet(&input, reinterpret_cast<fixed<short, 8> *>(oam_addr + i * 8 * 4 + 6), 1, 8);
    }
}

void display_obj_affine_rotscale() {
    palette_ram[0] = 0x4210;

    // load sprites
    __agbabi_memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    __agbabi_memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

    mmio::DISPCNT = {
        .video_mode = 0,
        .show_obj = true,
    };

    setup_affine_objects(obj_display::affine);
    setup_affine_params();

    wait_for_exit();
}

void display_obj_affine_8bpp_rotscale() {
    palette_ram[0] = 0x4210;

    // load sprites
    __agbabi_memcpy2(video_ram + 0x10000 / 2, sprites_256_tile_data, sizeof(sprites_256_tile_data));
    __agbabi_memcpy2(palette_ram + 0x200 / 2, sprites_256_palette, sizeof(sprites_256_palette));

    mmio::DISPCNT = {
        .video_mode = 0,
        .show_obj = true,
    };

    setup_affine_objects(obj_display::affine, true);
    setup_affine_params();

    wait_for_exit();
}

void display_obj_affine_double_rotscale() {
    palette_ram[0] = 0x4210;

    // load sprites
    __agbabi_memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    __agbabi_memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

    mmio::DISPCNT = {
        .video_mode = 0,
        .show_obj = true,
    };

    setup_affine_objects(obj_display::affine_double);
    setup_affine_params();

    wait_for_exit();
}

void display_obj_regular_4bpp_pal() {
    palette_ram[0] = 0x4210;

    // load sprites
    __agbabi_memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    __agbabi_memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

    palette_ram[256 + 17] = 0x000F;
    palette_ram[256 + 18] = 0x1084;
    palette_ram[256 + 19] = 0x0007;

    mmio::DISPCNT = {
        .video_mode = 0,
        .show_obj = true,
    };

    setup_objects(obj_display::normal, false, false, false, 1);

    wait_for_exit();
}

void display_obj_regular_1d() {
    // "regular" non-affine sprites, every size/shape

    palette_ram[0] = 0x4210;

    // load sprites
    __agbabi_memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    __agbabi_memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

    mmio::DISPCNT = {
        .video_mode = 0,
        .obj_vram_1d = true,
        .show_obj = true,
    };

    setup_objects(obj_display::normal);

    wait_for_exit();
}

void display_obj_regular_8bpp_1d() {
    palette_ram[0] = 0x4210;

    // load sprites
    __agbabi_memcpy2(video_ram + 0x10000 / 2, sprites_256_tile_data, sizeof(sprites_256_tile_data));
    __agbabi_memcpy2(palette_ram + 0x200 / 2, sprites_256_palette, sizeof(sprites_256_palette));

    mmio::DISPCNT = {
        .video_mode = 0,
        .obj_vram_1d = true,
        .show_obj = true,
    };

    setup_objects(obj_display::normal, true);

    wait_for_exit();
}

void display_obj_mixed() {
    // a mix of 4/8bit, regular/affine
    palette_ram[0] = 0x4210;

    // load both sets of sprites
    __agbabi_memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    __agbabi_memcpy2(video_ram + 0x14000 / 2, sprites_256_tile_data, sizeof(sprites_256_tile_data));
    // with a hybrid palette
    __agbabi_memcpy2(palette_ram + 0x200 / 2, sprites_256_palette, sizeof(sprites_256_palette));
    __agbabi_memcpy2(palette_ram + 0x200 / 2, sprites_palette, 4 * 2);

    setup_affine_params();

    mmio::DISPCNT = {
        .video_mode = 0,
        .show_obj = true,
    };

    const int spacing = 10;
    uint16_t x = spacing, y = spacing;

    // size 0 (4-bit regular)

    // 8x8
    set_object(0, objattr0 {
        .y = y,
        .style = obj_display::normal,
        .bpp8 = false,
        .shape = obj_shape::square
    }, objattr1 {
        .x = x,
        .size = 0
    }, objattr2 {
        .tile_id = 0,
        .palbank = 0
    });
    y += 8 + spacing;

    // 16x8
    set_object(1, objattr0 {
        .y = y,
        .style = obj_display::normal,
        .bpp8 = false,
        .shape = obj_shape::horizontal
    }, objattr1 {
        .x = x,
        .size = 0
    }, objattr2 {
        .tile_id = 1,
        .palbank = 1
    });

    y += 8 + spacing;

    // 8x16
    set_object(2, objattr0 {
        .y = y,
        .style = obj_display::normal,
        .bpp8 = false,
        .shape = obj_shape::vertical
    }, objattr1 {
        .x = x,
        .size = 0
    }, objattr2 {
        .tile_id = 32,
        .palbank = 2
    });

    y = spacing;
    x += 16 + spacing;

    // size 1 (8-bit regular)

    // 16x16
    set_object(3, objattr0 {
        .y = y,
        .style = obj_display::normal,
        .bpp8 = true,
        .shape = obj_shape::square
    }, objattr1 {
        .x = x,
        .size = 1
    }, objattr2 {
        .tile_id = 512 + 6,
    });

    y += 16 + spacing;

    // 32x8
    set_object(4, objattr0 {
        .y = y,
        .style = obj_display::normal,
        .bpp8 = true,
        .shape = obj_shape::horizontal
    }, objattr1 {
        .x = x,
        .size = 1
    }, objattr2 {
        .tile_id = 512 + 10,
    });

    y += 8 + spacing;

    // 8x32
    set_object(5, objattr0 {
        .y = y,
        .style = obj_display::normal,
        .bpp8 = true,
        .shape = obj_shape::vertical
    }, objattr1 {
        .x = x,
        .size = 1
    }, objattr2 {
        .tile_id = 512 + 18,
    });

    x += 32 + spacing;
    y = spacing;

    // size 2 (4-bit affine)

    // 32x32
    set_object(6, objattr0 {
        .y = y,
        .style = obj_display::affine,
        .bpp8 = false,
        .shape = obj_shape::square
    }, objattr1_affine {
        .x = x,
        .affine_index = 6,
        .size = 2
    }, objattr2 {
        .tile_id = 10,
        .palbank = 0
    });

    y += 32 + spacing;

    // 32x16
    set_object(7, objattr0 {
        .y = y,
        .style = obj_display::affine,
        .bpp8 = false,
        .shape = obj_shape::horizontal
    }, objattr1_affine {
        .x = x,
        .affine_index = 7,
        .size = 2
    }, objattr2 {
        .tile_id = 14,
        .palbank = 1
    });

    y += 16 + spacing;

    // 16x32
    set_object(8, objattr0 {
        .y = y,
        .style = obj_display::affine_double,
        .bpp8 = false,
        .shape = obj_shape::vertical
    }, objattr1_affine {
        .x = x,
        .affine_index = 8,
        .size = 2
    }, objattr2 {
        .tile_id = 18,
        .palbank = 2
    });

    x += 32 + spacing;
    y = spacing;

    // size 3 (8-bit affine)

    // 64x64
    set_object(9, objattr0 {
        .y = y,
        .style = obj_display::affine,
        .bpp8 = true,
        .shape = obj_shape::square
    }, objattr1_affine {
        .x = x,
        .affine_index = 9,
        .size = 3
    }, objattr2 {
        .tile_id = 512 + 264,
    });

    y += 64 + spacing;

    // 64x32
    set_object(10, objattr0 {
        .y = y,
        .style = obj_display::affine,
        .bpp8 = true,
        .shape = obj_shape::horizontal
    }, objattr1_affine {
        .x = x,
        .affine_index = 10,
        .size = 3
    }, objattr2 {
        .tile_id = 512 + 144,
    });

    // new column, no space left
    y = spacing;
    x += 64 + spacing;

    // 32x64
    set_object(11, objattr0 {
        .y = y,
        .style = obj_display::affine_double,
        .bpp8 = true,
        .shape = obj_shape::vertical
    }, objattr1_affine {
        .x = x,
        .affine_index = 11,
        .size = 3
    }, objattr2 {
        .tile_id = 512 + 280,
    });

    // hide everything else off-screen
    for(int i = 12; i < 128; i++)
        set_object(i, objattr0 {.y = 160}, objattr1 {}, {});

    wait_for_exit();
}

void display_obj_wrap_x() {
    palette_ram[0] = 0x4210;

    // load sprites
    __agbabi_memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    __agbabi_memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

    mmio::DISPCNT = {
        .video_mode = 0,
        .show_obj = true,
    };

    // right edge of the screen, no wrapping

    // 8x8
    set_object(0, objattr0 {
        .y = 0,
    }, objattr1 {
        .x = 240 - 4,
    }, objattr2 {
        .tile_id = 0
    });

    // 16x16
    set_object(1, objattr0 {
        .y = 8,
    }, objattr1 {
        .x = 240 - 8,
        .size = 1
    }, objattr2 {
        .tile_id = 3
    });

    // 32x32
    set_object(2, objattr0 {
        .y = 24,
    }, objattr1 {
        .x = 240 - 16,
        .size = 2
    }, objattr2 {
        .tile_id = 10
    });

    // 64x64
    set_object(3, objattr0 {
        .y = 56,
    }, objattr1 {
        .x = 240 - 32,
        .size = 3
    }, objattr2 {
        .tile_id = 20
    });

    // 128x128 (64x64 x2)
    set_object(4, objattr0 {
        .y = 0,
        .style = obj_display::affine_double
    }, objattr1 {
        .x = 240 - 64,
        .size = 3
    }, objattr2 {
        .tile_id = 20
    });

    // wrap back to left

    // 8x8
    set_object(5, objattr0 {
        .y = 152,
    }, objattr1 {
        .x = 512 - 4,
    }, objattr2 {
        .tile_id = 0
    });

    // 16x16
    set_object(6, objattr0 {
        .y = 136,
    }, objattr1 {
        .x = 512 - 8,
        .size = 1
    }, objattr2 {
        .tile_id = 3
    });

    // 32x32
    set_object(7, objattr0 {
        .y = 104,
    }, objattr1 {
        .x = 512 - 16,
        .size = 2
    }, objattr2 {
        .tile_id = 10
    });

    // 64x64
    set_object(8, objattr0 {
        .y = 40,
    }, objattr1 {
        .x = 512 - 32,
        .size = 3
    }, objattr2 {
        .tile_id = 20 
    });

    // 128x128 (64x64 x2)
    set_object(9, objattr0 {
        .y = 32,
        .style = obj_display::affine_double
    }, objattr1 {
        .x = 512 - 64,
        .size = 3
    }, objattr2 {
        .tile_id = 20
    });

    // hide everything else off-screen
    for(int i = 10; i < 128; i++)
        set_object(i, objattr0 {.y = 160}, objattr1 {}, {});

    // 2x scale
    set_affine_params(0, 0x80, 0, 0, 0x80);

    wait_for_exit();
}

void display_obj_wrap_y() {
    palette_ram[0] = 0x4210;

    // load sprites
    __agbabi_memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    __agbabi_memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

    mmio::DISPCNT = {
        .video_mode = 0,
        .show_obj = true,
    };

    // bottom edge of the screen, no wrapping

    // 8x8
    set_object(0, objattr0 {
        .y = 160 - 4,
    }, objattr1 {
        .x = 0,
    }, objattr2 {
        .tile_id = 0
    });

    // 16x16
    set_object(1, objattr0 {
        .y = 160 - 8,
    }, objattr1 {
        .x = 8,
        .size = 1
    }, objattr2 {
        .tile_id = 3
    });

    // 32x32
    set_object(2, objattr0 {
        .y = 160 - 16,
    }, objattr1 {
        .x = 24,
        .size = 2
    }, objattr2 {
        .tile_id = 10
    });

    // 64x64
    set_object(3, objattr0 {
        .y = 160 - 32,
    }, objattr1 {
        .x = 56,
        .size = 3
    }, objattr2 {
        .tile_id = 20
    });

    // 128x128 (64x64 x2)
    set_object(4, objattr0 {
        .y = 160 - 64,
        .style = obj_display::affine_double
    }, objattr1 {
        .x = 120,
        .size = 3
    }, objattr2 {
        .tile_id = 20
    });

    // wrap back to top

    // 8x8
    set_object(5, objattr0 {
        .y = 256 - 4,
    }, objattr1 {
        .x = 232,
    }, objattr2 {
        .tile_id = 0
    });

    // 16x16
    set_object(6, objattr0 {
        .y = 256 - 8,
    }, objattr1 {
        .x = 216,
        .size = 1
    }, objattr2 {
        .tile_id = 3
    });

    // 32x32
    set_object(7, objattr0 {
        .y = 256 - 16,
    }, objattr1 {
        .x = 184,
        .size = 2
    }, objattr2 {
        .tile_id = 10
    });

    // 64x64
    set_object(8, objattr0 {
        .y = 256 - 32,
    }, objattr1 {
        .x = 120,
        .size = 3
    }, objattr2 {
        .tile_id = 20 
    });

    // 128x128 (64x64 x2)
    set_object(9, objattr0 {
        .y = 256 - 64,
        .style = obj_display::affine_double
    }, objattr1 {
        .x = 504, // -8
        .size = 3
    }, objattr2 {
        .tile_id = 20
    });

    // hide everything else off-screen
    for(int i = 10; i < 128; i++)
        set_object(i, objattr0 {.y = 160}, objattr1 {}, {});

    // 2x scale
    set_affine_params(0, 0x80, 0, 0, 0x80);

    wait_for_exit();
}

void display_obj_wrap_y_bug() {
    palette_ram[0] = 0x4210;

    // load sprites
    __agbabi_memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    __agbabi_memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

    mmio::DISPCNT = {
        .video_mode = 0,
        .show_obj = true,
    };

    // 128x128 (64x64 x2)
    set_object(0, objattr0 {
        .y = 128,
        .style = obj_display::affine_double
    }, objattr1 {
        .x = 120,
        .size = 3
    }, objattr2 {
        .tile_id = 20
    });

    // should have 31 pixels visivle at the bottom and wrap around to the top
    // instead only shows at the top
    set_object(1, objattr0 {
        .y = 129,
        .style = obj_display::affine_double
    }, objattr1 {
        .x = 504, // -8
        .size = 3
    }, objattr2 {
        .tile_id = 20
    });

    // hide everything else off-screen
    for(int i = 2; i < 128; i++)
        set_object(i, objattr0 {.y = 160}, objattr1 {}, {});

    // 2x scale
    set_affine_params(0, 0x80, 0, 0, 0x80);

    wait_for_exit();
}

void display_obj_priority() {
    palette_ram[0] = 0x4210;

    // load sprites
    __agbabi_memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    __agbabi_memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

    mmio::DISPCNT = {
        .video_mode = 0,
        .show_obj = true,
    };

    // both priority 0, obj 0 should be on top of obj 1

    // 8x8
    set_object(0, objattr0 {
        .y = 12,
    }, objattr1 {
        .x = 12,
    }, objattr2 {
        .tile_id = 0
    });

    // 16x16
    set_object(1, objattr0 {
        .y = 8,
    }, objattr1 {
        .x = 8,
        .size = 1
    }, objattr2 {
        .tile_id = 3
    });

    // obj 2 should be on top of obj 3, but it has a lower priority so it isn't

    // 64x64
    set_object(2, objattr0 {
        .y = 8,
    }, objattr1 {
        .x = 32,
        .size = 3
    }, objattr2 {
        .tile_id = 20,
        .priority = 1
    });

    // 32x32
    set_object(3, objattr0 {
        .y = 24,
    }, objattr1 {
        .x = 48,
        .size = 2
    }, objattr2 {
        .tile_id = 10
    });

    // hide everything else off-screen
    for(int i = 4; i < 128; i++)
        set_object(i, objattr0 {.y = 160}, objattr1 {}, {});

    wait_for_exit();
}

void display_obj_priority_bug() {
    palette_ram[0] = 0x4210;

    // load sprites
    __agbabi_memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    __agbabi_memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

    mmio::DISPCNT = {
        .video_mode = 0,
        .show_bg0 = true,
        .show_obj = true,
    };

    // add layer at priority 0
    mmio::BG0CNT = {
        .screenblock = 2
    };

    __agbabi_wordset4(video_ram + 0x0800, 0x800, 0);
    __agbabi_wordset4(video_ram, 64, 0x11111111);

    palette_ram[1] = 0x4210;

    // both priority 0, obj 0 should be on top of obj 1

    // 8x8
    set_object(0, objattr0 {
        .y = 12,
    }, objattr1 {
        .x = 12,
    }, objattr2 {
        .tile_id = 0
    });

    // 16x16
    set_object(1, objattr0 {
        .y = 8,
    }, objattr1 {
        .x = 8,
        .size = 1
    }, objattr2 {
        .tile_id = 3
    });

    // obj 2 should be on top of obj 3, but it has a lower priority so it isn't
    // ... and as there's a layer in the middle there's a bug

    // 64x64
    set_object(2, objattr0 {
        .y = 8,
    }, objattr1 {
        .x = 32,
        .size = 3
    }, objattr2 {
        .tile_id = 20,
        .priority = 1
    });

    // 32x32
    set_object(3, objattr0 {
        .y = 24,
    }, objattr1 {
        .x = 48,
        .size = 2
    }, objattr2 {
        .tile_id = 10
    });

    // hide everything else off-screen
    for(int i = 4; i < 128; i++)
        set_object(i, objattr0 {.y = 160}, objattr1 {}, {});

    wait_for_exit();
}

void display_obj_bmp_char_base() {
    palette_ram[0] = 0x4210;

    // load sprites
    // first 16 is used by background so these can't be used
    __agbabi_memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    // this should be okay
    __agbabi_memcpy2(video_ram + 0x14000 / 2, sprites_tile_data, sizeof(sprites_tile_data));

    __agbabi_memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

    mmio::DISPCNT = {
        .video_mode = 3, // bitmap
        .show_obj = true,
    };

    // this sprite won't be shown
    set_object(0, objattr0 {
        .y = 16,
    }, objattr1 {
        .x = 16,
        .size = 3
    }, objattr2 {
        .tile_id = 20
    });

    // this one will
    set_object(1, objattr0 {
        .y = 16,
    }, objattr1 {
        .x = 64 + 32,
        .size = 3
    }, objattr2 {
        .tile_id = 20 + 512
    });

    // hide everything else off-screen
    for(int i = 2; i < 128; i++)
        set_object(i, objattr0 {.y = 160}, objattr1 {}, {});

    wait_for_exit();
}

static void object_limit_test(uint16_t size, obj_display mode = obj_display::normal, int x_off = 0, bool partial_hidden = false) {
    palette_ram[0] = 0x4210;

    // load sprites
    __agbabi_memcpy2(video_ram + 0x10000 / 2, sprites_tile_data, sizeof(sprites_tile_data));
    __agbabi_memcpy2(palette_ram + 0x200 / 2, sprites_palette, sizeof(sprites_palette));

    mmio::DISPCNT = {
        .video_mode = 0,
        .show_obj = true,
    };

    const uint16_t tiles[]{0, 3, 10, 20};

    int y_mask = (4 << size) - 1;

    for(int i = 0; i < 128; i++) {
        set_object(i, objattr0 {
            .y = uint16_t(i & y_mask),
            .style = partial_hidden && i < 64 ? obj_display::hidden : mode
        }, objattr1 {
            .x = uint16_t(i + x_off),
            .size = size
        }, objattr2 {
            .tile_id = tiles[size]
        });
    }

    set_affine_params(0, mode == obj_display::affine_double ? 0x80 : 0x100, 0, 0, mode == obj_display::affine_double ? 0x80 : 0x100);
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
    object_limit_test(0, obj_display::affine);
    wait_for_exit();
}

void display_obj_line_limit_affine_size1() {
    object_limit_test(1, obj_display::affine);
    wait_for_exit();
}

void display_obj_line_limit_affine_size2() {
    object_limit_test(2, obj_display::affine);
    wait_for_exit();
}

void display_obj_line_limit_affine_size3() {
    object_limit_test(3, obj_display::affine);
    wait_for_exit();
}

void display_obj_line_limit_affine_double_size0() {
    object_limit_test(0, obj_display::affine_double);
    wait_for_exit();
}

void display_obj_line_limit_affine_double_size1() {
    object_limit_test(1, obj_display::affine_double);
    wait_for_exit();
}

void display_obj_line_limit_affine_double_size2() {
    object_limit_test(2, obj_display::affine_double);
    wait_for_exit();
}

void display_obj_line_limit_affine_double_size3() {
    object_limit_test(3, obj_display::affine_double);
    wait_for_exit();
}

// not testing every variant because there are already too many tests here...
void display_obj_line_limit_regular_offscreen() {
    // move some of the sprites off the screen
    object_limit_test(2, obj_display::normal, -64);
    wait_for_exit();
}

void display_obj_line_limit_regular_hidden() {
    // first half set to hidden
    object_limit_test(2, obj_display::normal, 0, true);
    wait_for_exit();
}

void display_obj_line_limit_regular_size0_hblank_access() {
    object_limit_test(0);

    mmio::DISPCNT = {
        .video_mode = 0,
        .hblank_oam_free = true,
        .show_obj = true,
    };
    wait_for_exit();
}

void display_obj_line_limit_regular_size2_hblank_access() {
    object_limit_test(2);

    mmio::DISPCNT = {
        .video_mode = 0,
        .hblank_oam_free = true,
        .show_obj = true,
    };
    wait_for_exit();
}
