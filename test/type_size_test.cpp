#include <gtest/gtest.h>
#include <rfat/rfat.h>

TEST(type_size, type_size_le_test) {
  // The following list of types must be less than or equal to RFAT_BLOCK_SZ.
  // 1.rfat_file_alloc_t
  // 2.rfat_file_entry_t
  // 3.rfat_metadata_block_t
  // 4.rfat_fat_block_t
  // 5.rfat_fet_block_t
  // 6.rfat_data_block_t

  EXPECT_LE(sizeof(rfat_file_alloc_t), RFAT_BLOCK_SZ)
      << "rfat_file_alloc_t is too large";

  EXPECT_LE(sizeof(rfat_file_entry_t), RFAT_BLOCK_SZ)
      << "rfat_file_entry_t is too large";

  EXPECT_LE(sizeof(rfat_metadata_block_t), RFAT_BLOCK_SZ)
      << "rfat_metadata_block_t is too large";

  EXPECT_LE(sizeof(rfat_fat_block_t), RFAT_BLOCK_SZ)
      << "rfat_fat_block_t is too large";

  EXPECT_LE(sizeof(rfat_fet_block_t), RFAT_BLOCK_SZ)
      << "rfat_fet_block_t is too large";

  EXPECT_LE(sizeof(rfat_data_block_t), RFAT_BLOCK_SZ)
      << "rfat_data_block_t is too large";
}