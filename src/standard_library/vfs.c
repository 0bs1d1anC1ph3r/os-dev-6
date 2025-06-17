#include <vfs.h>
#include <stddef.h>
#include <stdint.h>
#include <vga_text.h>

int vfs_write(fd_t file, uint8_t *data, size_t size)
  {
    switch (file) {
    case VFS_FD_STDIN:
      return 0;
    case VFS_FD_STDOUT:
    case VFS_FD_STDERR:
      for (size_t i = 0; i < size; ++i) {
        //vga_putc(data[i]);
      }
      return size;
    default:
      return -1;
    }
  }
