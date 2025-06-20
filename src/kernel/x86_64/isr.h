#pragma once
#include <stdint.h>

typedef struct {
    uint64_t regs[15];
    uint64_t error_code;
    uint64_t vector;
    uint64_t dummy_align;
} exception_frame_t;

void isr_exception_handler(exception_frame_t *frame);
