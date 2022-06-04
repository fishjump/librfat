#include <string.h>

#include <rfat/rfat.h>

#include "backend/backend.h"
#include "helper.h"

static void init_fet_item(rfat_file_entry_t *entry, const char *name,
                          const uint8_t used, const uint8_t type,
                          const int32_t entry_idx) {
  if (entry == NULL || name == NULL) {
    return;
  }

  strcpy((char *)entry->name, name);
  entry->used = used;
  entry->type = type;
  entry->sz = 0;
  entry->entry_idx = entry_idx;
}

int rfat_create(const struct fs_area *fap, const char *name, uint8_t mode,
                rfat_file_entry_t *entry) {
  int ret, i, fat_idx, fet_idx;
  size_t sz;
  rfat_metadata_block_t metadata;

  ret = fs_area_get_size(fap, &sz);
  if (ret != RFAT_SUCCESS) {
    ret = RFAT_FAILURE;
    goto end;
  }

  WITH_METADATA_BLOCK(ret, fap, sz, &metadata, end, true, {
    fat_idx = -1;
    fet_idx = -1;

    // find first available file alloc entry
    for (i = 0; i < RFAT_MAX_FAT_CNT; i++) {
      if (metadata.fat_list[i].next == RFAT_FAT_UNUSED) {
        fat_idx = i;
        break;
      }
    }

    // find first available file entry
    for (i = 0; i < RFAT_MAX_FET_CNT; i++) {
      if (metadata.fet_list[i].used == 0) {
        fet_idx = i;
        break;
      }
    }

    // if have no free entry, return failure
    if (fat_idx == -1 || fet_idx == -1) {
      ret = RFAT_STORAGE_NOT_FOUND_FAILURE;
      goto end;
    }

    metadata.fat_list[fat_idx].next = RFAT_FAT_EOF;
    init_fet_item(&metadata.fet_list[fet_idx], name, true, mode, fat_idx);
    memcpy(entry, &metadata.fet_list[fet_idx], sizeof(rfat_file_entry_t));
  });

  ret = RFAT_SUCCESS;

end:
  return ret;
}