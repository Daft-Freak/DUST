#include <gba/gba.hpp>
#include <gba/ext/agbabi.hpp>

#include "../common.hpp"

using namespace gba;

static uint16_t dma_dest[5 * 16 + 8];

// run from IWRAM for 1 cycle timing
[[gnu::section(".iwram")]]
void dma_delay() {
    reg::dma3sad::write(reg::tm0d::address);
    reg::dma3dad::write(uintptr_t(dma_dest));

    /*reg::tm0d_cnt::write({
        .control = {
            .enable = true
        }
    });
    reg::dma3cnt::write({
        .transfers = 1,
        .control = {
            .enable = true
        }
    });*/

    const uint32_t timer = 0x800000; // enable
    const uint32_t dma = 0x80000001; // enable, 1 transfer

    // start timer then DMA
    asm volatile(
        "str %1, [%0]\n"
        "str %3, [%2]\n"
        :
        : "r" (reg::tm0d_cnt::address), "r" (timer), "r" (reg::dma3cnt::address), "r" (dma)
    );

    reg::tm0d_cnt::write({});

    char buf[10];

    clear_text();
    snprintf(buf, sizeof(buf), "%i", dma_dest[0]);

    write_text(1, 1, buf);

    wait_for_exit();
}

[[gnu::section(".iwram")]]
void dma_priority() {
    // each DMA overrites part of the source data of the next DMA
    reg::dma0sad::write(uintptr_t(dma_dest + 0 * 16));  // 0 1 2 3...
    reg::dma0dad::write(uintptr_t(dma_dest + 1 * 16 + 4));

    reg::dma1sad::write(uintptr_t(dma_dest + 1 * 16)); // ff fe fd fc...
    reg::dma1dad::write(uintptr_t(dma_dest + 2 * 16 + 4));

    reg::dma2sad::write(uintptr_t(dma_dest + 2 * 16)); // 20 21 22 23...
    reg::dma2dad::write(uintptr_t(dma_dest + 3 * 16 + 4));

    reg::dma3sad::write(uintptr_t(dma_dest + 3 * 16)); // df de dd dc...
    reg::dma3dad::write(uintptr_t(dma_dest + 4 * 16 + 4));

    for(int i = 0; i < 16; i++) {
        dma_dest[i] = i;
        dma_dest[i + 16] = 0xFF - i;
        dma_dest[i + 32] = i + 32;
        dma_dest[i + 48] = 0xFF - (i + 32);
        dma_dest[i + 64] = i + 64;
    }

    const uint32_t timer = 0x800000; // enable
    const uint32_t dma = 0x80000010; // enable, 16 transfers

    // start timer then DMA
    asm volatile(
        "str %1, [%0]\n"
        "str %3, [%2]\n"
        "str %3, [%4]\n"
        "str %3, [%5]\n"
        "str %3, [%6]\n"
        :
        : "r" (reg::tm0d_cnt::address), "r" (timer), "r" (reg::dma0cnt::address), "r" (dma), "r" (reg::dma1cnt::address), "r" (reg::dma2cnt::address), "r" (reg::dma3cnt::address)
    );

    reg::tm0d_cnt::write({});

    char buf[30];

    clear_text();

    for(int i = 0; i < 4; i++) {

        int off = (i + 1) * 16 + 4;

        snprintf(buf, sizeof(buf), "%2x %2x %2x %2x %2x %2x %2x %2x %2x %2x",
                dma_dest[off + 0], dma_dest[off + 1], dma_dest[off + 2], dma_dest[off + 3], dma_dest[off + 4],
                dma_dest[off + 5], dma_dest[off + 6], dma_dest[off + 7], dma_dest[off + 8], dma_dest[off + 9]);

        write_text(0, i * 2 + 1, buf);
    }

    wait_for_exit();
}


[[gnu::section(".iwram")]]
void dma_priority_reverse() {
    // same thing but triggers the DMAs in reverse order

    // each DMA overrites part of the source data of the next DMA
    reg::dma0sad::write(uintptr_t(dma_dest + 0 * 16));  // 0 1 2 3...
    reg::dma0dad::write(uintptr_t(dma_dest + 1 * 16 + 4));

    reg::dma1sad::write(uintptr_t(dma_dest + 1 * 16)); // ff fe fd fc...
    reg::dma1dad::write(uintptr_t(dma_dest + 2 * 16 + 4));

    reg::dma2sad::write(uintptr_t(dma_dest + 2 * 16)); // 20 21 22 23...
    reg::dma2dad::write(uintptr_t(dma_dest + 3 * 16 + 4));

    reg::dma3sad::write(uintptr_t(dma_dest + 3 * 16)); // df de dd dc...
    reg::dma3dad::write(uintptr_t(dma_dest + 4 * 16 + 4));

    for(int i = 0; i < 16; i++) {
        dma_dest[i] = i;
        dma_dest[i + 16] = 0xFF - i;
        dma_dest[i + 32] = i + 32;
        dma_dest[i + 48] = 0xFF - (i + 32);
        dma_dest[i + 64] = i + 64;
    }

    const uint32_t timer = 0x800000; // enable
    const uint32_t dma = 0x80000010; // enable, 16 transfers

    // start timer then DMA
    asm volatile(
        "str %1, [%0]\n"
        "str %3, [%2]\n"
        "str %3, [%4]\n"
        "str %3, [%5]\n"
        "str %3, [%6]\n"
        :
        : "r" (reg::tm0d_cnt::address), "r" (timer), "r" (reg::dma3cnt::address), "r" (dma), "r" (reg::dma2cnt::address), "r" (reg::dma1cnt::address), "r" (reg::dma0cnt::address)
    );

    reg::tm0d_cnt::write({});

    char buf[30];

    clear_text();

    for(int i = 0; i < 4; i++) {

        int off = (i + 1) * 16 + 4;

        snprintf(buf, sizeof(buf), "%2x %2x %2x %2x %2x %2x %2x %2x %2x %2x",
                dma_dest[off + 0], dma_dest[off + 1], dma_dest[off + 2], dma_dest[off + 3], dma_dest[off + 4],
                dma_dest[off + 5], dma_dest[off + 6], dma_dest[off + 7], dma_dest[off + 8], dma_dest[off + 9]);

        write_text(0, i * 2 + 1, buf);
    }

    wait_for_exit();
}
