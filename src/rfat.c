#include <rfat/rfat.h>

#include "backend/backend.h"

int rfat_fs_open(const void *id, struct fs_area **fapp) {
  int ret;

  ret = fs_area_open(id, fapp);
  if (ret != RFAT_SUCCESS) {
    ret = RFAT_FS_OPEN_FAILURE;
    goto rfat_fs_open_end;
  }

  ret = RFAT_SUCCESS;

rfat_fs_open_end:
  return ret;
}

int rfat_fs_close(const struct fs_area *fap) {
  int ret;

  ret = fs_area_close(fap);
  if (ret != RFAT_SUCCESS) {
    ret = RFAT_FS_CLOSE_FAILURE;
    goto rfat_fs_close_end;
  }

rfat_fs_close_end:
  return fs_area_close(fap);
}

int rfat_fs_validate(const struct fs_area *fap) {
  rfat_metadata_block_t metadata;
  int ret, sz;

  ret = fs_area_get_size(fap, &sz);
  if (ret != RFAT_SUCCESS) {
    ret = RFAT_FAILURE;
    goto rfat_fs_validate_end;
  }

  ret = fs_area_read(fap, sz - sizeof(metadata), &metadata, sizeof(metadata));
  if (ret != RFAT_SUCCESS) {
    ret = RFAT_FS_READ_FAILURE;
    goto rfat_fs_validate_end;
  }

  if (metadata.magic != RFAT_MAGIC) {
    ret = RFAT_FS_MAGIC_NUMBER_FAILURE;
    goto rfat_fs_validate_end;
  }

  ret = RFAT_SUCCESS;

rfat_fs_validate_end:
  return ret;
}

int rfat_fs_init(const struct fs_area *fap) {
  rfat_metadata_block_t metadata;
  int ret, sz;

  ret = fs_area_get_size(fap, &sz);
  if (ret != RFAT_SUCCESS) {
    ret = RFAT_FAILURE;
    goto rfat_fs_init_end;
  }

  ret = fs_area_read(fap, sz - sizeof(metadata), &metadata, sizeof(metadata));
  if (ret != RFAT_SUCCESS) {
    ret = RFAT_FS_READ_FAILURE;
    goto rfat_fs_init_end;
  }

  if (metadata.magic == RFAT_MAGIC) {
    ret = RFAT_SUCCESS;
    goto rfat_fs_init_end;
  }

  metadata.magic = RFAT_MAGIC;
  ret = fs_area_write(fap, sz - sizeof(metadata), &metadata, sizeof(metadata));
  if (ret != RFAT_SUCCESS) {
    ret = RFAT_FS_WRITE_FAILURE;
    goto rfat_fs_init_end;
  }

  ret = RFAT_SUCCESS;

rfat_fs_init_end:
  return ret;
}

int rfat_open(const struct fs_area *fap, const char *pathname, int flags) {
  return 0;
}

int rfat_creat(const struct fs_area *fap, const char *pathname, uint8_t mode) {
  return 0;
}

int rfat_close(const struct fs_area *fap, int fd) { return 0; }

int rfat_read(const struct fs_area *fap, int fd, void *buf, size_t count) {
  return 0;
}

int rfat_write(const struct fs_area *fap, int fd, const void *buf,
               size_t count) {
  return 0;
}