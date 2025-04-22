cmake_minimum_required(VERSION 3.20)
project(easy-states VERSION 1.0 LANGUAGES CXX)

# Use C++17 (or later)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(fmt REQUIRED)
find_package(magic_enum REQUIRED)

# Optional: set runtime output dir (for cleaner builds)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/
)

add_executable(app
    main.cpp
)
target_link_libraries(app PRIVATE fmt::fmt-header-only)

