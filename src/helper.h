#ifndef _LIBRFAT_HELPER_H_
#define _LIBRFAT_HELPER_H_

#include <rfat/rfat.h>

#define BLOCK(SZ, INDEX) ((SZ) - ((RFAT_BLOCK_SZ) * ((INDEX) + 1)))

#define WITH_BLOCK(RET, FAQ, SZ, INDEX, BLOCK_ADDR, ON_ERR, EXPR)              \
  do {                                                                         \
    RET = fs_area_read((FAQ), BLOCK((SZ), (INDEX)), (BLOCK_ADDR),              \
                       (RFAT_BLOCK_SZ));                                       \
    if (RET != RFAT_SUCCESS) {                                                 \
      RET = RFAT_FS_READ_FAILURE;                                              \
      goto ON_ERR;                                                             \
    }                                                                          \
                                                                               \
    EXPR;                                                                      \
                                                                               \
    RET = fs_area_write((FAQ), BLOCK((SZ), (INDEX)), (BLOCK_ADDR),             \
                        (RFAT_BLOCK_SZ));                                      \
    if (RET != RFAT_SUCCESS) {                                                 \
      RET = RFAT_FS_WRITE_FAILURE;                                             \
      goto ON_ERR;                                                             \
    }                                                                          \
  } while (0);

#endif // _LIBRFAT_HELPER_H_