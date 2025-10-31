cmake_minimum_required(VERSION 3.15)
project(SeekFS VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(GNUInstallDirs)

include(FetchContent)
FetchContent_Declare(
  cxxopts
  GIT_REPOSITORY https://github.com/jarro2783/cxxopts.git
  GIT_TAG v3.1.0
)
FetchContent_MakeAvailable(cxxopts)

set(SOURCES
    src/main.cpp
    src/FileSearcher.cpp
    src/HashCalculator.cpp
)

add_executable(SeekFS ${SOURCES})
target_link_libraries(SeekFS PRIVATE cxxopts::cxxopts)
target_include_directories(SeekFS PRIVATE include)

install(TARGETS SeekFS
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    BUNDLE DESTINATION ${CMAKE_INSTALL_BINDIR}
)

install(FILES README.md LICENSE
    DESTINATION ${CMAKE_INSTALL_DOCDIR}
)

include(CPack)

set(CPACK_PACKAGE_NAME "SeekFS")
set(CPACK_PACKAGE_VENDOR "Maxim Goglov")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Advanced file search utility")
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_RESOURCE_FILE_README "${CMAKE_SOURCE_DIR}/README.md")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_SOURCE_DIR}/LICENSE")

if(UNIX AND NOT APPLE)
    set(CPACK_GENERATOR "DEB")
    set(CPACK_DEBIAN_PACKAGE_MAINTAINER "Maxim Goglov")
    set(CPACK_DEBIAN_PACKAGE_DEPENDS "libc6 (>= 2.14), libstdc++6 (>= 4.9)")
    set(CPACK_DEBIAN_PACKAGE_SECTION "utils")
    
elseif(APPLE)
    set(CPACK_GENERATOR "DragNDrop;TGZ")
    
elseif(WIN32)
    set(CPACK_GENERATOR "NSIS;ZIP")
    set(CPACK_NSIS_DISPLAY_NAME "SeekFS File Search Utility")
    set(CPACK_NSIS_PACKAGE_NAME "SeekFS")
endif()

include(CPack)