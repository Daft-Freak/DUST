
#include <gba/gba.hpp>
#include <gba/ext/agbabi.hpp>

using namespace gba;

int main(int argc, char * argv[]) {
    reg::dispstat::write({.vblank_irq = true});
    reg::ie::write({.vblank = true});
    reg::ime::emplace(true);

    reg::dispcnt::write({
        .mode = 0,
        .layer_background_0 = true,
    });

    io::keypad_manager keypad;

    while(true) {
        keypad.poll();

        bios::vblank_intr_wait();
    }

    __builtin_unreachable();
}
