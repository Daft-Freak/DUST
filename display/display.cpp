#include <gba/gba.hpp>
#include <gba/ext/agbabi.hpp>

#include "../common.hpp"

using namespace gba;

// no layers enabled
void display_layer_none() {

    // nothing enabled
    reg::dispcnt::write({});

    // should get backdrop
    palette_ram[0] = 0x7C10;

    // put some junk in the other colours
    for(int i = 1; i < 256; i++)
        palette_ram[i] = (i * 0x1F2E3D4C) >> 16;

    // and fill vram with more junk
    for(int i = 0; i < 0x8000; i++)
        video_ram[i] = i;

    wait_for_exit();
}

void display_cgb_mode() {
    clear_text();
    palette_ram[1] = 0x1F;

    // shouldn't be set for AGB mode
    if(reg::dispcnt::read().color_game_boy) {
        write_text(0, 0, "CGB mode is set!");
        return wait_for_exit();
    }

    // shouldn't be writable outside BIOS
    auto dispcnt = reg::dispcnt::read();
    dispcnt.color_game_boy = true;
    reg::dispcnt::write(dispcnt);

    if(reg::dispcnt::read().color_game_boy) {
        write_text(0, 1, "CGB mode is writable!");
        return wait_for_exit();
    }

    palette_ram[0] = 0x7C10;

    wait_for_exit();
}

void display_forced_blank() {
    palette_ram[0] = 0;

    write_text(0, 10, "     That didn't work...     ");

    // should blank screen (to white)
    auto dispcnt = reg::dispcnt::read();
    dispcnt.force_blank = true;
    reg::dispcnt::write({dispcnt});

    wait_for_exit();
}

void display_priority_default() {
    // all four "text" layers
    reg::dispcnt::write({
        .mode = 0,
        .layer_background_0 = true,
        .layer_background_1 = true,
        .layer_background_2 = true,
        .layer_background_3 = true,
    });

    reg::bg0cnt::write(background_control {
        .character_base_block = 0,
        .screen_base_block = 1,
    });

    reg::bg1cnt::write(background_control {
        .character_base_block = 0,
        .screen_base_block = 2,
    });

    reg::bg2cnt::write(background_control {
        .character_base_block = 0,
        .screen_base_block = 3,
    });

    reg::bg3cnt::write(background_control {
        .character_base_block = 0,
        .screen_base_block = 4,
    });

    // palette
    palette_ram[0] = 0x4210;
    palette_ram[1] = 0x03F0;
    palette_ram[2] = 0x001F;
    palette_ram[3] = 0x7C10;
    palette_ram[4] = 0x7FE0;

    // fill each layer with a single tile
    agbabi::wordset4(video_ram + 0x0400, 0x800, 0x00010001);
    agbabi::wordset4(video_ram + 0x0800, 0x800, 0x00020002);
    agbabi::wordset4(video_ram + 0x0C00, 0x800, 0x00030003);
    agbabi::wordset4(video_ram + 0x1000, 0x800, 0x00040004);

    // generate some tiles
    int i = 0;

    for(int y = 0; y < 8; y++) {
        video_ram[i++] = 0x0000; video_ram[i++] = 0x0000;
    }

    for(int y = 0; y < 8; y++) {
        video_ram[i++] = 0x0010; video_ram[i++] = 0x0000;
    }

    for(int y = 0; y < 8; y++) {
        video_ram[i++] = 0x0022; video_ram[i++] = 0x0000;
    }

    for(int y = 0; y < 8; y++) {
        video_ram[i++] = 0x3033; video_ram[i++] = 0x0000;
    }

    for(int y = 0; y < 8; y++) {
        video_ram[i++] = 0x4444; video_ram[i++] = 0x0000;
    }

    wait_for_exit();
}

static uint16_t stat_hblank_irq[228]{};
static uint16_t stat_hblank_dma[228]{};

static void irq_handler(interrupt_mask mask) {
    if(mask.hblank) {
        stat_hblank_irq[reg::vcount::read()] = *reinterpret_cast<volatile uint16_t *>(reg::dispstat::address);
    }
}

void display_stat_flags() {
    // checks what lines the vblank flag is set on
    // and when hblank interrupts/dma happen
    uint16_t stat[228]{};

    reg::dispstat::write({
        .vblank_irq = true,
        .vcount_setting = 123 // set to something
    });

    // read DISPSTAT on every line
    for(int i = 0; i < 228; i++) {
        while(reg::vcount::read() != i);

        // keep reading until we reach the next line
        // (trying to catch h-blank)
        auto v = *reinterpret_cast<volatile uint16_t *>(reg::dispstat::address);
        while(reg::vcount::read() == i) {
            stat[i] = v;
            v = *reinterpret_cast<volatile uint16_t *>(reg::dispstat::address);
        }
    }

    // now check using hblank interrupts
    memset(stat_hblank_irq, 0, sizeof(stat_hblank_irq));

    agbabi::interrupt_handler::set(irq_handler);

    reg::dispstat::write({
        .vblank_irq = true,
        .hblank_irq = true,
        .vcount_setting = 123 // set to something
    });

    reg::ie::write({
        .vblank = true,
        .hblank = true,
    });

    // wait for a frame
    while(reg::vcount::read() != 0);
    while(reg::vcount::read() == 0);
    while(reg::vcount::read() != 0);

    agbabi::interrupt_handler::set(nullptr);

    // and hblank dma
    memset(stat_hblank_dma, 0, sizeof(stat_hblank_dma));

    reg::dma0sad::write(reg::dispstat::address);
    reg::dma0dad::write(reinterpret_cast<uint32_t>(stat_hblank_dma));
    reg::dma0cnt_l::write(1);

    while(reg::vcount::read() != 227);

    reg::dma0cnt_h::write(dma_control {
        .destination_control = dma_control::destination_address::increment,
        .source_control = dma_control::source_address::fixed,
        .repeat = true,
        .type = dma_control::type::half,
        .start_condition = dma_control::start::next_hblank,
        .enable = true
    });

    // wait for a frame
    while(reg::vcount::read() != 0);
    while(reg::vcount::read() == 0);
    while(reg::vcount::read() != 0);

    reg::dma0cnt_h::write({});

    // now put something on the screen
    reg::dispcnt::write({
        .mode = 3,
        .layer_background_2 = true
    });

    agbabi::wordset4(video_ram, 240 * 160 * 2, 0);

    for(int i = 0; i < 228; i++) {
        video_ram[i + 0 * 240] = (stat[i] & (1 << 0)) ? 0x03E0 : 0x001F;
        video_ram[i + 2 * 240] = (stat[i] & (1 << 1)) ? 0x03E0 : 0x001F;
        video_ram[i + 4 * 240] = (stat[i] & (1 << 2)) ? 0x03E0 : 0x001F;

        // hblank irq
        video_ram[i +  8 * 240] = (stat_hblank_irq[i] & (1 << 0)) ? 0x03E0 : 0x001F;
        video_ram[i + 10 * 240] = (stat_hblank_irq[i] & (1 << 1)) ? 0x03E0 : 0x001F;
        video_ram[i + 12 * 240] = (stat_hblank_irq[i] & (1 << 2)) ? 0x03E0 : 0x001F;

        // hblank dma
        video_ram[i + 16 * 240] = (stat_hblank_dma[i] & (1 << 0)) ? 0x03E0 : 0x001F;
        video_ram[i + 18 * 240] = (stat_hblank_dma[i] & (1 << 1)) ? 0x03E0 : 0x001F;
        video_ram[i + 20 * 240] = (stat_hblank_dma[i] & (1 << 2)) ? 0x03E0 : 0x001F;
    }

    wait_for_exit();
}