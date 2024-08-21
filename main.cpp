
#include <gba/gba.hpp>
#include <gba/ext/agbabi/agbabi.hpp>

#include "assets/8x8font.h"
#include "common.hpp"

using namespace gba;

keystate key_state;

// hack to drop unwind code, removing ~6k from the output
// seems to be getting pulled in from irq.s...
extern "C" void __aeabi_unwind_cpp_pr0(){};

static void load_font() {
    palette_ram[0] = 0;
    palette_ram[1] = 0x7FFF;

    static const bios::bit_un_pack unpack {
        .src_len = sizeof(font8x8_char_data_1bpp),
        .src_bpp = 1,
        .dst_bpp = 4
    };
    bios::BitUnPack(font8x8_char_data_1bpp, video_ram, &unpack);
}

struct test_info {
    const char *name;
    void (*func)();
};

static void display_layer0_test_list();
static void display_layer2_test_list();
static void display_object_test_list();
static void display_window_test_list();
static void display_mosaic_test_list();
static void display_blend_test_list();

static void dma_test_list();

static const std::array tests {
    test_info {"Display/No layers" , display_layer_none},
    test_info {"Display/CGB mode" , display_cgb_mode},
    test_info {"Display/Forced blank", display_forced_blank},
    test_info {"Display/Default priority", display_priority_default},
    test_info {"Display/STAT flags", display_stat_flags},
    test_info {"Display/Layer 0            >", display_layer0_test_list},
    test_info {"Display/Layer 2            >", display_layer2_test_list},
    test_info {"Display/Objects            >", display_object_test_list},
    test_info {"Display/Window             >", display_window_test_list},
    test_info {"Display/Mosaic             >", display_mosaic_test_list},
    test_info {"Display/Blend              >", display_blend_test_list},

    test_info {"DMA                        >", dma_test_list},
};

static const std::array display_layer0_tests {
    test_info {"Display/Layer 0 char base", display_layer0_char_base},
    test_info {"Display/Layer 0 4bpp", display_layer0_4bpp},
    test_info {"Display/Layer 0 8bpp", display_layer0_8bpp},
    test_info {"Display/Layer 0 size 0", display_layer0_size0},
    test_info {"Display/Layer 0 size 1", display_layer0_size1},
    test_info {"Display/Layer 0 size 2", display_layer0_size2},
    test_info {"Display/Layer 0 size 3", display_layer0_size3},
    test_info {"Display/Layer 0 flip", display_layer0_flip},
    test_info {"Display/Layer 0 mode 1", display_layer0_mode1},
    test_info {"Display/Layer 0 mode 2", display_layer0_mode2},
    test_info {"Display/Layer 0 mode 3", display_layer0_mode3},
    test_info {"Display/Layer 0 mode 4", display_layer0_mode4},
    test_info {"Display/Layer 0 mode 5", display_layer0_mode5},
    test_info {"Display/Layer 0 cb invalid", display_layer0_char_base_invalid},
    test_info {"Display/Layer 0 sb invalid", display_layer0_screen_base_invalid},
};

static const std::array display_layer2_tests {
    test_info {"Display/Layer 2 mode 0", display_layer2_mode0},
    test_info {"Display/Layer 2 mode 1", display_layer2_mode1},
    test_info {"Display/Layer 2 m1 char base", display_layer2_mode1_char_base},
    test_info {"Display/Layer 2 m1 wrap", display_layer2_mode1_wrap},
    test_info {"Display/Layer 2 m1 size 1", display_layer2_mode1_size1},
    test_info {"Display/Layer 2 m1 size 2", display_layer2_mode1_size2},
    test_info {"Display/Layer 2 m1 size 3", display_layer2_mode1_size3},
    test_info {"Display/Layer 2 m1 rotscale", display_layer2_mode1_rotscale},
    test_info {"Display/Layer 2 mode 2", display_layer2_mode2},
    test_info {"Display/Layer 2 mode 3", display_layer2_mode3},
    test_info {"Display/Layer 2 m3 rotscale", display_layer2_mode3_rotscale},
    test_info {"Display/Layer 2 mode 4", display_layer2_mode4},
    test_info {"Display/Layer 2 m4 pageflip", display_layer2_mode4_pageflip},
    test_info {"Display/Layer 2 m4 rotscale", display_layer2_mode4_rotscale},
    test_info {"Display/Layer 2 mode 5", display_layer2_mode5},
    test_info {"Display/Layer 2 m5 pageflip", display_layer2_mode5_pageflip},
    test_info {"Display/Layer 2 m5 rotscale", display_layer2_mode5_rotscale},
};

static const std::array display_object_tests {
    test_info {"Display/Objects regular", display_obj_regular},
    test_info {"Display/Objects affine", display_obj_affine},
    test_info {"Display/Objects affine 2x", display_obj_affine_double},
    test_info {"Display/Objects hidden", display_obj_hidden},
    test_info {"Display/Objects regular 8bpp", display_obj_regular_8bpp},
    test_info {"Display/Objects affine 8bpp", display_obj_affine_8bpp},
    test_info {"Display/Objects reg hflip", display_obj_regular_hflip},
    test_info {"Display/Objects reg vflip", display_obj_regular_vflip},
    test_info {"Display/Objects reg hvflip", display_obj_regular_hvflip},
    test_info {"Display/Objects aff rotscale", display_obj_affine_rotscale},
    test_info {"Display/Objects aff dbl rtscl", display_obj_affine_double_rotscale},
    test_info {"Display/Objects reg 4bpp pal", display_obj_regular_4bpp_pal},
    test_info {"Display/Objects reg 1d", display_obj_regular_1d},
    test_info {"Display/Objects reg 8bpp 1d", display_obj_regular_8bpp_1d},
    test_info {"Display/Objects wrap x", display_obj_wrap_x},
    test_info {"Display/Objects wrap y", display_obj_wrap_y},
    test_info {"Display/Objects wrap y bug", display_obj_wrap_y_bug},
    test_info {"Display/Objects priority", display_obj_priority},
    test_info {"Display/Objects priority bug", display_obj_priority_bug},
    test_info {"Display/Objects bmp char base", display_obj_bmp_char_base},
    test_info {"Display/Objects limit reg s0", display_obj_line_limit_regular_size0},
    test_info {"Display/Objects limit reg s1", display_obj_line_limit_regular_size1},
    test_info {"Display/Objects limit reg s2", display_obj_line_limit_regular_size2},
    test_info {"Display/Objects limit reg s3", display_obj_line_limit_regular_size3},
    test_info {"Display/Objects limit aff s0", display_obj_line_limit_affine_size0},
    test_info {"Display/Objects limit aff s1", display_obj_line_limit_affine_size1},
    test_info {"Display/Objects limit aff s2", display_obj_line_limit_affine_size2},
    test_info {"Display/Objects limit aff s3", display_obj_line_limit_affine_size3},
    test_info {"Display/Objects limit dbl s0", display_obj_line_limit_affine_double_size0},
    test_info {"Display/Objects limit dbl s1", display_obj_line_limit_affine_double_size1},
    test_info {"Display/Objects limit dbl s2", display_obj_line_limit_affine_double_size2},
    test_info {"Display/Objects limit dbl s3", display_obj_line_limit_affine_double_size3},
    test_info {"Display/Objects limit offscr", display_obj_line_limit_regular_offscreen},
    test_info {"Display/Objects limit hidden", display_obj_line_limit_regular_hidden},
    test_info {"Display/Objects limit h acc0", display_obj_line_limit_regular_size0_hblank_access},
    test_info {"Display/Objects limit h acc2", display_obj_line_limit_regular_size2_hblank_access},
};

static const std::array display_window_tests {
    test_info {"Display/Window win0 bg", display_window_win0_bg},
    test_info {"Display/Window win1 bg", display_window_win1_bg},
    test_info {"Display/Window obj win bg", display_window_obj_win_bg},
    test_info {"Display/Window priority", display_window_priority},
    test_info {"Display/Window objects", display_window_objects},
    test_info {"Display/Window win0 bg aff", display_window_win0_bg_affine},
    test_info {"Display/Window hblank irq", display_window_hblank_irq},
    test_info {"Display/Window hblank dma", display_window_hblank_dma},
    test_info {"Display/Window invalid x1", display_window_invalid_x1},
    test_info {"Display/Window invalid x2", display_window_invalid_x2},
    test_info {"Display/Window invalid y1", display_window_invalid_y1},
    test_info {"Display/Window invalid y2", display_window_invalid_y2},
    test_info {"Display/Window inv y2 no dis", display_window_invalid_y2_no_disable},
};

static const std::array display_mosaic_tests {
    test_info {"Display/Mosaic mode 0 0", display_mosaic_mode0_0},
    test_info {"Display/Mosaic mode 0 1x1", display_mosaic_mode0_1_1},
    test_info {"Display/Mosaic mode 0 4x4", display_mosaic_mode0_4_4},
    test_info {"Display/Mosaic mode 0 9x9", display_mosaic_mode0_9_9},
    test_info {"Display/Mosaic mode 0 15x15", display_mosaic_mode0_15_15},
    test_info {"Display/Mosaic mode 0 0x15", display_mosaic_mode0_0_15},
    test_info {"Display/Mosaic mode 0 15x0", display_mosaic_mode0_15_0},
    test_info {"Display/Mosaic m0 4x4 scroll", display_mosaic_mode0_4_4_scroll},
    test_info {"Display/Mosaic mode 2 0", display_mosaic_mode2_0},
    test_info {"Display/Mosaic mode 2 1x1", display_mosaic_mode2_1_1},
    test_info {"Display/Mosaic mode 2 4x4", display_mosaic_mode2_4_4},
    test_info {"Display/Mosaic mode 2 9x9", display_mosaic_mode2_9_9},
    test_info {"Display/Mosaic mode 2 15x15", display_mosaic_mode2_15_15},
    test_info {"Display/Mosaic mode 2 0x15", display_mosaic_mode2_0_15},
    test_info {"Display/Mosaic mode 2 15x0", display_mosaic_mode2_15_0},
    test_info {"Display/Mosaic m2 1x1 rtscl", display_mosaic_mode2_1_1_rotscale},
    test_info {"Display/Mosaic m2 4x4 rtscl", display_mosaic_mode2_4_4_rotscale},
    test_info {"Display/Mosaic m2 9x9 rtscl", display_mosaic_mode2_9_9_rotscale},
    test_info {"Display/Mosaic m2 15x15 rtscl", display_mosaic_mode2_15_15_rotscale},
    test_info {"Display/Mosaic m2 0x15 rtscl", display_mosaic_mode2_0_15_rotscale},
    test_info {"Display/Mosaic m2 15x0 rtscl", display_mosaic_mode2_15_0_rotscale},
    test_info {"Display/Mosaic mode 3 0", display_mosaic_mode3_0},
    test_info {"Display/Mosaic mode 3 1x1", display_mosaic_mode3_1_1},
    test_info {"Display/Mosaic mode 3 15x15", display_mosaic_mode3_15_15},
    test_info {"Display/Mosaic mode 3 0x15", display_mosaic_mode3_0_15},
    test_info {"Display/Mosaic mode 3 15x0", display_mosaic_mode3_15_0},
    test_info {"Display/Mosaic mode 4 0", display_mosaic_mode4_0},
    test_info {"Display/Mosaic mode 4 1x1", display_mosaic_mode4_1_1},
    test_info {"Display/Mosaic mode 4 15x15", display_mosaic_mode4_15_15},
    test_info {"Display/Mosaic mode 4 0x15", display_mosaic_mode4_0_15},
    test_info {"Display/Mosaic mode 4 15x0", display_mosaic_mode4_15_0},
    test_info {"Display/Mosaic mode 5 0", display_mosaic_mode5_0},
    test_info {"Display/Mosaic mode 5 1x1", display_mosaic_mode5_1_1},
    test_info {"Display/Mosaic mode 5 15x15", display_mosaic_mode5_15_15},
    test_info {"Display/Mosaic mode 5 0x15", display_mosaic_mode5_0_15},
    test_info {"Display/Mosaic mode 5 15x0", display_mosaic_mode5_15_0},
    test_info {"Display/Mosaic obj reg 1x1", display_mosaic_objects_regular_1_1},
    test_info {"Display/Mosaic obj reg 4x4", display_mosaic_objects_regular_4_4},
    test_info {"Display/Mosaic obj reg 9x9", display_mosaic_objects_regular_9_9},
    test_info {"Display/Mosaic obj reg 15x15", display_mosaic_objects_regular_15_15},
    test_info {"Display/Mosaic obj reg 0x15", display_mosaic_objects_regular_0_15},
    test_info {"Display/Mosaic obj reg 15x0", display_mosaic_objects_regular_15_0},
    test_info {"Display/Mosaic obj aff 1x1", display_mosaic_objects_affine_1_1},
    test_info {"Display/Mosaic obj aff 4x4", display_mosaic_objects_affine_4_4},
    test_info {"Display/Mosaic obj aff 9x9", display_mosaic_objects_affine_9_9},
    test_info {"Display/Mosaic obj aff 15x15", display_mosaic_objects_affine_15_15},
    test_info {"Display/Mosaic obj aff 0x15", display_mosaic_objects_affine_0_15},
    test_info {"Display/Mosaic obj aff 15x0", display_mosaic_objects_affine_15_0},
    test_info {"Display/Mosaic window", display_mosaic_window},
};

static const std::array display_blend_tests {
    test_info {"Display/Blend mode 0", display_blend_mode0},
    test_info {"Display/Blend mode 1", display_blend_mode1},
    test_info {"Display/Blend m1 all", display_blend_mode1_all},
    test_info {"Display/Blend m1 add",display_blend_mode1_add},
    test_info {"Display/Blend m1 rev", display_blend_mode1_reversed},
    test_info {"Display/Blend m1 no second", display_blend_mode1_no_second},
    test_info {"Display/Blend m1 between", display_blend_mode1_between},
    test_info {"Display/Blend m1 objects", display_blend_mode1_objects},
    test_info {"Display/Blend mode 2", display_blend_mode2},
    test_info {"Display/Blend m2 backdrop", display_blend_mode2_backdrop},
    test_info {"Display/Blend mode 3", display_blend_mode3},
    test_info {"Display/Blend m3 backdrop", display_blend_mode3_backdrop},
    test_info {"Display/Blend window", display_blend_window},
    test_info {"Display/Blend obj trans", display_blend_object_trans},
    test_info {"Display/Blend obj trans en", display_blend_object_trans_enable},
    test_info {"Display/Blend obj trans prio", display_blend_object_trans_priority},
};

static const std::array dma_tests {
    test_info {"DMA/Delay", dma_delay},
    test_info {"DMA/Priority", dma_priority},
    test_info {"DMA/Priority reverse", dma_priority_reverse},
    test_info {"DMA/Priority hblank", dma_priority_hblank},
    test_info {"DMA/Timer", dma_timer},
    test_info {"DMA/Timer multi", dma_timer_multi},
};

static void init_display() {
    mmio::DISPCNT = {
        .video_mode = 0,
        .show_bg0 = true,
    };

    mmio::BG0CNT = {
        .charblock = 0,
        .screenblock = 2,
        .size = 2 // 32x64
    };

    mmio::BG0HOFS.reset();
    mmio::BG0VOFS.reset();

    mmio::BLDCNT.reset();

    load_font();
}

static void draw_menu(const auto &tests) {
    clear_text();

    // menu
    write_text(0, 1, "Dafts Useful(?) Suite of Tests");

    int y = 3;
    for(const auto &test : tests)
        write_text(2, y++, test.name);
}

static void test_list(const auto &tests) {
    int item = 0;

    draw_menu(tests);

    while(true) {
        key_state = *gba::mmio::KEYINPUT;

        bios::VBlankIntrWait();

        if(key_state.pressed(key::up) || key_state.pressed(key::down)) {
            write_text(1, item + 3, " ");

            int num_tests = std::size(tests);
            item = std::max(0, std::min(num_tests - 1, item - key_state.yaxis()));
        }

        if(key_state.released(key::a)) {
            tests[item].func();
            init_display(); // reload after test
            draw_menu(tests);
        }
        // return to previous menu
        else if(key_state.released(key::b))
            return;

        // scroll to current item
        mmio::BG0VOFS = std::max(0, (item + 3) * 8 - 80);

        write_text(1, item + 3, ">");
    }
}

// sub menus
static void display_layer0_test_list() {
    test_list(display_layer0_tests);
}

static void display_layer2_test_list() {
    test_list(display_layer2_tests);
}

static void display_object_test_list() {
    test_list(display_object_tests);
}

static void display_window_test_list() {
    test_list(display_window_tests);
}

static void display_mosaic_test_list() {
    test_list(display_mosaic_tests);
}

static void display_blend_test_list() {
    test_list(display_blend_tests);
}

static void dma_test_list() {
    test_list(dma_tests);
}

int main(int argc, char * argv[]) {
    mmio::IRQ_HANDLER = agbabi::irq_empty;

    mmio::DISPSTAT = {.irq_vblank = true};
    mmio::IE = {.vblank = true};
    mmio::IME = true;

    init_display();

#ifdef SINGLE_TEST
    while(true)
        SINGLE_TEST();
#else
    while(true)
        test_list(tests);
#endif

    __builtin_unreachable();
}
