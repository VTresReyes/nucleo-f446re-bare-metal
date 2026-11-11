#ifndef DRIVER_H
#define DRIVER_H

#include <stdint.h>
#include <stdbool.h>

#define BUFFER_SIZE 16u
#define CONTROL_ENABLE_BIT 1u
#define STATUS_BUSY_BIT 1u

// struct that mirrors a made-up hardware register block
typedef struct {
    uint32_t CONTROL;
    uint32_t STATUS;
    uint8_t BUFFER[BUFFER_SIZE];
} Fake_TypeDef;

static inline void periph_enable(volatile Fake_TypeDef *ptr) {
    ptr->CONTROL |= CONTROL_ENABLE_BIT;
}

static inline bool periph_write_byte(volatile Fake_TypeDef *ptr, unsigned index, uint8_t data) {
    if(index >= BUFFER_SIZE) return false;
    ptr->BUFFER[index] = data;
    return true;
}

static inline bool periph_is_busy(volatile Fake_TypeDef *ptr) {
    return ptr->STATUS & STATUS_BUSY_BIT;
}

#endif