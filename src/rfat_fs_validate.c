#include <rfat/rfat.h>

#include "helper.h"

int rfat_fs_validate(const struct fs_area *fap) {
  rfat_metadata_header_t hdr;
  int ret;
  size_t sz;

  ret = fs_area_get_size(fap, &sz);
  if (ret != RFAT_SUCCESS) {
    ret = RFAT_FAILURE;
    goto end;
  }

  WITH_METADATA_HEADER(ret, fap, sz, &hdr, end, false, {
    if (hdr.magic != RFAT_MAGIC) {
      ret = RFAT_FS_MAGIC_NUMBER_FAILURE;
      goto end;
    }
  });

  ret = RFAT_SUCCESS;

end:
  return ret;
}