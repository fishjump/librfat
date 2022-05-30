#ifdef ZEPHYR_BACKEND

#include <zephyr/storage/flash_map.h>

#include "backend.h"

int fs_area_open(const void *id, struct fs_area **fap) {
  int ret;

  ret = flash_area_open(*((uint8_t *)id), (const struct flash_area **)fap);
  if (ret != 0) {
    ret = BACKEND_FAILURE;
    goto fs_area_open_end;
  }

  ret = BACKEND_SUCCESS;

fs_area_open_end:
  return ret;
}

int fs_area_close(const struct fs_area *fap) {
  int ret;

  flash_area_close((const struct flash_area *)fap);
  ret = BACKEND_SUCCESS;

  return ret;
}

int fs_area_read(const struct fs_area *fap, size_t offset, void *dst,
                 size_t len) {
  int ret;

  ret = flash_area_read((const struct flash_area *)fap, offset, dst, len);
  if (ret != 0) {
    ret = BACKEND_FAILURE;
    goto fs_area_read_end;
  }

  ret = BACKEND_SUCCESS;

fs_area_read_end:
  return ret;
}

int fs_area_write(const struct fs_area *fap, size_t offset, const void *src,
                  size_t len) {
  int ret;

  ret = flash_area_write((const struct flash_area *)fap, offset, src, len);
  if (ret != 0) {
    ret = BACKEND_FAILURE;
    goto fs_area_write_end;
  }

  ret = BACKEND_SUCCESS;

fs_area_write_end:
  return ret;
}

int fs_area_get_size(const struct fs_area *fap, size_t *sz) {
  int ret;

  if (fap == NULL) {
    ret = BACKEND_FAILURE;
    goto fs_area_get_size_end;
  }

  *sz = ((const struct flash_area *)fap)->fa_size;

  ret = BACKEND_SUCCESS;

fs_area_get_size_end:
  return ret;
}

#endif // ZEPHYR_BACKEND