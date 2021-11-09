
#include <gba/gba.hpp>
#include <gba/ext/agbabi.hpp>

#include "assets/8x8font.h"
#include "common.hpp"

using namespace gba;

io::keypad_manager keypad_man;

// hack to drop unwind code, removing ~6k from the output
// seems to be getting pulled in from irq.s...
extern "C" void __aeabi_unwind_cpp_pr0(){};

static void load_font() {
    palette_ram[0] = 0;
    palette_ram[1] = 0x7FFF;

    static const bios::bit_un_pack_input unpack {
        .size = sizeof(font8x8_char_data_1bpp),
        .source_width = bios::un_pack_bits::_1,
        .destination_width = bios::un_pack_bits::_4
    };
    bios::bit_un_pack(font8x8_char_data_1bpp, video_ram, &unpack);
}

struct test_info {
    const char *name;
    void (*func)();
};

static void display_layer0_test_list();
static void display_layer2_test_list();
static void display_object_test_list();

static const std::array tests {
    test_info {"Display/No layers" , display_layer_none},
    test_info {"Display/CGB mode" , display_cgb_mode},
    test_info {"Display/Forced blank", display_forced_blank},
    test_info {"Display/Default priority", display_priority_default},
    test_info {"Display/STAT flags", display_stat_flags},
    test_info {"Display/Layer 0            >", display_layer0_test_list},
    test_info {"Display/Layer 2            >", display_layer2_test_list},
    test_info {"Display/Objects            >", display_object_test_list},
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

static void init_display() {
    reg::dispcnt::write({
        .mode = 0,
        .layer_background_0 = true,
    });

    reg::bg0cnt::write(background_control {
        .character_base_block = 0,
        .color_depth = color_depth::bpp_4,
        .screen_base_block = 2,
        .screen_size = screen_size::regular_32x64
    });

    reg::bg0hofs::write(0);
    reg::bg0vofs::write(0);

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
        keypad_man.poll();

        bios::vblank_intr_wait();

        if(keypad_man.any_switched_down(key_mask::make(key::up, key::down))) {
            write_text(1, item + 3, " ");

            int32_t num_tests = std::size(tests);
            item = std::max(0l, std::min(num_tests - 1, item + keypad_man.axis_y()));
        }

        if(keypad_man.switched_up(key::button_a)) {
            tests[item].func();
            init_display(); // reload after test
            draw_menu(tests);
        }
        // return to previous menu
        else if(keypad_man.switched_up(gba::key::button_b))
            return;

        // scroll to current item
        reg::bg0vofs::write(std::max(0, (item + 3) * 8 - 80));

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

int main(int argc, char * argv[]) {
    reg::dispstat::write({.vblank_irq = true});
    reg::ie::write({.vblank = true});
    reg::ime::emplace(true);

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
