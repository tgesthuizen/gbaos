#ifndef KERN_ROMFS_H
#define KERN_ROMFS_H

#include "types.h"

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
extern const char *romfs_file_name(u32 file);
extern int romfs_get_root(u32 *file);
extern int romfs_read_next(u32 *target, u32 file);
extern void *romfs_file_base(u32 file);

#endif
