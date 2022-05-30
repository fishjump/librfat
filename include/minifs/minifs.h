#ifndef _LIB_MINIFS_H_
#define _LIB_MINIFS_H_

#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct fs_area;

int minifs_fs_open(const uint64_t id, struct fs_area **fapp);

int minifs_fs_close(const struct fs_area *fap);

int minifs_open(const struct fs_area *fap, const char *pathname, int flags);

int minifs_creat(const struct fs_area *fap, const char *pathname, uint8_t mode);

int minifs_close(const struct fs_area *fap, int fd);

int minifs_read(const struct fs_area *fap, int fd, void *buf, size_t count);

int minifs_write(const struct fs_area *fap, int fd, const void *buf,
                 size_t count);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _LIB_MINIFS_H_