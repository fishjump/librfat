#include <gtest/gtest.h>
#include <rfat/rfat.h>

#define RFAT_BIN_FILE_PATH "rfat_fs_test.bin"
#define RFAT_BIN_FILE_SZ (1024)

class rfat_fs : public ::testing::Test {
protected:
  void SetUp() override {
    FILE *f;
    int i, ret;

    f = fopen(RFAT_BIN_FILE_PATH, "w");
    ASSERT_NE(f, (void *)NULL) << "Failed to open " << RFAT_BIN_FILE_PATH;

    for (i = 0; i < RFAT_BIN_FILE_SZ; i++) {
      fwrite("\0", 1, 1, f);
    }

    ret = fclose(f);

    ASSERT_EQ(ret, 0) << "Failed to close file";
  }

  void TearDown() override {
    int ret;

    ret = remove(RFAT_BIN_FILE_PATH);

    ASSERT_EQ(ret, 0) << "remove " << RFAT_BIN_FILE_PATH << " failed";
  }
};

TEST_F(rfat_fs, open_and_close) {
  int ret;
  struct fs_area *fap;

  ret = rfat_fs_open(RFAT_BIN_FILE_PATH, &fap);

  ASSERT_EQ(ret, RFAT_SUCCESS) << "rfat_fs_open failed, error code: " << ret;

  ret = rfat_fs_close(fap);

  ASSERT_EQ(ret, RFAT_SUCCESS) << "rfat_fs_close failed, error code: " << ret;
}

TEST_F(rfat_fs, init_and_validate) {
  int ret, i;
  struct fs_area *fap;

  ret = rfat_fs_open(RFAT_BIN_FILE_PATH, &fap);

  ASSERT_EQ(ret, RFAT_SUCCESS) << "rfat_fs_open failed, error code: " << ret;

  ret = rfat_fs_validate(fap);

  ASSERT_EQ(ret, RFAT_FS_MAGIC_NUMBER_FAILURE)
      << "rfat_fs_validate failed, error code: " << ret;

  ret = rfat_fs_init(fap);

  ASSERT_EQ(ret, RFAT_SUCCESS) << "rfat_fs_init failed, error code: " << ret;

  ret = rfat_fs_validate(fap);

  ASSERT_EQ(ret, RFAT_SUCCESS)
      << "rfat_fs_validate failed, error code: " << ret;
}