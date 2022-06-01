#include <rfat/rfat.h>

#include "backend/backend.h"

int rfat_fs_open(const void *id, struct fs_area **fapp) {
  int ret;

  ret = fs_area_open(id, fapp);
  if (ret != RFAT_SUCCESS) {
    ret = RFAT_FS_OPEN_FAILURE;
    goto end;
  }

  ret = RFAT_SUCCESS;

end:
  return ret;
}
