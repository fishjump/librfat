#ifndef _LIB_MINIFS_BACKEND_H_
#define _LIB_MINIFS_BACKEND_H_

#include <minifs/minifs.h>

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

struct fs_area;

int fs_area_open(const void *id, struct fs_area **fap);

int fs_area_close(const struct fs_area *fap);

int fs_area_read(const struct fs_area *fap, size_t offset, void *dst,
                 size_t len);

int fs_area_write(const struct fs_area *fap, size_t offset, const void *src,
                  size_t len);

int fs_area_get_size(const struct fs_area *fap, size_t *sz);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _LIB_MINIFS_BACKEND_H_