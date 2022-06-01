#include <stdlib.h>
#include <string.h>

#include <rfat/rfat.h>

#include "backend/backend.h"
#include "helper.h"

/**
 * @brief Setup rFAT filesystem metadata.
 *
 * @param metadata[inout] Pointer to the rFAT filesystem metadata block.
 * @param sz[in] Number of bytes in the whole filesystem.
 */
static void setup_metadata(rfat_metadata_block_t *metadata, const size_t sz);

/**
 * @brief Setup the first FAT block. The 0th FAT block will be marked as
 * reserved for metadata block. The boundary of FAT and FET will be determined
 * by the input indexes.
 *
 * @param block[inout] Pointer to the FAT block.
 * @param fat_start[in] The first FAT block index.
 * @param fet_start[in] The first FET block index.
 */
static void setup_file_alloc_table(rfat_fat_block_t *block,
                                   const int32_t fat_start,
                                   const int32_t fet_start);

/**
 * @brief Setup the first FET block. All items are set to "unused".
 *
 * @param block[inout] Pointer to the FET block.
 */
static void setup_file_entry_table(rfat_fet_block_t *block);

int rfat_fs_init(const struct fs_area *fap) {
  int ret;
  size_t sz;
  rfat_any_block_t block;
  int32_t fat_entry_idx, fet_entry_idx;

  ret = fs_area_get_size(fap, &sz);
  if (ret != RFAT_SUCCESS) {
    ret = RFAT_FAILURE;
    goto end;
  }

  // setup metadata
  WITH_BLOCK(ret, fap, sz, 0, &block, end, {
    if (block.metadata.magic == RFAT_MAGIC) {
      ret = RFAT_SUCCESS;
      goto end;
    }

    setup_metadata((rfat_metadata_block_t *)&block, sz / RFAT_BLOCK_SZ);

    fat_entry_idx = block.metadata.fat_entry_idx;
    fet_entry_idx = block.metadata.fet_entry_idx;
  });

  // setup file alloc table
  WITH_BLOCK(ret, fap, sz, fat_entry_idx, &block, end, {
    setup_file_alloc_table((rfat_fat_block_t *)&block, fat_entry_idx,
                           fet_entry_idx);
  });

  // setup file entry table
  WITH_BLOCK(ret, fap, sz, fet_entry_idx, &block, end, {
    // mark all items to "unused"
    setup_file_entry_table((rfat_fet_block_t *)&block);
  });

  ret = RFAT_SUCCESS;

end:
  return ret;
}

static void setup_metadata(rfat_metadata_block_t *metadata, const size_t sz) {
  metadata->magic = RFAT_MAGIC;
  metadata->block_cnt = sz / RFAT_BLOCK_SZ;
  metadata->fat_entry_idx = 1;
  metadata->fet_entry_idx = 2;
}

static void setup_file_alloc_table(rfat_fat_block_t *block,
                                   const int32_t fat_start,
                                   const int32_t fet_start) {
  int i;
  for (i = 0; i < RFAT_FAT_BLOCK_SZ; i++) {
    block->data[i].next = RFAT_FAT_UNUSED;
  }

  block->data[0].next = RFAT_FAT_RESERVED;
  block->data[fat_start].next = RFAT_FAT_EOF;
  block->data[fet_start].next = RFAT_FAT_EOF;
}

static void setup_file_entry_table(rfat_fet_block_t *block) {
  int i;
  for (i = 0; i < RFAT_FET_BLOCK_SZ; i++) {
    block->data[i].used = 0;
  }
}