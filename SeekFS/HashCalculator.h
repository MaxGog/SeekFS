//
//  HashCalculator.h
//  SeekFS
//
//  Created by Максим Гоглов on 29.10.2025.
//
#pragma once
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <cstdint>

namespace fs = std::filesystem;

class MD5 {
public:
    MD5();
    void update(const unsigned char* data, size_t length);
    void update(const std::string& data);
    std::string final();
    static std::string calculate(const std::string& data);
    static std::string calculateFile(const fs::path& filePath);
    
private:
    void transform(const unsigned char block[64]);
    void encode(const uint32_t* input, unsigned char* output, size_t length);
    static void decode(const unsigned char* input, uint32_t* output, size_t length);
    
    uint32_t state[4];
    uint32_t count[2];
    unsigned char buffer[64];
    bool finalized;
};

class HashCalculator {
public:
    static std::string calculateMD5(const fs::path& filePath);
    static std::string calculateFileSizeHash(const fs::path& filePath);
};
