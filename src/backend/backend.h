#ifndef _LIBRFAT_BACKEND_BACKEND_H_
#define _LIBRFAT_BACKEND_BACKEND_H_

#include <rfat/rfat.h>

#define BACKEND_SUCCESS (0)
#define BACKEND_FAILURE (-1)

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @brief open a rFAT file system on a implementation defined media.
 *
 * @param id[in] the id of the media, the type of id is implementation defined.
 * @param fap[out] the file access pointer of the opened file system, the
 * content is implementation defined.
 * @return [int] 0: success, -1: failure
 */
int fs_area_open(const void *id, struct fs_area **fap);

/**
 * @brief close a rFAT file system.
 *
 * @param fap[in] the file access pointer of the opened file system.
 * @return [int] 0: success, -1: failure
 */
int fs_area_close(const struct fs_area *fap);

/**
 * @brief read a block of raw data from the opened file system.
 *
 * @param fap[in] the file access pointer of the opened file system.
 * @param offset[in] the offset of the filesystem area to read.
 * @param dst[out] the destination buffer to store the read data.
 * @param len[in] the length of the data to read.
 * @return [int] 0: success, -1: failure
 */
int fs_area_read(const struct fs_area *fap, size_t offset, void *dst,
                 size_t len);

/**
 * @brief write a block of raw data to the opened file system.
 *
 * @param fap[in] the file access pointer of the opened file system.
 * @param offset[in] the offset of the filesystem area to write.
 * @param src[in] the source buffer to write.
 * @param len[in] the length of the data to write.
 * @return [int] 0: success, -1: failure
 */
int fs_area_write(const struct fs_area *fap, size_t offset, const void *src,
                  size_t len);

/**
 * @brief get the availalbe size of the opened file system.
 *
 * @param fap[in] the file access pointer of the opened file system.
 * @param sz[out] the available size of the opened file system.
 * @return [int] 0: success, -1: failure
 */
int fs_area_get_size(const struct fs_area *fap, size_t *sz);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _LIBRFAT_BACKEND_BACKEND_H_