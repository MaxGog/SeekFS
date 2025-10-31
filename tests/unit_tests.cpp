//
//  unit_tests.cpp
//  SeekFS
//
//  Created by Максим Гоглов on 31.10.2025.
//

#include <gtest/gtest.h>
#include "FileSearcher.h"
#include "HashCalculator.h"

class FileSearcherTest : public ::testing::Test {
protected:
    void SetUp() override {
        fs::create_directories("test_dir/subdir");
        std::ofstream("test_dir/file1.txt") << "test content";
        std::ofstream("test_dir/file2.txt") << "different content";
        std::ofstream("test_dir/subdir/file3.txt") << "test content";
    }
    
    void TearDown() override {
        fs::remove_all("test_dir");
    }
};

TEST_F(FileSearcherTest, SearchByName) {
    FileSearcher searcher("test_dir");
    auto results = searcher.searchByName("file.*\\.txt");
    EXPECT_EQ(results.size(), 3);
}

TEST_F(FileSearcherTest, SearchByContent) {
    FileSearcher searcher("test_dir");
    auto results = searcher.searchByContent("test");
    EXPECT_EQ(results.size(), 2);
}

TEST_F(FileSearcherTest, MD5Calculation) {
    auto hash1 = HashCalculator::calculateMD5("test_dir/file1.txt");
    auto hash2 = HashCalculator::calculateMD5("test_dir/file2.txt");
    auto hash3 = HashCalculator::calculateMD5("test_dir/subdir/file3.txt");
    
    EXPECT_EQ(hash1, hash3);
    EXPECT_NE(hash1, hash2);
}
