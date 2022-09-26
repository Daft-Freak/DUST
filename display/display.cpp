#include <gba/gba.hpp>
#include <gba/ext/agbabi/agbabi.hpp>

#include "../common.hpp"

using namespace gba;

// no layers enabled
void display_layer_none() {

    // nothing enabled
    mmio::DISPCNT = {};

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

    auto raw_dispcnt = (volatile uint16_t *)&mmio::DISPCNT;

    // shouldn't be set for AGB mode
    if(*raw_dispcnt & (1 << 3)) {
        write_text(0, 0, "CGB mode is set!");
        return wait_for_exit();
    }

    // shouldn't be writable outside BIOS
    *raw_dispcnt = *raw_dispcnt | 1 << 3;

    if(*raw_dispcnt & (1 << 3)) {
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
    mmio::DISPCNT->forced_blank = true;

    wait_for_exit();
}

void display_priority_default() {
    // all four "text" layers
    mmio::DISPCNT = {
        .video_mode = 0,
        .show_bg0 = true,
        .show_bg1 = true,
        .show_bg2 = true,
        .show_bg3 = true,
    };

    mmio::BG0CNT = {
        .charblock = 0,
        .screenblock = 1,
    };

    mmio::BG1CNT = {
        .charblock = 0,
        .screenblock = 2,
    };

    mmio::BG2CNT = {
        .charblock = 0,
        .screenblock = 3,
    };

    mmio::BG3CNT = {
        .charblock = 0,
        .screenblock = 4,
    };

    // palette
    palette_ram[0] = 0x4210;
    palette_ram[1] = 0x03F0;
    palette_ram[2] = 0x001F;
    palette_ram[3] = 0x7C10;
    palette_ram[4] = 0x7FE0;

    // fill each layer with a single tile
    __agbabi_wordset4(video_ram + 0x0400, 0x800, 0x00010001);
    __agbabi_wordset4(video_ram + 0x0800, 0x800, 0x00020002);
    __agbabi_wordset4(video_ram + 0x0C00, 0x800, 0x00030003);
    __agbabi_wordset4(video_ram + 0x1000, 0x800, 0x00040004);

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

static const int dispstat_addr = 0x4000004;

static void irq_handler(int mask) {
    if(mask & (1 << 1)/*hblank*/) {
        stat_hblank_irq[*mmio::VCOUNT] = *reinterpret_cast<volatile uint16_t *>(dispstat_addr);
    }
}

// workaround irq handler assignment getting optimised out
[[gnu::noinline]] static void clear_irq_handler() {
    mmio::IRQ_HANDLER = agbabi::irq_empty;
}

void display_stat_flags() {
    // checks what lines the vblank flag is set on
    // and when hblank interrupts/dma happen
    uint16_t stat[228]{};

    mmio::DISPSTAT = {
        .irq_vblank = true,
        .vcount_setting = 123 // set to something
    };

    // read DISPSTAT on every line
    for(int i = 0; i < 228; i++) {
        while(*mmio::VCOUNT != i);

        // keep reading until we reach the next line
        // (trying to catch h-blank)
        auto v = *reinterpret_cast<volatile uint16_t *>(dispstat_addr);
        while(*mmio::VCOUNT == i) {
            stat[i] = v;
            v = *reinterpret_cast<volatile uint16_t *>(dispstat_addr);
        }
    }

    // now check using hblank interrupts
    memset(stat_hblank_irq, 0, sizeof(stat_hblank_irq));

    mmio::IRQ_HANDLER = agbabi::irq_user(irq_handler);

    mmio::DISPSTAT = {
        .irq_vblank = true,
        .irq_hblank = true,
        .vcount_setting = 123 // set to something
    };

    mmio::IE = {
        .vblank = true,
        .hblank = true,
    };

    // wait for a frame
    while(*mmio::VCOUNT != 0);
    while(*mmio::VCOUNT == 0);
    while(*mmio::VCOUNT != 0);

    clear_irq_handler();

    // and hblank dma
    memset(stat_hblank_dma, 0, sizeof(stat_hblank_dma));

    mmio::DMA0_SRC = reinterpret_cast<void *>(dispstat_addr);
    mmio::DMA0_DEST = stat_hblank_dma;
    mmio::DMA0_COUNT = 1;

    while(*mmio::VCOUNT != 227);

    mmio::DMA0_CONTROL = dmacnt_h {
        .dest_control = dest_addr::increment,
        .src_control = src_addr::fixed,
        .repeat = true,
        .transfer_32bit = false,
        .start_time = start::hblank,
        .enabled = true
    };

    // wait for a frame
    while(*mmio::VCOUNT != 0);
    while(*mmio::VCOUNT == 0);
    while(*mmio::VCOUNT != 0);

    mmio::DMA0_CONTROL.reset();

    // now put something on the screen
    mmio::DISPCNT = {
        .video_mode = 3,
        .show_bg2 = true
    };

    __agbabi_wordset4(video_ram, 240 * 160 * 2, 0);

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
