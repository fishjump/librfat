#ifndef _LIBRFAT_TYPES_H_
#define _LIBRFAT_TYPES_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/**
 * @note Please note that the RFAT_BLOCK_SZ * RFAT_MAX_FAT_LEN should be
 * greater than or equal to the maximum possible storage size of your media. In
 * this case, I write this fs for Nordic nRF52 which has 512KB storage.
 */

#define RFAT_MAGIC (0x54414672u)                             /* "rFat" */
#define RFAT_BLOCK_SZ (64)                                   /* bytes */
#define RFAT_MAX_FAT_CNT (8192)                              /* blocks */
#define RFAT_MAX_FET_CNT (255)                               /* files */
#define RFAT_DATA_BLOCK_SZ (RFAT_BLOCK_SZ / sizeof(uint8_t)) /* bytes */

#define RFAT_FAT_UNUSED (0)
#define RFAT_FAT_EOF (-1)
#define RFAT_FAT_RESERVED (-2)

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @brief rfat_file_alloc is the item type of a file allocation table. Field
 * "next" points to the next item, except it equals to RFAT_UNUSED, RFAT_EOF, or
 * RFAT_RESERVED.
 */
typedef struct rfat_file_alloc {
  int32_t next; // index of next item or control value
} __attribute__((__packed__)) rfat_file_alloc_t;

/**
 * @brief rfat_file_entry describes a file entry. The "name" field is quite the
 * literal meaning, and it cannot be longer than 22 characters(ends with '\0'),
 * entry_idx is the first FAT item index of this file. Details of other fields,
 * "used" and "type", are described below.
 */
typedef struct rfat_file_entry {
  uint8_t name[23];      // file name, ends with '\0'
  uint8_t used : 1;      // 1: used, 0: unused
  uint8_t type : 3;      // 0: image, 1: patch, 2: other file
  uint8_t reserved0 : 4; // reserved
  uint32_t sz;           // file size in bytes
  int32_t entry_idx;     // index of the entry in the file allocation table
} __attribute__((__packed__)) rfat_file_entry_t;

/**
 * @brief  rfat_metadata_header_t is the header type of rFAT metadata block. The
 * first 32 bits(4 bytes) is the magic number, then follows the block count.
 */
typedef struct rfat_metadata_header {
  uint32_t magic;    // magic number, must be "rFat"(0x54414672)
  int32_t block_cnt; // block count
} __attribute__((__packed__)) rfat_metadata_header_t;

/**
 * @brief rfat_metadata_block is the type of a metadata block. It is always
 * placed at the end of rFAT file system(so called reversed FAT).
 */
typedef struct rfat_metadata_block {
  rfat_metadata_header_t header;
  rfat_file_entry_t fet_list[RFAT_MAX_FET_CNT];
  rfat_file_alloc_t fat_list[RFAT_MAX_FAT_CNT];
} __attribute__((__packed__)) rfat_metadata_block_t;

/**
 * @brief rfat_data_block is the type of a data block. It saves data in raw
 * format.
 */
typedef struct rfat_data_block {
  uint8_t data[RFAT_DATA_BLOCK_SZ]; // raw data
} __attribute__((__packed__)) rfat_data_block_t;

/**
 * @brief fs_area is a implementation defined struct. Developers can get it by
 * using rfat_fs_open(), and remember to release it by using
 * rfat_fs_close().
 */
struct fs_area;

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _LIBRFAT_TYPES_H_