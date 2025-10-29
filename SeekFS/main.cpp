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
#include "GraphicsUtils.h"

namespace fs = std::filesystem;
using namespace std;

int main(int argc, char** argv) {
    cxxopts::Options options("FileSearchUtility", "🎯 Advanced file search utility - Modern C++17/20");
    
    options.add_options()
        ("p,path", "Search path", cxxopts::value<std::string>()->default_value("."))
        ("n,name", "File name pattern (regex)", cxxopts::value<std::string>())
        ("c,content", "Content pattern (regex)", cxxopts::value<std::string>())
        ("d,duplicates", "Find duplicate files by hash")
        ("i,ignore-case", "Case insensitive search")
        ("progress", "Show progress visualization")
        ("t,threads", "Number of threads", cxxopts::value<int>()->default_value("4"))
        ("max-size", "Max file size in MB", cxxopts::value<size_t>()->default_value("100"))
        ("type", "File extensions (comma separated)", cxxopts::value<std::string>())
        ("h,help", "Print usage")
    ;

    try {
        auto result = options.parse(argc, argv);

        if (result.count("help")) {
            GraphicsUtils::printHeader("FILE SEARCH UTILITY");
            cout << options.help() << endl;
            cout << "\n📚 Examples:\n";
            cout << "  " << argv[0] << " -n \".*\\.txt$\"\n";
            cout << "  " << argv[0] << " -c \"TODO\" -i --progress\n";
            cout << "  " << argv[0] << " -d -t 8 --progress\n";
            return 0;
        }

        string search_path = result["path"].as<string>();
        int num_threads = result["threads"].as<int>();
        size_t max_size_mb = result["max-size"].as<size_t>();
        bool show_progress = result.count("progress");
        
        FileSearcher searcher(search_path, num_threads, show_progress);
        searcher.setCaseSensitive(!result.count("ignore-case"));
        searcher.setMaxFileSize(max_size_mb * 1024 * 1024);
        
        if (result.count("type")) {
            string types_str = result["type"].as<string>();
            vector<string> types;
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
            GraphicsUtils::printHeader("NAME SEARCH");
            std::cout << "Pattern: " << result["name"].as<string>() << endl;
            
            auto files = searcher.searchByName(result["name"].as<string>());
            GraphicsUtils::printFileTree(files, "📁 Matching Files");
        }
        
        if (result.count("content")) {
            found_any = true;
            GraphicsUtils::printHeader("CONTENT SEARCH");
            cout << "Pattern: " << result["content"].as<string>() << endl;
            
            auto files = searcher.searchByContent(result["content"].as<string>());
            GraphicsUtils::printFileTree(files, "📄 Files with Matching Content");
        }
        
        if (result.count("duplicates")) {
            found_any = true;
            auto duplicates = searcher.findDuplicates();
            GraphicsUtils::printDuplicateGroups(duplicates);
        }

        if (!found_any) {
            GraphicsUtils::printHeader("INFO");
            cout << "❓ No search criteria specified. Use -h for help.\n";
        }

    } catch (const cxxopts::exceptions::exception& e) {
        cerr << "❌ Error parsing options: " << e.what() << endl;
        return 1;
    } catch (const exception& e) {
        cerr << "💥 Error: " << e.what() << endl;
        return 1;
    }

    return 0;
}
