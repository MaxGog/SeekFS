//
//  FileSearcher.cpp
//  SeekFS
//
//  Created by Максим Гоглов on 29.10.2025.
//

#include "FileSearcher.h"
#include <algorithm>
#include <iterator>

FileSearcher::FileSearcher(const std::string& root_path, int num_threads, bool show_progress)
    : root_path_(root_path), num_threads_(std::max(1, num_threads)), show_progress_(show_progress) {
    
    if (!fs::exists(root_path_)) {
        throw std::runtime_error("Path does not exist: " + root_path);
    }
}

std::vector<fs::path> FileSearcher::collectAllFiles() {
    std::vector<fs::path> files;
    
    if (show_progress_) {
        GraphicsUtils::printSection("Scanning directory structure...");
    }
    
    try {
        size_t count = 0;
        for (const auto& entry : fs::recursive_directory_iterator(
            root_path_,
            fs::directory_options::skip_permission_denied
        )) {
            if (entry.is_regular_file() &&
                entry.file_size() <= max_file_size_ &&
                matchesFileType(entry.path())) {
                files.push_back(entry.path());
                count++;
                
                if (show_progress_ && count % 100 == 0) {
                    std::cout << "\rFound " << count << " files..." << std::flush;
                }
            }
        }
        
        if (show_progress_) {
            std::cout << "\rFound " << files.size() << " files total." << std::endl;
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
    
    return files;
}

bool FileSearcher::matchesFileType(const fs::path& file) {
    if (file_types_.empty()) return true;
    
    std::string extension = file.extension().string();
    if (extension.empty()) return false;
    
    if (!extension.empty() && extension[0] == '.') {
        extension = extension.substr(1);
    }
    
    return std::find(file_types_.begin(), file_types_.end(), extension) != file_types_.end();
}

std::vector<std::string> FileSearcher::processBatch(
    const std::vector<fs::path>& batch,
    const std::function<bool(const fs::path&)>& predicate) {
    
    std::vector<std::string> results;
    for (const auto& file : batch) {
        if (predicate(file)) {
            results.push_back(file.string());
        }
    }
    return results;
}

template<typename Func>
std::vector<std::string> FileSearcher::processParallel(
    const std::vector<fs::path>& files, Func func) {
    
    if (files.empty()) {
        return {};
    }
    
    size_t batch_size = std::max(size_t(1), files.size() / num_threads_);
    std::vector<std::vector<fs::path>> batches;
    
    for (size_t i = 0; i < files.size(); i += batch_size) {
        auto end = std::next(files.begin(),
                           std::min(i + batch_size, files.size()));
        batches.emplace_back(files.begin() + i, end);
    }
    
    std::vector<std::future<std::vector<std::string>>> futures;
    for (const auto& batch : batches) {
        futures.push_back(std::async(std::launch::async,
            [this, &func, &batch]() {
                return processBatch(batch, func);
            }));
    }
    
    std::vector<std::string> results;
    for (auto& future : futures) {
        auto batch_results = future.get();
        results.insert(results.end(),
                      batch_results.begin(), batch_results.end());
    }
    
    return results;
}

std::vector<std::string> FileSearcher::searchByName(const std::string& pattern) {
    auto files = collectAllFiles();
    
    std::regex re;
    try {
        auto flags = case_sensitive_ ? std::regex_constants::ECMAScript
                                     : std::regex_constants::icase;
        re = std::regex(pattern, flags);
    } catch (const std::regex_error& e) {
        throw std::runtime_error("Invalid regex pattern: " + std::string(e.what()));
    }
    
    return processParallel(files, [&re](const fs::path& file) {
        return std::regex_search(file.filename().string(), re);
    });
}

std::vector<std::string> FileSearcher::searchByContent(const std::string& pattern) {
    auto files = collectAllFiles();
    
    std::regex re;
    try {
        auto flags = case_sensitive_ ? std::regex_constants::ECMAScript
                                     : std::regex_constants::icase;
        re = std::regex(pattern, flags);
    } catch (const std::regex_error& e) {
        throw std::runtime_error("Invalid regex pattern: " + std::string(e.what()));
    }
    
    return processParallel(files, [&re](const fs::path& file) {
        try {
            std::ifstream stream(file);
            if (!stream) return false;
            
            std::string line;
            while (std::getline(stream, line)) {
                if (std::regex_search(line, re)) {
                    return true;
                }
            }
        } catch (const std::exception& e) {
            // Игнорирование файлов, если не прочитали
        }
        return false;
    });
}

std::unordered_map<std::string, std::vector<std::string>> FileSearcher::findDuplicates() {
    auto start_time = std::chrono::steady_clock::now();
    
    GraphicsUtils::printHeader("DUPLICATE SEARCH");
    
    if (show_progress_) {
        GraphicsUtils::printSection("Phase 1: Collecting files");
    }
    
    auto files = collectAllFiles();
    
    if (show_progress_) {
        GraphicsUtils::printSection("Phase 2: Grouping by size");
    }
    
    std::unordered_map<std::string, std::vector<fs::path>> sizeGroups;
    ProgressVisualizer size_progress("Size grouping", files.size());
    
    for (const auto& file : files) {
        try {
            auto sizeHash = HashCalculator::calculateFileSizeHash(file);
            sizeGroups[sizeHash].push_back(file);
            
            if (show_progress_) {
                size_progress.increment();
            }
        } catch (const std::exception& e) {
            // Пропуск файлов с ошибками
        }
    }
    
    if (show_progress_) {
        size_progress.complete();
        GraphicsUtils::printSection("Phase 3: Calculating MD5 hashes");
    }
    
    std::unordered_map<std::string, std::vector<std::string>> duplicates;
    
    size_t total_candidates = 0;
    for (const auto& [sizeHash, fileGroup] : sizeGroups) {
        if (fileGroup.size() > 1) {
            total_candidates += fileGroup.size();
        }
    }
    
    ProgressVisualizer md5_progress("MD5 calculation", total_candidates);
    
    for (const auto& [sizeHash, fileGroup] : sizeGroups) {
        if (fileGroup.size() > 1) {
            std::unordered_map<std::string, std::vector<std::string>> md5Groups;
            
            for (const auto& file : fileGroup) {
                try {
                    auto md5 = HashCalculator::calculateMD5(file);
                    md5Groups[md5].push_back(file.string());
                    
                    if (show_progress_) {
                        md5_progress.increment();
                    }
                } catch (const std::exception& e) {
                    if (show_progress_) {
                        md5_progress.increment();
                    }
                }
            }
            
            for (const auto& [md5, filePaths] : md5Groups) {
                if (filePaths.size() > 1) {
                    duplicates[md5] = filePaths;
                }
            }
        }
    }
    
    if (show_progress_) {
        md5_progress.complete();
        
        auto end_time = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(end_time - start_time).count() / 1000.0;
        
        GraphicsUtils::printStats(files.size(), duplicates.size(), elapsed);
    }
    
    return duplicates;
}
