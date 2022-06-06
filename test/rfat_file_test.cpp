#include <gtest/gtest.h>
#include <rfat/rfat.h>

#include "helper.hpp"

/**
 * @brief This test suite is used to test rfat file system. In Setup(), we'll
 * create a new binary file with size=RFA_BIN_FILE_SZ, and it will be deleted in
 * TearsDown().
 */
class rfat_file : public ::testing::Test {
protected:
  struct fs_area *fap;

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

    ret = rfat_fs_open(RFAT_BIN_FILE_PATH, &fap);
    ASSERT_EQ(ret, RFAT_SUCCESS) << "rfat_fs_open failed, error code: " << ret;

    ret = rfat_fs_init(fap);

    ASSERT_EQ(ret, RFAT_SUCCESS) << "rfat_fs_init failed, error code: " << ret;

    ret = rfat_fs_validate(fap);

    ASSERT_EQ(ret, RFAT_SUCCESS)
        << "rfat_fs_validate failed, error code: " << ret;
  }

  void TearDown() override {
    int ret;

    ret = rfat_fs_close(fap);
    ASSERT_EQ(ret, RFAT_SUCCESS) << "rfat_fs_close failed, error code: " << ret;

#ifdef TEST_CLEANUP
    ret = remove(RFAT_BIN_FILE_PATH);

    ASSERT_EQ(ret, 0) << "remove " << RFAT_BIN_FILE_PATH << " failed";
#endif // TEST_CLEANUP
  }

  int test;
};

TEST_F(rfat_file, create_and_read) {
  int ret, fd;

  ret = rfat_create(fap, "helloworld.txt", 0, &fd);
  ASSERT_EQ(ret, RFAT_SUCCESS) << "rfat_create failed, error code: " << ret;

  ret = rfat_close(fap, fd);
  ASSERT_EQ(ret, RFAT_SUCCESS) << "rfat_close failed, error code: " << ret;

  ret = rfat_open(fap, "helloworld.txt", &fd);
  ASSERT_EQ(ret, RFAT_SUCCESS) << "rfat_create failed, error code: " << ret;

  ret = rfat_close(fap, fd);
  ASSERT_EQ(ret, RFAT_SUCCESS) << "rfat_close failed, error code: " << ret;
}

TEST_F(rfat_file, open_non_exist_file) {
  int ret, fd;

  ret = rfat_open(fap, "itdoesnotexist.txt", &fd);
  ASSERT_EQ(ret, RFAT_FILE_DOES_NOT_EXIST_FAILURE)
      << "rfat_open failed, error code: " << ret;
}

TEST_F(rfat_file, write_and_read) {
  int ret, fd;
  const char buf[13] = "hello world!";
  char buf2[13] = "";

  ret = rfat_create(fap, "helloworld.txt", 0, &fd);
  ASSERT_EQ(ret, RFAT_SUCCESS) << "rfat_create failed, error code: " << ret;

  ret = rfat_write(fap, fd, buf, sizeof(buf));
  ASSERT_EQ(ret, RFAT_SUCCESS) << "rfat_write failed, error code: " << ret;

  ret = rfat_close(fap, fd);
  ASSERT_EQ(ret, RFAT_SUCCESS) << "rfat_close failed, error code: " << ret;

  ret = rfat_open(fap, "helloworld.txt", &fd);
  ASSERT_EQ(ret, RFAT_SUCCESS) << "rfat_open failed, error code: " << ret;

  ret = rfat_read(fap, fd, buf2, sizeof(buf2));
  ASSERT_EQ(ret, RFAT_SUCCESS) << "rfat_read failed, error code: " << ret;
  ASSERT_EQ(strcmp(buf, buf2), 0)
      << "rfat_read result inconsistency, expected: " << buf
      << ", actual: " << buf2;

  ret = rfat_close(fap, fd);
  ASSERT_EQ(ret, RFAT_SUCCESS) << "rfat_close failed, error code: " << ret;
}
