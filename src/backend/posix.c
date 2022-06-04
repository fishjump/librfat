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

int fs_area_open(const void *id, struct fs_area **fap) {
  FILE *f;
  int ret;
  size_t sz;

  f = fopen((const char *)id, "rb+");
  if (!f) {
    ret = BACKEND_FAILURE;
    goto fs_area_open_end;
  }

  fseek(f, 0, SEEK_END);
  sz = ftell(f);
  fseek(f, 0, SEEK_SET);

  *fap =
      (struct fs_area *)malloc(sizeof(struct fs_area) + sz * sizeof(uint8_t));
  if (!(*fap)) {
    ret = BACKEND_FAILURE;
    goto fs_area_open_end;
  }

  (*fap)->f = f;
  (*fap)->sz = sz;
  ret = fread((*fap)->buffer, sizeof(uint8_t), sz, f);
  if (ret != sz) {
    ret = BACKEND_FAILURE;
    goto fs_area_open_end;
  }

  ret = BACKEND_SUCCESS;

fs_area_open_end:
  return ret;
}

int fs_area_close(const struct fs_area *fap) {
  int ret;

  ret = fwrite(fap->buffer, sizeof(uint8_t), fap->sz, fap->f);
  if (ret != fap->sz) {
    printf("%s: %d\n", __FILE__, __LINE__);
    ret = BACKEND_FAILURE;
    goto fs_area_close_end;
  }

  ret = fclose(fap->f);
  if (ret == EOF) {
    printf("%s: %d\n", __FILE__, __LINE__);
    ret = BACKEND_FAILURE;
    goto fs_area_close_end;
  }

  ret = BACKEND_SUCCESS;

fs_area_close_end:
  free((void *)fap);
  return ret;
}

int fs_area_read(const struct fs_area *fap, size_t offset, void *dst,
                 size_t len) {
  int ret;

  memcpy(dst, (void *)((uintptr_t)fap->buffer + offset), len);

  ret = BACKEND_SUCCESS;

  return ret;
}

int fs_area_write(const struct fs_area *fap, size_t offset, const void *src,
                  size_t len) {
  int ret;

  memcpy((void *)((uintptr_t)fap->buffer + offset), src, len);

  ret = BACKEND_SUCCESS;

  return ret;
}

int fs_area_get_size(const struct fs_area *fap, size_t *sz) {
  int ret;

  if (fap == NULL) {
    ret = BACKEND_FAILURE;
    goto fs_area_get_size_end;
  }

  *sz = fap->sz;

  ret = BACKEND_SUCCESS;

fs_area_get_size_end:
  return ret;
}

#endif // POSIX_BACKEND