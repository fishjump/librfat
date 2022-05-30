#ifndef _LIB_MINIFS_BACKEND_H_
#define _LIB_MINIFS_BACKEND_H_

#include <minifs/minifs.h>

#ifdef __cplusplus
extern "C" {
#endif

struct fs_area;

int fs_area_open(const uint64_t id, struct fs_area **fap);

int fs_area_close(const struct fs_area *fap);

int fs_area_read(const struct fs_area *fap, size_t offset, void *dst,
                 size_t len);

int fs_area_write(const struct fs_area *fap, size_t offset, const void *src,
                  size_t len);

#ifdef __cplusplus
}
#endif

#endif // _LIB_MINIFS_BACKEND_H_