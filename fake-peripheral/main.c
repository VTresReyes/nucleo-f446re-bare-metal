#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "driver.h"

typedef struct {
    Fake_TypeDef dev;
    uint8_t guard[8];
} TestBench;

static TestBench bench;
static volatile Fake_TypeDef *const p = &bench.dev;
static int failures = 0;

static void check(const char *name, uint32_t got, uint32_t expected) {
    if (got == expected) {
        printf("PASS %s\n", name);
    } else {
        printf("FAIL %s (got 0x%X, expected 0x%X)\n",
               name, (unsigned)got, (unsigned)expected);
        failures++;
    }
}

static void reset(void) {
    memset(&bench, 0, sizeof bench);
    memset(bench.guard, 0xEE, sizeof bench.guard);
}

int main(void) {
    /* 1. layout: must match the "hardware" */
    check("sizeof block",   (uint32_t)sizeof(Fake_TypeDef), 24);
    check("offset CONTROL", (uint32_t)offsetof(Fake_TypeDef, CONTROL), 0);
    check("offset STATUS",  (uint32_t)offsetof(Fake_TypeDef, STATUS), 4);
    check("offset BUFFER",  (uint32_t)offsetof(Fake_TypeDef, BUFFER), 8);

    /* 2. enable */
    reset();
    periph_enable(p);
    check("enable sets bit 0", p->CONTROL, 0x1);
    p->CONTROL = 0xF0;
    periph_enable(p);
    check("enable keeps other bits", p->CONTROL, 0xF1);
    periph_enable(p);
    check("enable is idempotent", p->CONTROL, 0xF1);
    check("enable leaves STATUS alone", p->STATUS, 0);

    /* 3. busy flag (the test plays the role of the hardware) */
    reset();
    check("idle is not busy", periph_is_busy(p), 0);
    p->STATUS = 0x1;
    check("busy when bit 0 set", periph_is_busy(p), 1);
    p->STATUS = 0xFE;
    check("ignores other status bits", periph_is_busy(p), 0);
    p->STATUS = 0xFF;
    check("busy with all bits set", periph_is_busy(p), 1);

    /* 4. writing bytes */
    reset();
    check("write idx 0 returns true",  periph_write_byte(p, 0, 0xAB), 1);
    check("write idx 0 value",         p->BUFFER[0], 0xAB);
    check("write idx 15 returns true", periph_write_byte(p, 15, 0xCD), 1);
    check("write idx 15 value",        p->BUFFER[15], 0xCD);
    check("idx 1 untouched",           p->BUFFER[1], 0);
    check("idx 14 untouched",          p->BUFFER[14], 0);

    /* 5. bounds: out-of-range writes must be rejected and harmless */
    reset();
    check("idx 16 rejected",   periph_write_byte(p, 16, 0x77), 0);
    check("idx 17 rejected",   periph_write_byte(p, 17, 0x77), 0);
    check("idx 1000 rejected", periph_write_byte(p, 1000, 0x77), 0);
    uint32_t guard_ok = 1;
    for (size_t i = 0; i < sizeof bench.guard; i++) {
        if (bench.guard[i] != 0xEE) guard_ok = 0;
    }
    check("guard bytes intact", guard_ok, 1);
    check("last valid byte untouched", p->BUFFER[15], 0);

    /* 6. full buffer, overwrite, and no side effects */
    reset();
    for (unsigned i = 0; i < BUFFER_SIZE; i++) {
        periph_write_byte(p, i, (uint8_t)(i * 17u));
    }
    uint32_t all_ok = 1;
    for (unsigned i = 0; i < BUFFER_SIZE; i++) {
        if (p->BUFFER[i] != (uint8_t)(i * 17u)) all_ok = 0;
    }
    check("all 16 bytes stored", all_ok, 1);
    check("writes leave CONTROL alone", p->CONTROL, 0);
    check("writes leave STATUS alone", p->STATUS, 0);
    periph_write_byte(p, 3, 0x11);
    periph_write_byte(p, 3, 0x22);
    check("overwrite keeps last value", p->BUFFER[3], 0x22);

    printf("\n%d failure(s)\n", failures);
    return failures ? 1 : 0;
}