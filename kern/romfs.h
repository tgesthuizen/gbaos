#ifndef KERN_ROMFS_H
#define KERN_ROMFS_H

#include <stdint.h>

enum romfs_filetype {
  romfs_hard_link,
  romfs_directory,
  romfs_regular,
  romfs_symbolic_link,
  romfs_block_device,
  romfs_char_device,
  romfs_socket,
  romfs_fifo,
};

extern const char *romfs_get_name();
extern const char *romfs_file_name(uint32_t file);
extern int romfs_get_root(uint32_t *file);
extern int romfs_read_next(uint32_t *target, uint32_t file);
extern void *romfs_file_base(uint32_t file);

#endif
