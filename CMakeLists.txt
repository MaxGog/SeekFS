cmake_minimum_required(VERSION 3.15)
project(SeekFS VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)

FetchContent_Declare(
  cxxopts
  GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
  GIT_TAG v3.1.0
)

FetchContent_MakeAvailable(cxxopts)

add_executable(SeekFS
    main.cpp
    FileSearcher.cpp
    HashCalculator.cpp
    GraphicsUtils.cpp
    ProgressVisualizer.cpp
)

target_link_libraries(SeekFS PRIVATE cxxopts::cxxopts)

install(TARGETS SeekFS DESTINATION bin)