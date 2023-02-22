#include "stacktrace.h"
#include <cassert>
#include <map>
#include <gtest/gtest.h>

[[nodiscard]] constexpr auto foo(int const val) noexcept -> bool{
	return 42 == val;
}

consteval void static_tests() noexcept{
	//Success case
	static_assert(foo(42), "My Error 1");
	
	//Failure cases
	static_assert(!foo(0), "My Error 2");
	static_assert(!foo(INT_MAX), "My Error 3");
	static_assert(!foo(INT_MIN), "My Error 4");
	

}

[[nodiscard]] auto main() noexcept -> int
{
	std::cout << "Begin" << std::endl;

	testing::InitGoogleTest();
	return RUN_ALL_TESTS();
	  
	//std::cout << "End" << std::endl;
	
}

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
  
  // Expect equality.
  EXPECT_EQ(6 * 6, 36);
  
  // Expect equality.
  EXPECT_EQ(5 * 5, 25);
  
  EXPECT_EQ(foo(42), true);
  //EXPECT_EQ(foo(42), false);
}

/*
Output


End

*/