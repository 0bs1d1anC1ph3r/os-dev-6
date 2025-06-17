#pragma once
#ifndef MALLOC_H_
#define MALLOC_H_

#include "stdint.h"

struct malloc_header
{
  unsigned int size;
  int index;

  struct malloc_header *next;
  struct malloc_header *prev;
  uint32_t bin_magic;
};

#endif
