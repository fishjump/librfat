#include <string.h>

#include <rfat/rfat.h>

#include "helper.h"

int rfat_open(const struct fs_area *fap, const char *name, int *fd) {
  int ret, i, fat_idx, fet_idx;
  size_t sz;
  rfat_metadata_block_t metadata;

  ret = fs_area_get_size(fap, &sz);
  if (ret != RFAT_SUCCESS) {
    ret = RFAT_FAILURE;
    goto end;
  }

  WITH_METADATA_BLOCK(ret, fap, sz, &metadata, end, false, {
    fet_idx = -1;
    for (i = 0; i < RFAT_MAX_FET_CNT; i++) {
      if (strcmp((const char *)metadata.fet_list[i].name, name) == 0) {
        fet_idx = i;
        break;
      }
    }

    if (fet_idx == -1) {
      ret = RFAT_FILE_DOES_NOT_EXIST_FAILURE;
      goto end;
    }

    *fd = fet_idx;
  });

  ret = RFAT_SUCCESS;

end:
  return ret;
}