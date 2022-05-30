#ifndef _LIB_MINIFS_TYPEDEFS_H_
#define _LIB_MINIFS_TYPEDEFS_H_

#include <stdint.h>

#include "backend/backend.h"

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus

/**
 * @brief Minifs descriptor, it has two partitions. The first one is for image
 * storage, the second one is for patches storage. The first partition grows
 * from lower offset to higher. The second partition grows from higher offset to
 * lower.
 * +------------------+--------+-------------------------+----------+
 * | Partition 0 -->  |        |     <-- Partition 1     |          |
 * +------------------+ Unused +-----+---------+---------+ Metadata |
 * |   Image storage  |        | ... | Block 1 | Block 0 |          |
 * +------------------+--------+-----+---------+---------+----------+
 */
typedef struct __attribute__((__packed__)) minifs_metadata {
  uint8_t magic;
  size_t fs_sz;
  size_t partition0_sz;
  size_t partition1_sz;
  size_t partition0_start;
  size_t partition1_start;
} minifs_metadata_t;

/**
 * @brief The main block of minifs partition 1. The main block size must be
 * fixed and must be located at block 0, because we want find it as easy as
 * possible. The size is 128 bits in total.
 */
typedef struct __attribute__((__packed__)) main_block {
  uint8_t magic;
  uint16_t id;
  uint32_t sz;
  uint8_t flags;
  uint8_t data[8];
} main_block_t;

/**
 * @brief Regular block definition. Data is declared as a zero-size array, the
 * length of which is defined by the sz field, i.e. it is an intentional
 * overflow.
 */
typedef struct __attribute__((__packed__)) block {
  uint8_t magic;
  uint16_t id;
  uint32_t sz;
  uint8_t flags;
  uint8_t data[0];
} block_t;

#ifdef __cplusplus
}
#endif // __cplusplus

#endif // _LIB_MINIFS_TYPEDEFS_H_