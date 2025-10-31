#!/bin/bash
# Демонстрационные примеры использования SeekFS

echo "🚀 Running SeekFS examples..."

echo "1. Searching for C++ files:"
./SeekFS -n ".*\\.(cpp|h|hpp)$" --progress

echo "2. Finding TODO comments:"
./SeekFS -c "TODO|FIXME" --type cpp,h --progress

echo "3. Finding duplicate images:"
./SeekFS -d --type jpg,png --max-size 50 --progress
