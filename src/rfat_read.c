#include <string.h>

#include <rfat/rfat.h>

#include "helper.h"

int rfat_read(const struct fs_area *fap, int fd, void *buf, size_t count) {
  int ret, i, loop_cnt, remain_cnt;
  rfat_metadata_block_t metadata;
  rfat_data_block_t data;
  int32_t fat_idx;
  size_t sz;

  ret = fs_area_get_size(fap, &sz);
  if (ret != RFAT_SUCCESS) {
    ret = RFAT_FAILURE;
    goto end;
  }

  WITH_METADATA_BLOCK(ret, fap, sz, &metadata, end, false, {});

  fat_idx = metadata.fet_list[fd].entry_idx;
  loop_cnt = count / RFAT_BLOCK_SZ;
  remain_cnt = count % RFAT_BLOCK_SZ;
  for (i = 0; i < loop_cnt; i++) {
    if (fat_idx == RFAT_FAT_UNUSED) {
      ret = RFAT_FILE_UNEXPECTED_BLOCK_UNUSED_FAILURE;
      goto end;
    }

    if (fat_idx == RFAT_FAT_EOF) {
      ret = RFAT_FILE_UNEXPECTED_BLOCK_EOF_FAILURE;
      goto end;
    }

    if (fat_idx == RFAT_FAT_RESERVED) {
      ret = RFAT_FILE_UNEXPECTED_BLOCK_RESERVED_FAILURE;
      goto end;
    }

    WITH_DATA_BLOCK(ret, fap, sz, fat_idx, &data, end, false, {});
    memcpy(buf + i * RFAT_BLOCK_SZ, &data, RFAT_BLOCK_SZ);
    fat_idx = metadata.fat_list[fat_idx].next;
  }

  if (remain_cnt == 0) {
    ret = RFAT_SUCCESS;
    goto end;
  }

  if (fat_idx == RFAT_FAT_UNUSED) {
    ret = RFAT_FILE_UNEXPECTED_BLOCK_UNUSED_FAILURE;
    goto end;
  }

  if (fat_idx == RFAT_FAT_EOF) {
    ret = RFAT_FILE_UNEXPECTED_BLOCK_EOF_FAILURE;
    goto end;
  }

  if (fat_idx == RFAT_FAT_RESERVED) {
    ret = RFAT_FILE_UNEXPECTED_BLOCK_RESERVED_FAILURE;
    goto end;
  }

  WITH_DATA_BLOCK(ret, fap, sz, fat_idx, &data, end, false, {});
  memcpy(buf + i * RFAT_BLOCK_SZ, &data, remain_cnt);

  ret = RFAT_SUCCESS;

end:
  return ret;
}
