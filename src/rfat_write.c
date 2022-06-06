#include <string.h>

#include <rfat/rfat.h>

#include "helper.h"

static int find_avaliable_block(rfat_metadata_block_t *metadata) {
  int i, ret;

  ret = -1;
  for (i = 1; i < RFAT_MAX_FAT_CNT; i++) {
    if (metadata->fat_list[i].next == RFAT_FAT_UNUSED) {
      ret = i;
      break;
    }
  }

  return ret;
}

int rfat_write(const struct fs_area *fap, int fd, const void *buf,
               size_t count) {

  int ret, i, sz_to_be_write;
  rfat_metadata_block_t metadata;
  rfat_data_block_t data;
  int32_t fat_idx;
  size_t sz;

  ret = fs_area_get_size(fap, &sz);
  if (ret != RFAT_SUCCESS) {
    ret = RFAT_FAILURE;
    goto end;
  }

  WITH_METADATA_BLOCK(ret, fap, sz, &metadata, end, true, {
    fat_idx = metadata.fet_list[fd].entry_idx;
    if (fat_idx == RFAT_FAT_EOF) {
      fat_idx = find_avaliable_block(&metadata);
      if (fat_idx == -1) {
        ret = RFAT_FILE_UNEXPECTED_BLOCK_EOF_FAILURE;
        goto end;
      }
      metadata.fet_list[fd].entry_idx = fat_idx;
      metadata.fat_list[fat_idx].next = RFAT_FAT_EOF;
    }

    while (count > 0) {
      if (fat_idx == RFAT_FAT_UNUSED) {
        ret = RFAT_FILE_UNEXPECTED_BLOCK_UNUSED_FAILURE;
        goto end;
      }

      if (fat_idx == RFAT_FAT_RESERVED) {
        ret = RFAT_FILE_UNEXPECTED_BLOCK_RESERVED_FAILURE;
        goto end;
      }

      // write block
      WITH_DATA_BLOCK(ret, fap, sz, fat_idx, &data, end, true, {
        sz_to_be_write = count > RFAT_BLOCK_SZ ? RFAT_BLOCK_SZ : count;
        memcpy(&data, buf, sz_to_be_write);
        count = count - sz_to_be_write;
      });

      // alloc new block if there's still data to write and current block is EOF
      if (count > 0 && metadata.fat_list[fat_idx].next == RFAT_FAT_EOF) {
        i = find_avaliable_block(&metadata);
        if (i == -1) {
          ret = RFAT_FILE_UNEXPECTED_BLOCK_EOF_FAILURE;
          goto end;
        }

        metadata.fat_list[fat_idx].next = i;
        metadata.fat_list[i].next = RFAT_FAT_EOF;
      }

      fat_idx = metadata.fat_list[fat_idx].next;
    }

    // release unused blocks
    while (metadata.fat_list[fat_idx].next != RFAT_FAT_EOF) {
      i = metadata.fat_list[fat_idx].next;
      metadata.fat_list[fat_idx].next = RFAT_FAT_EOF;
      fat_idx = i;
    }
  });

  ret = RFAT_SUCCESS;

end:
  return ret;
}