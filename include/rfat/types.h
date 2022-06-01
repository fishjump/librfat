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
 * "next" points to the next item, except it equals to RFAT_UNUSED or RFAT_EOF.
 */
typedef struct rfat_file_alloc {
  int32_t next;
} __attribute__((__packed__)) rfat_file_alloc_t;

typedef struct rfat_file_entry {
  uint8_t name[10];
  uint8_t used : 1;
  uint8_t type : 3;
  uint8_t reserved0 : 4;
  uint8_t reserved1;
  uint32_t entry_idx;
} __attribute__((__packed__)) rfat_file_entry_t;

typedef struct rfat_metadata_block {
  // 16 bytes used
  uint32_t magic;
  uint32_t block_cnt;
  int32_t fat_entry_idx; // file allocation table start index
  int32_t fet_entry_idx; // file entry table start index

  // 16 bytes unused
  uint8_t reserved[16];
} __attribute__((__packed__)) rfat_metadata_block_t;

typedef struct rfat_fat_block {
  rfat_file_alloc_t data[RFAT_FAT_BLOCK_SZ];
} __attribute__((__packed__)) rfat_fat_block_t;

typedef struct rfat_fet_block {
  rfat_file_entry_t data[RFAT_FET_BLOCK_SZ];
} __attribute__((__packed__)) rfat_fet_block_t;

typedef struct rfat_data_block {
  uint8_t data[RFAT_DATA_BLOCK_SZ];
} __attribute__((__packed__)) rfat_data_block_t;

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