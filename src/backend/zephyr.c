#ifdef ZEPHYR_BACKEND

#include <zephyr/storage/flash_map.h>

#include "backend.h"

int fs_area_open(const uint64_t id, struct fs_area **fap) {
  int ret;

  ret = flash_area_open(id, (struct flash_area **)fap);

  return ret;
}

int fs_area_close(const struct fs_area *fap) {
  int ret;

  flash_area_close((const struct flash_area *)fap);
  ret = 0;

  return ret;
}

int fs_area_read(const struct fs_area *fap, size_t offset, void *dst,
                 size_t len) {
  int ret;

  ret = flash_area_read((const struct flash_area *)fap, offset, dst, len);

  return ret;
}

int fs_area_write(const struct fs_area *fap, size_t offset, const void *src,
                  size_t len) {
  int ret;

  ret = flash_area_write((const struct flash_area *)fap, offset, src, len);

  return ret;
}

#endif // ZEPHYR_BACKEND