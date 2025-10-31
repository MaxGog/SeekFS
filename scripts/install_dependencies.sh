#!/bin/bash
# Скрипт установки зависимостей для SeekFS

echo "Installing SeekFS dependencies..."

# Ubuntu/Debian
if [ -f /etc/debian_version ]; then
    sudo apt update
    sudo apt install -y build-essential cmake git
    
# macOS
elif [ "$(uname)" == "Darwin" ]; then
    brew update
    brew install cmake git
    
# Windows (через Chocolatey)
elif [ -f /c/ProgramData/chocolatey/choco.exe ]; then
    choco install cmake git.install -y
fi

echo "Dependencies installed successfully!"
