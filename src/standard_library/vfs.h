#pragma once

#include <stddef.h>
#include <stdint.h>

typedef int fd_t;

#define VFS_FD_STDIN    0
#define VFS_FD_STDOUT   1
#define VFS_FD_STDERR   2

int VFS_Write(fd_t file, uint8_t* data, size_t size);
