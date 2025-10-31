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

using namespace std;
namespace fs = filesystem;

int main(int argc, char** argv) {
    cxxopts::Options options("SeekFS", "🎯 Advanced file search utility - Modern C++17/20");
    
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
        
        if (!fs::exists(search_path)) {
            cerr << "❌ Error: Path '" << search_path << "' does not exist\n";
            return 1;
        }
        
        if (!fs::is_directory(search_path)) {
            cerr << "❌ Error: '" << search_path << "' It is not a directory\n";
            return 1;
        }
        
        if (num_threads < 1) {
            cerr << "❌ Error: The number of threads must be a positive number\n";
            return 1;
        }
        
        if (max_size_mb == 0) {
            cerr << "❌ Error: The maximum file size must be greater than 0\n";
            return 1;
        }

        bool show_progress = result.count("progress");
        
        FileSearcher searcher(search_path, num_threads, show_progress);
        searcher.setCaseSensitive(!result.count("ignore-case"));
        searcher.setMaxFileSize(max_size_mb * 1024 * 1024);
        
        if (result.count("type")) {
            try {
                string types_str = result["type"].as<string>();
                vector<string> types;
                size_t start = 0, end = 0;
                while ((end = types_str.find(',', start)) != string::npos) {
                    types.push_back(types_str.substr(start, end - start));
                    start = end + 1;
                }
                types.push_back(types_str.substr(start));
                searcher.setFileTypes(types);
            } catch (const exception& e) {
                cerr << "❌ File type processing error: " << e.what() << endl;
                return 1;
            }
        }

        bool found_any = false;
        bool search_successful = true;

        if (result.count("name")) {
            found_any = true;
            GraphicsUtils::printHeader("NAME SEARCH");
            cout << "Pattern: " << result["name"].as<string>() << endl;
            
            try {
                auto files = searcher.searchByName(result["name"].as<string>());
                if (files.empty()) {
                    cout << "🔍 Files according to the specified template were not found\n";
                } else {
                    GraphicsUtils::printFileTree(files, "📁 Matching Files");
                }
            } catch (const exception& e) {
                cerr << "❌ Error when searching by name: " << e.what() << endl;
                search_successful = false;
            }
        }
        
        if (result.count("content")) {
            found_any = true;
            GraphicsUtils::printHeader("CONTENT SEARCH");
            cout << "Pattern: " << result["content"].as<string>() << endl;
            
            try {
                auto files = searcher.searchByContent(result["content"].as<string>());
                if (files.empty()) {
                    cout << "🔍 Files with the specified content were not found\n";
                } else {
                    GraphicsUtils::printFileTree(files, "📄 Files with Matching Content");
                }
            } catch (const exception& e) {
                cerr << "❌ Error when searching by content: " << e.what() << endl;
                search_successful = false;
            }
        }
        
        if (result.count("duplicates")) {
            found_any = true;
            try {
                auto duplicates = searcher.findDuplicates();
                if (duplicates.empty()) {
                    cout << "✅ No duplicate files found\n";
                } else {
                    GraphicsUtils::printDuplicateGroups(duplicates);
                }
            } catch (const exception& e) {
                cerr << "❌ Error when searching for duplicates: " << e.what() << endl;
                search_successful = false;
            }
        }

        if (!found_any) {
            GraphicsUtils::printHeader("INFO");
            cout << "❓ No search criteria specified. Use -h for help.\n";
        }

        return search_successful ? 0 : 2;

    } catch (const cxxopts::exceptions::exception& e) {
        cerr << "❌ Argument parsing error: " << e.what() << endl;
        cerr << "💡 Use -h to view the help\n";
        return 1;
    } catch (const filesystem::filesystem_error& e) {
        cerr << "❌ File system error: " << e.what() << endl;
        cerr << "💡 Check the path and access rights\n";
        return 1;
    } catch (const exception& e) {
        cerr << "💥 Unexpected error: " << e.what() << endl;
        return 1;
    } catch (...) {
        cerr << "💥 Unknown error\n";
        return 1;
    }
}
