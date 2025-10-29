//
//  HashCalculator.cpp
//  SeekFS
//
//  Created by Максим Гоглов on 29.10.2025.
//
#pragma once
#include "HashCalculator.h"
#include <string>
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iomanip>

namespace fs = std::filesystem;

std::string HashCalculator::calculateFileSizeHash(const fs::path& filePath) {
    auto fileSize = fs::file_size(filePath);
    std::stringstream ss;
    ss << fileSize << "_" << filePath.filename().string();
    return ss.str();
}
