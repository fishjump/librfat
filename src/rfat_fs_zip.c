#include <string.h>

#include <rfat/rfat.h>

#include "backend/backend.h"
#include "helper.h"

/**
 * @brief Find the first fragment(unused block) in the FAT, start from "from"
 *
 * @param fat_list[in] Pointer to the FAT list.
 * @param sz[in] Number of blocks in the whole filesystem.
 * @param from[in] Start from this block.
 * 
 * @return [int] The first fragment(unused block) in the FAT, or -1 if not found.
 */
static int find_fragment_index(const rfat_file_alloc_t *fat_list, size_t sz,
                               size_t from);

/**
 * @brief Find the first used(EOF or the next field is greater than 0) block in the FAT, start from "from"
 *
 * @param fat_list[in] Pointer to the FAT list.
 * @param sz[in] Number of blocks in the whole filesystem.
 * @param from[in] Start from this block.
 * @return [int] The first used(EOF or the next field is greater than 0) block in the FAT, or -1 if not found.
 */
static int find_block_index(const rfat_file_alloc_t *fat_list, size_t sz,
                            size_t from);

int rfat_fs_zip(const struct fs_area *fap) {
  int ret, dst_idx, src_idx;
  size_t sz;
  bool has_fragment;
  rfat_metadata_block_t metadata;
  rfat_data_block_t src, dst;

  ret = fs_area_get_size(fap, &sz);
  if (ret != RFAT_SUCCESS) {
    ret = RFAT_FAILURE;
    goto end;
  }

  WITH_METADATA_BLOCK(ret, fap, sz, &metadata, end, true, {
    dst_idx = 0;
    has_fragment = true;
    while (has_fragment) {
      // look for a fragment
      dst_idx = find_fragment_index(metadata.fat_list,
                                    metadata.header.block_cnt, dst_idx);
      if (dst_idx < 0) {
        ret = RFAT_SUCCESS;
        goto end;
      }

      // find the next non-empty block to the current index
      src_idx = find_block_index(metadata.fat_list, metadata.header.block_cnt,
                                 dst_idx);
      if (src_idx < 0) {
        ret = RFAT_SUCCESS;
        goto end;
      }

      // move block at src_idx to dst_idx
      metadata.fat_list[dst_idx].next = metadata.fat_list[src_idx].next;
      metadata.fat_list[src_idx].next = RFAT_FAT_UNUSED;

      WITH_DATA_BLOCK(ret, fap, sz, src_idx, &src, end, false, {});

      WITH_DATA_BLOCK(ret, fap, sz, dst_idx, &dst, end, true, {
        // also remember to update the data block
        memcpy(&dst, &src, RFAT_BLOCK_SZ);
      });
    }
  });

  ret = RFAT_SUCCESS;

end:
  return ret;
}

static int find_fragment_index(const rfat_file_alloc_t *fat_list, size_t sz,
                               size_t from) {
  int idx;

  for (idx = from; idx < sz; idx++) {
    if (fat_list[idx].next == RFAT_FAT_UNUSED) {
      return idx;
    }
  }

  return -1;
}

static int find_block_index(const rfat_file_alloc_t *fat_list, size_t sz,
                            size_t from) {
  int idx;

  for (idx = from; idx < sz; idx++) {
    if (fat_list[idx].next == RFAT_FAT_EOF || fat_list[idx].next > 0) {
      return idx;
    }
  }

  return -1;
}