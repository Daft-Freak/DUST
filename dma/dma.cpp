#include <gba/gba.hpp>
#include <gba/ext/agbabi/agbabi.hpp>

#include "../common.hpp"

using namespace gba;

static uint16_t dma_dest[5 * 16 + 8];

static const int dma0cnt_addr = 0x40000B8;
static const int dma1cnt_addr = 0x40000C4;
static const int dma2cnt_addr = 0x40000D0;
static const int dma3cnt_addr = 0x40000DC;

static const int dma0cnt_h_addr = 0x40000BA;
static const int dma3cnt_h_addr = 0x40000DE;

static const int tm0d_addr = 0x4000100;

// workaround interworking failure
// also affects memset (and memset generated for a manual fill loop...)
[[gnu::noinline]] static void wordset4(void *dest, size_t n, int c) {
    __agbabi_wordset4( dest, n, c );
}

// run from IWRAM for 1 cycle timing
[[gnu::section(".iwram")]]
void dma_delay() {

    mmio::DMA0_SRC = dma_dest + 0 * 16;
    mmio::DMA0_DEST = dma_dest + 1 * 16;
    mmio::DMA0_COUNT = 8;

    wordset4(dma_dest, sizeof(dma_dest), 0);

    const uint16_t dma0 = 0x8000; // enable

    int i = 0x00AA0055;
    auto p = dma_dest;

    // start DMA, then do some stores
    asm volatile(
        "strh %[dmaval], [%[dmacnt]]\n"
        "stmia %[dest]!, {%[val]}\n"
        "stmia %[dest]!, {%[val]}\n"
        "stmia %[dest]!, {%[val]}\n"
        "stmia %[dest]!, {%[val]}\n"
        : [dest] "+r" (p)
        : [dmacnt] "l" (dma0cnt_h_addr), [dmaval] "r" (dma0),  [val] "r" (i)
    );

    // and again with an extra nop
    mmio::DMA0_SRC = dma_dest + 2 * 16;
    mmio::DMA0_DEST = dma_dest + 3 * 16;
    mmio::DMA0_COUNT = 8;

    p = dma_dest + 2 * 16;

    asm volatile(
        "strh %[dmaval], [%[dmacnt]]\n"
        "nop\n"
        "stmia %[dest]!, {%[val]}\n"
        "stmia %[dest]!, {%[val]}\n"
        "stmia %[dest]!, {%[val]}\n"
        "stmia %[dest]!, {%[val]}\n"
        : [dest] "+r" (p)
        : [dmacnt] "l" (dma0cnt_h_addr), [dmaval] "r" (dma0),  [val] "r" (i)
    );

    char buf[30];

    clear_text();

    for(int i = 0; i < 4; i++) {

        int off = i * 16;

        snprintf(buf, sizeof(buf), "%2x %2x %2x %2x %2x %2x %2x %2x %2x %2x",
                dma_dest[off + 0], dma_dest[off + 1], dma_dest[off + 2], dma_dest[off + 3], dma_dest[off + 4],
                dma_dest[off + 5], dma_dest[off + 6], dma_dest[off + 7], dma_dest[off + 8], dma_dest[off + 9]);

        write_text(0, i * 2 + 1, buf);
    }

    wait_for_exit();
}

[[gnu::section(".iwram")]]
void dma_priority() {
    // each DMA overrites part of the source data of the next DMA
    mmio::DMA0_SRC = dma_dest + 0 * 16;  // 0 1 2 3...
    mmio::DMA0_DEST = dma_dest + 1 * 16 + 4;

    mmio::DMA1_SRC = dma_dest + 1 * 16; // ff fe fd fc...
    mmio::DMA1_DEST = dma_dest + 2 * 16 + 4;

    mmio::DMA2_SRC = dma_dest + 2 * 16; // 20 21 22 23...
    mmio::DMA2_DEST = dma_dest + 3 * 16 + 4;

    mmio::DMA3_SRC = dma_dest + 3 * 16; // df de dd dc...
    mmio::DMA3_DEST = dma_dest + 4 * 16 + 4;

    for(int i = 0; i < 16; i++) {
        dma_dest[i] = i;
        dma_dest[i + 16] = 0xFF - i;
        dma_dest[i + 32] = i + 32;
        dma_dest[i + 48] = 0xFF - (i + 32);
        dma_dest[i + 64] = i + 64;
    }

    const uint32_t dma = 0x80000010; // enable, 16 transfers

    // start timer then DMA
    asm volatile(
        "str %1, [%0]\n"
        "str %1, [%2]\n"
        "str %1, [%3]\n"
        "str %1, [%4]\n"
        :
        : "r" (dma0cnt_addr), "r" (dma), "r" (dma1cnt_addr), "r" (dma2cnt_addr), "r" (dma3cnt_addr)
    );

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
    mmio::DMA0_SRC = dma_dest + 0 * 16;  // 0 1 2 3...
    mmio::DMA0_DEST = dma_dest + 1 * 16 + 4;

    mmio::DMA1_SRC = dma_dest + 1 * 16; // ff fe fd fc...
    mmio::DMA1_DEST = dma_dest + 2 * 16 + 4;

    mmio::DMA2_SRC = dma_dest + 2 * 16; // 20 21 22 23...
    mmio::DMA2_DEST = dma_dest + 3 * 16 + 4;

    mmio::DMA3_SRC = dma_dest + 3 * 16; // df de dd dc...
    mmio::DMA3_DEST = dma_dest + 4 * 16 + 4;

    for(int i = 0; i < 16; i++) {
        dma_dest[i] = i;
        dma_dest[i + 16] = 0xFF - i;
        dma_dest[i + 32] = i + 32;
        dma_dest[i + 48] = 0xFF - (i + 32);
        dma_dest[i + 64] = i + 64;
    }

    const uint32_t dma = 0x80000010; // enable, 16 transfers

    // start DMA
    asm volatile(

        "str %1, [%0]\n"
        "str %1, [%2]\n"
        "str %1, [%3]\n"
        "str %1, [%4]\n"
        :
        : "r" (dma3cnt_addr), "r" (dma), "r" (dma2cnt_addr), "r" (dma1cnt_addr), "r" (dma0cnt_addr)
    );

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
void dma_priority_hblank() {
    // tries to start a dma in the middle of another one

    // high priority
    mmio::DMA0_SRC = dma_dest + 0 * 16;
    mmio::DMA0_DEST = dma_dest + 1 * 16;
    mmio::DMA0_COUNT = 8;

    // low priority
    mmio::DMA3_SRC = reinterpret_cast<void *>(0x8000000); // from ROM
    mmio::DMA3_DEST = dma_dest + 5 * 16;
    mmio::DMA3_COUNT = 300;

    wordset4(dma_dest, sizeof(dma_dest), 0);

    for(int i = 0; i < 16; i++)
        dma_dest[i] = i + 1;

    const uint16_t dma0 = 0xA000; // enable, hblank
    const uint16_t dma3 = 0x8040; // enable, dest fixed

    // start of first line
    while(*mmio::VCOUNT != 0);

    // start DMAs
    asm volatile(
        "strh %2, [%0]\n"
        "strh %3, [%1]\n"
        :
        : "r" (dma0cnt_h_addr), "r" (dma3cnt_h_addr), "l" (dma0), "l" (dma3)
    );

    char buf[30];

    clear_text();

    for(int i = 0; i < 4; i++) {

        int off = i * 16;

        snprintf(buf, sizeof(buf), "%2x %2x %2x %2x %2x %2x %2x %2x %2x %2x",
                dma_dest[off + 0], dma_dest[off + 1], dma_dest[off + 2], dma_dest[off + 3], dma_dest[off + 4],
                dma_dest[off + 5], dma_dest[off + 6], dma_dest[off + 7], dma_dest[off + 8], dma_dest[off + 9]);

        write_text(0, i * 2 + 1, buf);
    }

    wait_for_exit();
}

[[gnu::section(".iwram")]]
void dma_timer() {
    // use DMA to read a timer repeatedly... which seems like a silly thing to do
    mmio::DMA3_SRC = reinterpret_cast<void *>(tm0d_addr);
    mmio::DMA3_DEST = dma_dest;

    /*reg::tm0d_cnt = {
        .control = {
            .enable = true
        }
    };
    reg::dma3cnt = gba::dma_transfer_control {
        .transfers = 4,
        .control = {
            .source_control = dma_control::source_address::fixed,
            .enable = true
        }
    };*/

    const uint32_t timer = 0x800000; // enable
    const uint32_t dma = 0x81000004; // enable, fixed src, 4 transfers

    // start timer then DMA
    asm volatile(
        "str %1, [%0]\n"
        "str %3, [%2]\n"
        :
        : "r" (tm0d_addr), "r" (timer), "r" (dma3cnt_addr), "r" (dma)
    );

    mmio::TIMER0_CONTROL.reset();

    char buf[10];

    clear_text();

    snprintf(buf, sizeof(buf), "%2i %2i %2i %2i", dma_dest[0], dma_dest[1], dma_dest[2], dma_dest[3]);
    write_text(1, 1, buf);

    wait_for_exit();
}

[[gnu::section(".iwram")]]
void dma_timer_multi() {
    // use two DMA channels to read a timer repeatedly... which is an even sillier thing to do

    mmio::DMA3_SRC = reinterpret_cast<void *>(tm0d_addr);
    mmio::DMA3_DEST = dma_dest;
    mmio::DMA2_SRC = reinterpret_cast<void *>(tm0d_addr);
    mmio::DMA2_DEST = dma_dest + 8;

    const uint32_t timer = 0x800000; // enable
    const uint32_t dma = 0x81000004; // enable, fixed src, 4 transfers

    // start timer then DMAs
    asm volatile(
        "str %1, [%0]\n"
        "str %4, [%2]\n"
        "str %4, [%3]\n"
        :
        : "r" (tm0d_addr), "r" (timer), "r" (dma3cnt_addr), "r" (dma2cnt_addr), "r" (dma)
    );

    mmio::TIMER0_CONTROL.reset();

    char buf[20];

    clear_text();

    snprintf(buf, sizeof(buf), "%2i %2i %2i %2i", dma_dest[0], dma_dest[1], dma_dest[2], dma_dest[3]);
    write_text(1, 1, buf);

    snprintf(buf, sizeof(buf), "%2i %2i %2i %2i", dma_dest[8], dma_dest[9], dma_dest[10], dma_dest[11]);
    write_text(1, 2, buf);

    wait_for_exit();
}
