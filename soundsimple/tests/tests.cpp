#include <limits.h>
#include "gtest/gtest.h"

#include "../src/libs.h"
#include "../src/cSound.h"

using namespace std;

cSound sound(true);

TEST(globTest, Positive) {


}

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
