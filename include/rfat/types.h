#ifndef _LIBRFAT_TYPES_H_
#define _LIBRFAT_TYPES_H_

#include <stdint.h>

#define RFAT_MAGIC (0x54414672u) /* "rFat" */
#define RFAT_BLOCK_SZ (32)       /* bytes */
#define RFAT_FAT_BLOCK_SZ (RFAT_BLOCK_SZ / sizeof(rfat_file_alloc_t))
#define RFAT_FET_BLOCK_SZ (RFAT_BLOCK_SZ / sizeof(rfat_file_entry_t))
#define RFAT_DATA_BLOCK_SZ (RFAT_BLOCK_SZ / sizeof(uint8_t))

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
 * literal meaning, and it cannot be longer than 9 characters(ends with '\0'),
 * entry_idx is the first FAT item index of this file. Details of other fields,
 * "used" and "type", are described below.
 */
typedef struct rfat_file_entry {
  uint8_t name[10];      // file name, ends with '\0'
  uint8_t used : 1;      // 1: used, 0: unused
  uint8_t type : 3;      // 0: image, 1: patch, 2: other file
  uint8_t reserved0 : 4; // reserved
  uint8_t reserved1;     // reserved
  uint32_t entry_idx;    // index of the entry in the file allocation table
} __attribute__((__packed__)) rfat_file_entry_t;

/**
 * @brief rfat_metadata_block is the type of a metadata block. It is always
 * placed at the end of rFAT file system(so called reversed FAT). The first 32
 * bits(4 bytes) is the magic number, then follows the block count which in type
 * of uint32_t.
 */
typedef struct rfat_metadata_block {
  uint32_t magic;        // magic number, must be "rFat"(0x54414672)
  uint32_t block_cnt;    // block count
  int32_t fat_entry_idx; // file allocation table start index
  int32_t fet_entry_idx; // file entry table start index
  uint8_t reserved[16];  // reserved for future use
} __attribute__((__packed__)) rfat_metadata_block_t;

/**
 * @brief rfat_fat_block is the type of a file allocation table block. Each item
 * describes the next block of current item or the control values like RFAT_EOF,
 * RFAT_RESERVED, and RFA_UNUSED. For example, if block[0].next = 2, block[2] is
 * the next block of block[0].
 */
typedef struct rfat_fat_block {
  rfat_file_alloc_t data[RFAT_FAT_BLOCK_SZ]; // FAT items
} __attribute__((__packed__)) rfat_fat_block_t;

/**
 * @brief rfat_fet_block is the type of a file entry table block. Each item
 * gives a file entry and its entry index. Goto rfat_file_entry_t description
 * for more details.
 */
typedef struct rfat_fet_block {
  rfat_file_entry_t data[RFAT_FET_BLOCK_SZ]; // FET items
} __attribute__((__packed__)) rfat_fet_block_t;

/**
 * @brief rfat_data_block is the type of a data block. It saves data in raw
 * format.
 */
typedef struct rfat_data_block {
  uint8_t data[RFAT_DATA_BLOCK_SZ]; // raw data
} __attribute__((__packed__)) rfat_data_block_t;

/**
 * @brief rfat_any_block is a union of all block types decleared above.
 */
typedef struct rfat_any_block {
  union {
    rfat_metadata_block_t metadata;
    rfat_fat_block_t fat;
    rfat_fet_block_t fet;
    rfat_data_block_t data;
  };
} __attribute__((__packed__)) rfat_any_block_t;

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