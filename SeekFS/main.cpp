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
using namespace std;

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
            cout << options.help() << endl;
            cout << "\nExamples:" << endl;
            cout << "  Search for .txt files: " << argv[0] << " -n \".*\\.txt$\"" << endl;
            cout << "  Case insensitive content search: " << argv[0] << " -c \"TODO\" -i" << endl;
            cout << "  Find duplicates with 8 threads: " << argv[0] << " -d -t 8" << endl;
            return 0;
        }

        string search_path = result["path"].as<string>();
        int num_threads = result["threads"].as<int>();
        size_t max_size_mb = result["max-size"].as<size_t>();
        
        FileSearcher searcher(search_path, num_threads);
        searcher.setCaseSensitive(!result.count("ignore-case"));
        searcher.setMaxFileSize(max_size_mb * 1024 * 1024);
        
        if (result.count("type")) {
            string types_str = result["type"].as<string>();
            vector<std::string> types;
            size_t start = 0, end = 0;
            while ((end = types_str.find(',', start)) != string::npos) {
                types.push_back(types_str.substr(start, end - start));
                start = end + 1;
            }
            types.push_back(types_str.substr(start));
            searcher.setFileTypes(types);
        }

        bool found_any = false;

        if (result.count("name")) {
            found_any = true;
            cout << "Searching files by name pattern: " << result["name"].as<string>() << endl;
            auto files = searcher.searchByName(result["name"].as<string>());
            cout << "Found " << files.size() << " files:" << endl;
            for (const auto& file : files) {
                cout << "  " << file << endl;
            }
            cout << endl;
        }
        
        if (result.count("content")) {
            found_any = true;
            cout << "Searching files by content pattern: " << result["content"].as<string>() << endl;
            auto files = searcher.searchByContent(result["content"].as<string>());
            cout << "Found " << files.size() << " files:" << endl;
            for (const auto& file : files) {
                cout << "  " << file << endl;
            }
            cout << endl;
        }
        
        if (result.count("duplicates")) {
            found_any = true;
            cout << "Searching for duplicate files..." << endl;
            auto duplicates = searcher.findDuplicates();
            
            if (duplicates.empty()) {
                cout << "No duplicate files found." << endl;
            } else {
                cout << "Found " << duplicates.size() << " groups of duplicates:" << endl;
                for (const auto& [hash, files] : duplicates) {
                    cout << "Hash: " << hash.substr(0, 16) << "..." << endl;
                    for (const auto& file : files) {
                        cout << "  " << file << endl;
                    }
                    cout << endl;
                }
            }
        }

        if (!found_any) {
            cout << "No search criteria specified. Use -h for help." << endl;
        }

    } catch (const cxxopts::exceptions::exception& e) {
        cerr << "Error parsing options: " << e.what() << endl;
        return 1;
    } catch (const std::exception& e) {
        cerr << "Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
