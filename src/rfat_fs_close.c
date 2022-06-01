#include <rfat/rfat.h>

#include "backend/backend.h"

int rfat_fs_close(const struct fs_area *fap) {
  int ret;

  ret = fs_area_close(fap);
  if (ret != RFAT_SUCCESS) {
    ret = RFAT_FS_CLOSE_FAILURE;
    goto end;
  }

end:
  return ret;
}
