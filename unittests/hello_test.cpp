#include <gtest/gtest.h>

// Demonstrate some basic assertions.
TEST(SeconHelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello2", "world2");
  // Expect two strings not to be equal.
  EXPECT_STRNE("world2", "world3");
  // Expect equality.
  EXPECT_EQ(6 * 6, 36);
}