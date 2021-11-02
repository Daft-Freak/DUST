#include <gba/gba.hpp>

#include "../common.hpp"

using namespace gba;

// no layers enabled
void display_layer_none() {

    // nothing enabled
    reg::dispcnt::write({});

    // should get backdrop
    palRAM[0] = 0x7C10;

    // put some junk in the other colours
    for(int i = 1; i < 256; i++)
        palRAM[i] = (i * 0x1F2E3D4C) >> 16;

    // and fill vram with more junk
    for(int i = 0; i < 0x8000; i++)
        videoRAM[i] = i;

    wait_for_exit();
}