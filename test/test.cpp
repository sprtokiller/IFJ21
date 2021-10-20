#include "gtest/gtest.h"

TEST(widget, ok) {
	ASSERT_EQ(1, 1);
}

TEST(testy, not_ok) {
	ASSERT_EQ(1, 0);
}
