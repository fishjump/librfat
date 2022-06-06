#include <string.h>

#include <rfat/rfat.h>

#include "helper.h"

static void init_fet_item(rfat_file_entry_t *entry, const char *name,
                          const uint8_t used, const uint8_t type) {
  if (entry == NULL || name == NULL) {
    return;
  }

  strcpy((char *)entry->name, name);
  entry->used = used;
  entry->type = type;
  entry->sz = 0;
  entry->entry_idx = RFAT_FAT_EOF;
}

int rfat_create(const struct fs_area *fap, const char *name, uint8_t mode,
                int *fd) {
  int ret, i, fet_idx;
  size_t sz;
  rfat_metadata_block_t metadata;

  ret = fs_area_get_size(fap, &sz);
  if (ret != RFAT_SUCCESS) {
    ret = RFAT_FAILURE;
    goto end;
  }

  WITH_METADATA_BLOCK(ret, fap, sz, &metadata, end, true, {
    fet_idx = -1;

    // find first available file entry
    for (i = 0; i < RFAT_MAX_FET_CNT; i++) {
      if (metadata.fet_list[i].used == 0) {
        fet_idx = i;
        break;
      }
    }

    // if have no free entry, return failure
    if (fet_idx == -1) {
      ret = RFAT_STORAGE_NOT_FOUND_FAILURE;
      goto end;
    }

    init_fet_item(&metadata.fet_list[fet_idx], name, true, mode);
    *fd = fet_idx;
  });

  ret = RFAT_SUCCESS;

end:
  return ret;
}