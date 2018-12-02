//
// Created by cj on 2018/11/27.
//

#include "gtest/gtest.h"
#include "../../awesomeCC/include/strtools.h"

TEST(basic_check, test_eq) {
    EXPECT_EQ(string2int("1123"), 1123);

    EXPECT_EQ(string2double("11.5"), 11.5);
}
