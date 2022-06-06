#include <stdlib.h>
#include <string.h>

#include <rfat/rfat.h>

#include "helper.h"

/**
 * @brief Setup rFAT filesystem metadata.
 *
 * @param metadata[inout] Pointer to the rFAT filesystem metadata block.
 * @param sz[in] Number of bytes in the whole filesystem.
 */
static void setup_metadata(rfat_metadata_block_t *metadata, const size_t sz);

int rfat_fs_init(const struct fs_area *fap) {
  int ret;
  size_t sz;
  rfat_metadata_block_t metadata;

  ret = fs_area_get_size(fap, &sz);
  if (ret != RFAT_SUCCESS) {
    ret = RFAT_FAILURE;
    goto end;
  }

  // setup metadata
  WITH_METADATA_BLOCK(ret, fap, sz, &metadata, end, true, {
    if (metadata.header.magic == RFAT_MAGIC) {
      ret = RFAT_SUCCESS;
      goto end;
    }

    setup_metadata(&metadata, sz / RFAT_BLOCK_SZ);
  });

  ret = RFAT_SUCCESS;

end:
  return ret;
}

static void setup_metadata(rfat_metadata_block_t *metadata, const size_t sz) {
  metadata->header.magic = RFAT_MAGIC;
  metadata->header.block_cnt =
      (sz - sizeof(rfat_metadata_block_t)) / RFAT_BLOCK_SZ;
  memset(metadata->fat_list, 0, sizeof(metadata->fat_list));
  memset(metadata->fet_list, 0, sizeof(metadata->fet_list));
}
