//
//  performance_benchmark.cpp
//  SeekFS
//
//  Created by Максим Гоглов on 31.10.2025.
//

#include <benchmark/benchmark.h>
#include "FileSearcher.h"

static void BM_SearchByName(benchmark::State& state) {
    FileSearcher searcher("/large_directory");
    for (auto _ : state) {
        auto results = searcher.searchByName(".*\\.cpp$");
        benchmark::DoNotOptimize(results);
    }
}
BENCHMARK(BM_SearchByName)->Unit(benchmark::kMillisecond);

static void BM_MD5Calculation(benchmark::State& state) {
    for (auto _ : state) {
        auto hash = HashCalculator::calculateMD5("large_file.bin");
        benchmark::DoNotOptimize(hash);
    }
}
BENCHMARK(BM_MD5Calculation)->Unit(benchmark::kMillisecond);
