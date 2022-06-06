#ifndef _LIBRFAT_RFAT_H_
#define _LIBRFAT_RFAT_H_

#include "types.h"

#define RFAT_SUCCESS (0)
#define RFAT_FAILURE (-1)
#define RFAT_FS_OPEN_FAILURE (-2)
#define RFAT_FS_CLOSE_FAILURE (-3)
#define RFAT_FS_READ_FAILURE (-4)
#define RFAT_FS_WRITE_FAILURE (-5)
#define RFAT_FS_MAGIC_NUMBER_FAILURE (-6)
#define RFAT_STORAGE_NOT_FOUND_FAILURE (-7)
#define RFAT_FILE_DOES_NOT_EXIST_FAILURE (-8)
#define RFAT_FILE_UNEXPECTED_BLOCK_UNUSED_FAILURE (-9)
#define RFAT_FILE_UNEXPECTED_BLOCK_EOF_FAILURE (-10)
#define RFAT_FILE_UNEXPECTED_BLOCK_RESERVED_FAILURE (-11)

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @brief Open a rfat file system.
 *
 * @param id[in] Pointer to the id of a file system. The type is
 * implementation defined. For example, in posix backend, it is a file
 * path(const char *), and in zephyr backend, it is a interger.
 * @param fapp[out] Pointer to a fs_area pointer.
 * @return [int] return 0 if success, otherwise return negative error code.
 */
int rfat_fs_open(const void *id, struct fs_area **fapp);

/**
 * @brief Close a rfat file system.
 *
 * @param fap[in] Pointer to a fs_area variable.
 * @return [int] return 0 if success, otherwise return negative error code.
 */
int rfat_fs_close(const struct fs_area *fap);

/**
 * @brief Validate whether a opened fs_area pointer refers to a valid file
 * system or not. The file system must start with a specific magic number.
 *
 * @param fap[in] Pointer to a fs_area variable.
 * @return [int] return 0 if success, otherwise return negative error code.
 */
int rfat_fs_validate(const struct fs_area *fap);

/**
 * @brief Initialize a file system. If the file system is already initialized,
 * this function will do nothing.
 *
 * @param fap[in] Pointer to a fs_area variable.
 * @return [int] return 0 if success, otherwise return negative error code.
 */
int rfat_fs_init(const struct fs_area *fap);

/**
 * @brief Open a file. If a file is not exist, return failure.
 *
 * @param fap[in] Pointer to a fs_area variable.
 * @param name[in] Pointer to a file name.
 * @param fd[out] Pointer to a file id.
 * @return [int] return 0 if success, otherwise return negative error code.
 */
int rfat_open(const struct fs_area *fap, const char *name, int *fd);

/**
 * @brief Create a file.
 *
 * @param fap[in] Pointer to a fs_area variable.
 * @param name[in] Pointer to a file name.
 * @param mode[in] File mode.
 * @param fd[out] Pointer to a file id.
 * @return [int] return 0 if success, otherwise return negative error code.
 */
int rfat_create(const struct fs_area *fap, const char *name, uint8_t mode,
                int *fd);

/**
 * @brief CLose a file. If the pointer is NULL, do nothing.
 *
 * @param fap[in] Pointer to a fs_area.
 * @param fd[in] File id.
 * @return [int] return 0 if success, otherwise return negative error code.
 */
int rfat_close(const struct fs_area *fap, int fd);

int rfat_read(const struct fs_area *fap, int fd, void *buf, size_t count);

int rfat_write(const struct fs_area *fap, int fd, const void *buf,
               size_t count);

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _LIBRFAT_RFAT_H_