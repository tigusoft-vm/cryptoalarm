#include <limits.h>
#include "gtest/gtest.h"

bool check() { 
	return true;
}

TEST(IsPrimeTest, Positive) {
  EXPECT_FALSE(check());
  EXPECT_TRUE(check());
  EXPECT_FALSE(check());
  EXPECT_TRUE(check());

}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
