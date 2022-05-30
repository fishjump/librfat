#ifdef POSIX_BACKEND

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "backend.h"

struct fs_area {
  FILE *f;
  size_t sz;
  uint8_t buffer[0];
};

int fs_area_open(const uint64_t id, struct fs_area **fap) {
  FILE *f;
  int ret;
  size_t sz;

  f = fopen((const char *)id, "rb+");
  if (!f) {
    ret = -1;
    goto fs_area_open_end;
  }

  fseek(f, 0, SEEK_END);
  sz = ftell(f);
  *fap =
      (struct fs_area *)malloc(sizeof(struct fs_area) + sz * sizeof(uint8_t));
  if (!(*fap)) {
    ret = -2;
    goto fs_area_open_end;
  }

  (*fap)->f = f;
  (*fap)->sz = sz;
  ret = fread((*fap)->buffer, sizeof(uint8_t), sz, f);
  if (ret != sz) {
    ret = -3;
    goto fs_area_open_end;
  }

  ret = 0;

fs_area_open_end:
  return ret;
}

int fs_area_close(const struct fs_area *fap) {
  int ret;
  ret = fclose(fap->f);
  if (ret == EOF) {
    ret = -1;
    goto fs_area_close_end;
  }

  ret = 0;

fs_area_close_end:
  free((void *)fap);
  return ret;
}

int fs_area_read(const struct fs_area *fap, size_t offset, void *dst,
                 size_t len) {
  int ret;

  memcpy(dst, (void *)((uintptr_t)fap->buffer + offset), len);

  ret = 0;

  return ret;
}

int fs_area_write(const struct fs_area *fap, size_t offset, const void *src,
                  size_t len) {
  int ret;

  memcpy((void *)((uintptr_t)fap->buffer + offset), src, len);

  ret = 0;

  return ret;
}

#endif // POSIX_BACKEND