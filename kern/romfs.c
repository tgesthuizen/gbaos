#include "romfs.h"
#include <kern/errno.h>

/**
 * romfs data structures based on
 * https://docs.kernel.org/filesystems/romfs.html
 */

struct romfs_header {
  char id[8];
  uint32_t full_size;
  uint32_t checksum;
  char volume_name[16];
};

struct romfs_filehdr {
  uint32_t next_filehdr;
  uint32_t spec_info;
  uint32_t size;
  uint32_t checksum;
  char file_name[16];
};

static uint32_t be32toh(void *field) {
  unsigned char *f = field;
  return ((uint32_t)f[0] << 24) + ((uint32_t)f[1] << 16) +
         ((uint32_t)f[2] << 8) + ((uint32_t)f[3] << 0);
}

#define ROMFS_NEXT_FILEHDR_ADDR(x) ((x) & ~(uint32_t)15)
#define ROMFS_NEXT_FILEHDR_EXEC(x) ((x)&8)
#define ROMFS_NEXT_FILEHDR_TYPE(x) ((x)&7)

extern unsigned char __romfs_start[];

const char *romfs_get_name() {
  return ((struct romfs_header *)&__romfs_start)->volume_name;
}

#define HDR_FROM_OFFSET(offset)                                                \
  ((struct romfs_filehdr *)(__romfs_start + offset))

const char *romfs_file_name(uint32_t file) {
  return HDR_FROM_OFFSET(file)->file_name;
}

int romfs_get_root(uint32_t *file) {
  *file = sizeof(struct romfs_header);
  return 0;
}

int romfs_read_next(uint32_t *target, uint32_t file) {
  const uint32_t next_hdr =
      ROMFS_NEXT_FILEHDR_ADDR(be32toh(&HDR_FROM_OFFSET(file)->next_filehdr));
  if (!next_hdr)
    return ENOENT;
  *target = next_hdr;
  return 0;
}

void *romfs_file_base(uint32_t file) {
  return __romfs_start + file + sizeof(struct romfs_filehdr);
}
