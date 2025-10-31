//
//  integration_tests.cpp
//  SeekFS
//
//  Created by Максим Гоглов on 31.10.2025.
//

#include <gtest/gtest.h>
#include <cstdlib>
#include <fstream>

class IntegrationTest : public ::testing::Test {};

TEST_F(IntegrationTest, CommandLineHelp) {
    int result = std::system("./SeekFS --help");
    EXPECT_EQ(result, 0);
}

TEST_F(IntegrationTest, BasicSearch) {
    int result = std::system("./SeekFS -n \".*\\.txt\" --path test_dir");
    EXPECT_EQ(result, 0);
}
