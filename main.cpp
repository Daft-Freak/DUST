
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
    palRAM[0] = 0;
    palRAM[1] = 0x7FFF;

    static const bios::bit_un_pack_input unpack {
        .size = sizeof(font8x8_char_data_1bpp),
        .source_width = bios::un_pack_bits::_1,
        .destination_width = bios::un_pack_bits::_4
    };
    bios::bit_un_pack(font8x8_char_data_1bpp, videoRAM, &unpack);
}

struct test_info {
    const char *name;
    void (*func)();
};

static test_info tests[] {
    {"Display/No layers" , display_layer_none},
    {"Display/CGB mode" , display_cgb_mode},
    {"Display/Forced blank", display_forced_blank},
    {"Display/Default priority", display_priority_default},
    {"Display/Layer 0 char base", display_layer0_char_base},
    {"Display/Layer 0 4bpp", display_layer0_4bpp},
    {"Display/Layer 0 8bpp", display_layer0_8bpp},
    {"Display/Layer 0 size 0", display_layer0_size0},
    {"Display/Layer 0 size 1", display_layer0_size1},
    {"Display/Layer 0 size 2", display_layer0_size2},
    {"Display/Layer 0 size 3", display_layer0_size3},
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
        .screen_size = screen_size::regular_32x32
    });

    reg::bg0hofs::write(0);
    reg::bg0vofs::write(0);

    load_font();

#ifndef SINGLE_TEST
    clear_text();

    // menu
    write_text(0, 1, "Dafts Useful(?) Suite of Tests");

    int y = 3;
    for(auto &test : tests)
        write_text(2, y++, test.name);

#endif
}

int main(int argc, char * argv[]) {
    reg::dispstat::write({.vblank_irq = true});
    reg::ie::write({.vblank = true});
    reg::ime::emplace(true);

    init_display();

#ifdef SINGLE_TEST
    while(true) {
        SINGLE_TEST();
    }
#else
    int item = 0;

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
        }

        write_text(1, item + 3, ">");
    }
#endif

    __builtin_unreachable();
}
