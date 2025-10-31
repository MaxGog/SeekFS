//
//  Spinner.h
//  SeekFS
//
//  Created by Максим Гоглов on 29.10.2025.
//
// SpinnerRich.h - альтернативная версия с проверкой поддержки Unicode
#pragma once
#include <iostream>
#include <chrono>
#include <thread>
#include <string>

class SpinnerRich {
public:
    SpinnerRich(const std::string& message = "Processing") : message_(message), running_(false) {}
    
    void start() {
        running_ = true;
        spinner_thread_ = std::thread(&SpinnerRich::run, this);
    }
    
    void stop() {
        running_ = false;
        if (spinner_thread_.joinable()) {
            spinner_thread_.join();
        }
        std::cout << "\r" << std::string(message_.length() + 10, ' ') << "\r";
    }
    
private:
    void run() {
        const char* spinner_ascii[] = {"|", "/", "-", "\\"};
        const char* spinner_unicode[] = {"◐", "◓", "◑", "◒"};
        
        const char** spinner_chars = spinner_ascii;
        size_t spinner_size = 4;
        
        try {
            std::cout << "◐";
            std::cout << "\b";
            spinner_chars = spinner_unicode;
        } catch (...) {
            spinner_chars = spinner_ascii;
        }
        
        size_t index = 0;
        
        while (running_) {
            std::cout << "\r" << spinner_chars[index] << " " << message_ << "...";
            std::cout.flush();
            
            index = (index + 1) % spinner_size;
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }
    
    std::string message_;
    bool running_;
    std::thread spinner_thread_;
};
