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

namespace fs = std::filesystem;

class HashCalculator {
public:
    static std::string calculateFileSizeHash(const fs::path& filePath);
};
