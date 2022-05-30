#include <minifs/minifs.h>

#include "backend/backend.h"

int minifs_fs_open(const uint64_t id, struct fs_area **fapp) {
  return fs_area_open(id, fapp);
}

int minifs_fs_close(const struct fs_area *fap) { return fs_area_close(fap); }

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