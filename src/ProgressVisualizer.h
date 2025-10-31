//
//  ProgressVisualizer.h
//  SeekFS
//
//  Created by Максим Гоглов on 29.10.2025.
//
#pragma once
#include <iostream>
#include <iomanip>
#include <string>
#include <chrono>
#include <atomic>

class ProgressVisualizer {
public:
    ProgressVisualizer(const std::string& task_name, size_t total = 0)
        : task_name_(task_name), total_(total), current_(0), start_time_(std::chrono::steady_clock::now()) {}
    
    void update(size_t progress) {
        current_ = progress;
        display();
    }
    
    void increment() {
        ++current_;
        display();
    }
    
    void set_total(size_t total) {
        total_ = total;
    }
    
    void complete() {
        current_ = total_;
        display();
        std::cout << std::endl;
    }
    
private:
    void display() {
        if (total_ == 0) return;
        
        float percentage = static_cast<float>(current_) / total_;
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - start_time_).count() / 1000.0;
        
        std::cout << "\r" << task_name_ << " [";
        
        int bar_width = 30;
        int pos = bar_width * percentage;
        
        for (int i = 0; i < bar_width; ++i) {
            if (i < pos) std::cout << "█";
            else if (i == pos) std::cout << "▌";
            else std::cout << " ";
        }
        
        std::cout << "] " << std::setw(3) << static_cast<int>(percentage * 100) << "%";
        std::cout << " " << current_ << "/" << total_;
        
        if (percentage > 0.01) {
            double eta = elapsed / percentage * (1 - percentage);
            std::cout << " ETA: " << std::fixed << std::setprecision(1) << eta << "s";
        }
        
        std::cout << std::flush;
    }
    
    std::string task_name_;
    size_t total_;
    size_t current_;
    std::chrono::steady_clock::time_point start_time_;
};
