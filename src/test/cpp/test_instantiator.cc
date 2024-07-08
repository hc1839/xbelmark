#include <gtest/gtest.h>

int main(int argc, char *argv[]) {
  ::testing::InitGoogleTest(&argc, argv);
  const int error_status = RUN_ALL_TESTS();
  return error_status;
}
