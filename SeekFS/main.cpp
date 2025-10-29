//
//  main.cpp
//  SeekFS
//
//  Created by Максим Гоглов on 29.10.2025.
//

#include <iostream>
#include <vector>
#include <string>
#include <filesystem>
#include <thread>
#include "cxxopts.hpp"
#include "FileSearcher.h"

namespace fs = std::filesystem;

int main(int argc, char** argv) {
    cxxopts::Options options("FileSearch", "Advanced file search utility - Modern C++17/20");
    
    options.add_options()
        ("p,path", "Search path", cxxopts::value<std::string>()->default_value("."))
        ("n,name", "File name pattern (regex)", cxxopts::value<std::string>())
        ("c,content", "Content pattern (regex)", cxxopts::value<std::string>())
        ("d,duplicates", "Find duplicate files by hash")
        ("i,ignore-case", "Case insensitive search")
        ("t,threads", "Number of threads", cxxopts::value<int>()->default_value("4"))
        ("max-size", "Max file size in MB", cxxopts::value<size_t>()->default_value("100"))
        ("type", "File extensions (comma separated)", cxxopts::value<std::string>())
        ("h,help", "Print usage")
    ;

    try {
        auto result = options.parse(argc, argv);

        if (result.count("help")) {
            std::cout << options.help() << std::endl;
            std::cout << "\nExamples:" << std::endl;
            std::cout << "  Search for .txt files: " << argv[0] << " -n \".*\\.txt$\"" << std::endl;
            std::cout << "  Case insensitive content search: " << argv[0] << " -c \"TODO\" -i" << std::endl;
            std::cout << "  Find duplicates with 8 threads: " << argv[0] << " -d -t 8" << std::endl;
            return 0;
        }

        std::string search_path = result["path"].as<std::string>();
        int num_threads = result["threads"].as<int>();
        size_t max_size_mb = result["max-size"].as<size_t>();
        
        FileSearcher searcher(search_path, num_threads);
        searcher.setCaseSensitive(!result.count("ignore-case"));
        searcher.setMaxFileSize(max_size_mb * 1024 * 1024);
        
        if (result.count("type")) {
            std::string types_str = result["type"].as<std::string>();
            std::vector<std::string> types;
            size_t start = 0, end = 0;
            while ((end = types_str.find(',', start)) != std::string::npos) {
                types.push_back(types_str.substr(start, end - start));
                start = end + 1;
            }
            types.push_back(types_str.substr(start));
            searcher.setFileTypes(types);
        }

        bool found_any = false;

        if (result.count("name")) {
            found_any = true;
            std::cout << "Searching files by name pattern: " << result["name"].as<std::string>() << std::endl;
            auto files = searcher.searchByName(result["name"].as<std::string>());
            std::cout << "Found " << files.size() << " files:" << std::endl;
            for (const auto& file : files) {
                std::cout << "  " << file << std::endl;
            }
            std::cout << std::endl;
        }
        
        if (result.count("content")) {
            found_any = true;
            std::cout << "Searching files by content pattern: " << result["content"].as<std::string>() << std::endl;
            auto files = searcher.searchByContent(result["content"].as<std::string>());
            std::cout << "Found " << files.size() << " files:" << std::endl;
            for (const auto& file : files) {
                std::cout << "  " << file << std::endl;
            }
            std::cout << std::endl;
        }
        
        if (result.count("duplicates")) {
            found_any = true;
            std::cout << "Searching for duplicate files..." << std::endl;
            auto duplicates = searcher.findDuplicates();
            
            if (duplicates.empty()) {
                std::cout << "No duplicate files found." << std::endl;
            } else {
                std::cout << "Found " << duplicates.size() << " groups of duplicates:" << std::endl;
                for (const auto& [hash, files] : duplicates) {
                    std::cout << "Hash: " << hash.substr(0, 16) << "..." << std::endl;
                    for (const auto& file : files) {
                        std::cout << "  " << file << std::endl;
                    }
                    std::cout << std::endl;
                }
            }
        }

        if (!found_any) {
            std::cout << "No search criteria specified. Use -h for help." << std::endl;
        }

    } catch (const cxxopts::exceptions::exception& e) {
        std::cerr << "Error parsing options: " << e.what() << std::endl;
        return 1;
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
