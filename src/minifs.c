#include <minifs/minifs.h>

#include "backend/backend.h"
#include "typedefs.h"

int minifs_fs_open(const void *id, struct fs_area **fapp) {
  int ret;

  ret = fs_area_open(id, fapp);
  if (ret != MINIFS_SUCCESS) {
    ret = MINIFS_FS_OPEN_FAILURE;
    goto minifs_fs_open_end;
  }

  ret = MINIFS_SUCCESS;

minifs_fs_open_end:
  return ret;
}

int minifs_fs_close(const struct fs_area *fap) {
  int ret;

  ret = fs_area_close(fap);
  if (ret != MINIFS_SUCCESS) {
    ret = MINIFS_FS_CLOSE_FAILURE;
    goto minifs_fs_close_end;
  }

minifs_fs_close_end:
  return fs_area_close(fap);
}

int minifs_fs_validate(const struct fs_area *fap) {
  minifs_metadata_t metadata;
  int ret, sz;

  ret = fs_area_get_size(fap, &sz);
  if (ret != MINIFS_SUCCESS) {
    ret = MINIFS_FAILURE;
    goto minifs_fs_validate_end;
  }

  ret = fs_area_read(fap, sz - sizeof(metadata), &metadata, sizeof(metadata));
  if (ret != MINIFS_SUCCESS) {
    ret = MINIFS_FS_READ_FAILURE;
    goto minifs_fs_validate_end;
  }

  if (metadata.magic != MINIFS_MAGIC) {
    ret = MINIFS_FS_MAGIC_NUMBER_FAILURE;
    goto minifs_fs_validate_end;
  }

  ret = MINIFS_SUCCESS;

minifs_fs_validate_end:
  return ret;
}

int minifs_fs_init(const struct fs_area *fap) {
  minifs_metadata_t metadata;
  int ret, sz;

  ret = fs_area_get_size(fap, &sz);
  if (ret != MINIFS_SUCCESS) {
    ret = MINIFS_FAILURE;
    goto minifs_fs_init_end;
  }

  ret = fs_area_read(fap, sz - sizeof(metadata), &metadata, sizeof(metadata));
  if (ret != MINIFS_SUCCESS) {
    ret = MINIFS_FS_READ_FAILURE;
    goto minifs_fs_init_end;
  }

  if (metadata.magic == MINIFS_MAGIC) {
    ret = MINIFS_SUCCESS;
    goto minifs_fs_init_end;
  }

  metadata.magic = MINIFS_MAGIC;
  ret = fs_area_write(fap, sz - sizeof(metadata), &metadata, sizeof(metadata));
  if (ret != MINIFS_SUCCESS) {
    ret = MINIFS_FS_WRITE_FAILURE;
    goto minifs_fs_init_end;
  }

  ret = MINIFS_SUCCESS;

minifs_fs_init_end:
  return ret;
}

int minifs_open(const struct fs_area *fap, const char *pathname, int flags) {
  return 0;
}

int minifs_creat(const struct fs_area *fap, const char *pathname,
                 uint8_t mode) {
  return 0;
}

int minifs_close(const struct fs_area *fap, int fd) { return 0; }

int minifs_read(const struct fs_area *fap, int fd, void *buf, size_t count) {
  return 0;
}

int minifs_write(const struct fs_area *fap, int fd, const void *buf,
                 size_t count) {
  return 0;
}