//
//  GraphicsUtils.h
//  SeekFS
//
//  Created by Максим Гоглов on 29.10.2025.
//
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <iomanip>

using namespace std;

class GraphicsUtils {
public:
    static void printHeader(const string& title) {
        cout << endl;
        cout << "+" << string(title.length() + 4, '-') << "+\n";
        cout << "|  " << title << "  |\n";
        cout << "+" << string(title.length() + 4, '-') << "+\n";
    }
    
    static void printSection(const string& section) {
        cout << "\n-> " << section << endl;
    }
    
    static void printFileTree(const vector<string>& files, const string& root = "Found Files") {
        cout << endl;
        cout << root << "\n";
        cout << "`--\n";
        
        for (size_t i = 0; i < files.size(); ++i) {
            cout << "   ";
            if (i == files.size() - 1) {
                cout << "`-- " << files[i] << endl;
            } else {
                cout << "|-- " << files[i] << endl;
            }
        }
    }
    
    static void printDuplicateGroups(const unordered_map<string, vector<string>>& duplicates) {
        if (duplicates.empty()) {
            cout << endl;
            cout << "+---------------------+\n";
            cout << "|  No duplicates found |\n";
            cout << "+---------------------+\n";
            return;
        }
        
        int group_num = 1;
        for (const auto& [hash, files] : duplicates) {
            cout << endl;
            cout << "Duplicate Group #" << group_num << " (Hash: " << hash.substr(0, 12) << "...)\n";
            cout << "`--\n";
            
            for (size_t i = 0; i < files.size(); ++i) {
                cout << "   ";
                if (i == files.size() - 1) {
                    cout << "`-- " << files[i] << endl;
                } else {
                    cout << "|-- " << files[i] << endl;
                }
            }
            group_num++;
        }
    }
    
    static void printStats(size_t files_scanned, size_t duplicates_found, double elapsed_seconds) {
        cout << endl;
        cout << "+----------- Statistics -----------+\n";
        cout << "|                                  |\n";
        cout << "|  Files scanned: " << std::setw(15) << files_scanned << "     |\n";
        cout << "|  Duplicates found: " << std::setw(12) << duplicates_found << "     |\n";
        cout << "|  Time elapsed: " << std::setw(12) << std::fixed << std::setprecision(2) << elapsed_seconds << "s   |\n";
        cout << "|                                  |\n";
        cout << "+----------------------------------+\n";
    }
    
    static void printSearchBanner(const std::string& type, const std::string& pattern) {
        cout << endl;
        cout << "========================================\n";
        cout << "  FILE SEARCH: " << type << "\n";
        cout << "  Pattern: " << pattern << "\n";
        cout << "========================================\n";
    }
};
