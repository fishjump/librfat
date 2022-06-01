#include <rfat/rfat.h>

#include "backend/backend.h"
#include "helper.h"

int rfat_fs_validate(const struct fs_area *fap) {
  rfat_metadata_block_t metadata;
  int ret;
  size_t sz;

  ret = fs_area_get_size(fap, &sz);
  if (ret != RFAT_SUCCESS) {
    ret = RFAT_FAILURE;
    goto end;
  }

  ret = fs_area_read(fap, BLOCK(sz, 0), &metadata, sizeof(metadata));

  if (ret != RFAT_SUCCESS) {
    ret = RFAT_FS_READ_FAILURE;
    goto end;
  }

  if (metadata.magic != RFAT_MAGIC) {
    ret = RFAT_FS_MAGIC_NUMBER_FAILURE;
    goto end;
  }

  ret = RFAT_SUCCESS;

end:
  return ret;
}