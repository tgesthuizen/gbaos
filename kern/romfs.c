#include "romfs.h"
#include <kern/errno.h>

/**
 * romfs data structures based on
 * https://docs.kernel.org/filesystems/romfs.html
 */

struct romfs_header {
  char id[8];
  u32 full_size;
  u32 checksum;
  char volume_name[16];
};

struct romfs_filehdr {
  u32 next_filehdr;
  u32 spec_info;
  u32 size;
  u32 checksum;
  char file_name[16];
};

static u32 be32toh(void *field) {
  unsigned char *f = field;
  return ((u32)f[0] << 24) + ((u32)f[1] << 16) +
         ((u32)f[2] << 8) + ((u32)f[3] << 0);
}

#define ROMFS_NEXT_FILEHDR_ADDR(x) ((x) & ~(u32)15)
#define ROMFS_NEXT_FILEHDR_EXEC(x) ((x)&8)
#define ROMFS_NEXT_FILEHDR_TYPE(x) ((x)&7)

extern unsigned char __romfs_start[];

const char *romfs_get_name() {
  return ((struct romfs_header *)&__romfs_start)->volume_name;
}

#define HDR_FROM_OFFSET(offset)                                                \
  ((struct romfs_filehdr *)(__romfs_start + offset))

const char *romfs_file_name(u32 file) {
  return HDR_FROM_OFFSET(file)->file_name;
}

int romfs_get_root(u32 *file) {
  *file = sizeof(struct romfs_header);
  return 0;
}

int romfs_read_next(u32 *target, u32 file) {
  const u32 next_hdr =
      ROMFS_NEXT_FILEHDR_ADDR(be32toh(&HDR_FROM_OFFSET(file)->next_filehdr));
  if (!next_hdr)
    return ENOENT;
  *target = next_hdr;
  return 0;
}

void *romfs_file_base(u32 file) {
  return __romfs_start + file + sizeof(struct romfs_filehdr);
}
