#pragma once
#include <stdint.h>

void isr_exception_handler(uint64_t vector, uint64_t error_code);
