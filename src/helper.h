#ifndef _LIBRFAT_HELPER_H_
#define _LIBRFAT_HELPER_H_

#include <rfat/rfat.h>

#define METADATA_BLOCK(SZ) ((SZ) - sizeof(rfat_metadata_block_t))
#define DATA_BLOCK(SZ, INDEX)                                                  \
  ((SZ)-METADATA_BLOCK(SZ) - ((RFAT_BLOCK_SZ) * (INDEX)))

#define WITH_BLOCK(RET, FAP, OFFSET, ADDR, LEN, ON_ERR, SAVE, EXPR)            \
  do {                                                                         \
    RET = fs_area_read((FAP), (OFFSET), (ADDR), (LEN));                        \
    if (RET != RFAT_SUCCESS) {                                                 \
      RET = RFAT_FS_READ_FAILURE;                                              \
      goto ON_ERR;                                                             \
    }                                                                          \
                                                                               \
    EXPR;                                                                      \
                                                                               \
    if (SAVE) {                                                                \
      RET = fs_area_write((FAP), (OFFSET), (ADDR), (LEN));                     \
      if (RET != RFAT_SUCCESS) {                                               \
        RET = RFAT_FS_WRITE_FAILURE;                                           \
        goto ON_ERR;                                                           \
      }                                                                        \
    }                                                                          \
  } while (0);

#define WITH_METADATA_HEADER(RET, FAP, SZ, ADDR, ON_ERR, SAVE, EXPR)           \
  WITH_BLOCK(RET, FAP, METADATA_BLOCK(SZ), ADDR,                               \
             sizeof(rfat_metadata_header_t), ON_ERR, SAVE, EXPR)

#define WITH_METADATA_BLOCK(RET, FAP, SZ, ADDR, ON_ERR, SAVE, EXPR)            \
  WITH_BLOCK(RET, FAP, METADATA_BLOCK(SZ), ADDR,                               \
             sizeof(rfat_metadata_block_t), ON_ERR, SAVE, EXPR)

#define WITH_DATA_BLOCK(RET, FAP, SZ, INDEX, ADDR, ON_ERR, SAVE, EXPR)         \
  WITH_BLOCK(RET, FAP, DATA_BLOCK(SZ, INDEX), ADDR, sizeof(rfat_data_block_t), \
             ON_ERR, SAVE, EXPR)

#endif // _LIBRFAT_HELPER_H_