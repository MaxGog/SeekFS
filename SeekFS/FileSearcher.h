//
//  FileSearcher.h
//  SeekFS
//
//  Created by Максим Гоглов on 29.10.2025.
//
#pragma once
#include <vector>
#include <string>
#include <filesystem>
#include <regex>
#include <unordered_map>
#include <future>
#include <atomic>
#include <iostream>
#include "HashCalculator.h"
#include "ProgressVisualizer.h"
#include "GraphicsUtils.h"
#include "Spinner.h"

namespace fs = std::filesystem;

class FileSearcher {
public:
    FileSearcher(const std::string& root_path, int num_threads = 4, bool show_progress = false);
    
    std::vector<std::string> searchByName(const std::string& pattern);
    std::vector<std::string> searchByContent(const std::string& pattern);
    std::unordered_map<std::string, std::vector<std::string>> findDuplicates();
    
    void setCaseSensitive(bool sensitive) { case_sensitive_ = sensitive; }
    void setMaxFileSize(size_t max_size) { max_file_size_ = max_size; }
    void setFileTypes(const std::vector<std::string>& types) { file_types_ = types; }
    
private:
    fs::path root_path_;
    int num_threads_;
    bool case_sensitive_ = true;
    bool show_progress_ = false;
    size_t max_file_size_ = 100 * 1024 * 1024;
    std::vector<std::string> file_types_;
    
    std::vector<fs::path> collectAllFiles();
    bool matchesFileType(const fs::path& file);
    std::vector<std::string> processBatch(
        const std::vector<fs::path>& batch,
        const std::function<bool(const fs::path&)>& predicate
    );
    
    template<typename Func>
    std::vector<std::string> processParallel(const std::vector<fs::path>& files, Func func);
};
