#ifndef _LIBRFAT_TEST_HELPER_HPP_
#define _LIBRFAT_TEST_HELPER_HPP_

#define RFAT_BIN_FILE_PATH "rfat_fs_test.bin"
#define RFAT_BIN_FILE_SZ (RFAT_MAX_FAT_CNT * RFAT_BLOCK_SZ)

#define __MERGE(x, y) x_##y
#define _MERGE(x, y) __MERGE(x, y)
#define UNIQUE(x) _MERGE(x, __COUNTER__)

#define defer(expr) _defer_t UNIQUE(_defer) = [&]() { expr; }

template <class FUNC_T> struct _defer_t {
  FUNC_T func;
  _defer_t(FUNC_T f) : func(f) {}
  ~_defer_t() { func(); }
};

#endif // _LIBRFAT_TEST_HELPER_HPP_