#include <gtest/gtest.h>
#include <rfat/rfat.h>

/**
 * @brief The following list of types must be less than or equal to
 * RFAT_BLOCK_SZ.
 * 1. rfat_file_alloc_t
 * 2. rfat_file_entry_t
 */
TEST(type_size, type_size_le_test) {
  EXPECT_LE(sizeof(rfat_file_alloc_t), RFAT_BLOCK_SZ)
      << "rfat_file_alloc_t is too large, expected <= " << RFAT_BLOCK_SZ;

  EXPECT_LE(sizeof(rfat_file_entry_t), RFAT_BLOCK_SZ)
      << "rfat_file_entry_t is too large, expected <= " << RFAT_BLOCK_SZ;
}

/**
 * @brief The following list of types must extactly equal to RFAT_BLOCK_SZ.
 * 1. rfat_metadata_block_t
 * 2. rfat_fat_block_t
 * 3. rfat_fet_block_t
 * 4. rfat_data_block_t
 */
TEST(type_size, type_size_eq_test) {
  EXPECT_EQ(sizeof(rfat_metadata_block_t), RFAT_BLOCK_SZ)
      << "rfat_metadata_block_t is expected to be exactly " << RFAT_BLOCK_SZ;

  EXPECT_EQ(sizeof(rfat_fat_block_t), RFAT_BLOCK_SZ)
      << "rfat_fat_block_t is expected to be exactly " << RFAT_BLOCK_SZ;

  EXPECT_EQ(sizeof(rfat_fet_block_t), RFAT_BLOCK_SZ)
      << "rfat_fet_block_t is expected to be exactly " << RFAT_BLOCK_SZ;

  EXPECT_EQ(sizeof(rfat_data_block_t), RFAT_BLOCK_SZ)
      << "rfat_data_block_t is expected to be exactly " << RFAT_BLOCK_SZ;
}