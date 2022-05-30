#ifndef _LIB_MINIFS_H_
#define _LIB_MINIFS_H_

#include <stddef.h>
#include <stdint.h>

#define MINIFS_MAGIC 0xb9
#define MINIFS_SUCCESS 0
#define MINIFS_FAILURE -1
#define MINIFS_FS_OPEN_FAILURE -2
#define MINIFS_FS_CLOSE_FAILURE -3
#define MINIFS_FS_READ_FAILURE -4
#define MINIFS_FS_WRITE_FAILURE -5
#define MINIFS_FS_MAGIC_NUMBER_FAILURE -6

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @brief fs_area is a implementation defined struct. Developers can get it by
 * using minifs_fs_open(), and remember to release it by using
 * minifs_fs_close().
 */
struct fs_area;

/**
 * @brief Open a minifs file system.
 *
 * @param id[in] Pointer to the id of a file system. The type is
 * implementation defined. For example, in posix backend, it is a file
 * path(const char *), and in zephyr backend, it is a interger.
 * @param fapp[out] Pointer to a fs_area pointer.
 * @return int return 0 if success, otherwise return negative error code.
 */
int minifs_fs_open(const void *id, struct fs_area **fapp);

/**
 * @brief Close a minifs file system.
 *
 * @param fap[in] Pointer to a fs_area variable.
 * @return int return 0 if success, otherwise return negative error code.
 */
int minifs_fs_close(const struct fs_area *fap);

/**
 * @brief Validate whether a opened fs_area pointer refers to a valid file
 * system or not. The file system must start with a specific magic number.
 *
 * @param fap[in] Pointer to a fs_area variable.
 * @return int return 0 if success, otherwise return negative error code.
 */
int minifs_fs_validate(const struct fs_area *fap);

/**
 * @brief Initialize a file system. If the file system is already initialized,
 * this function will do nothing.
 *
 * @param fap[in] Pointer to a fs_area variable.
 * @return int return 0 if success, otherwise return negative error code.
 */
int minifs_fs_init(const struct fs_area *fap);

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