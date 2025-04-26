cmake_minimum_required(VERSION 3.20)
project(easy-states VERSION 1.0 LANGUAGES CXX)

# Use C++17 (or later)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Add examples
option(BUILD_EXAMPLES "Build examples" ON)
option(WITH_FMT "Include {fmt} library" ON)

# Header-only library
add_library(easy_states INTERFACE)
target_include_directories(easy_states INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
target_compile_features(easy_states INTERFACE cxx_std_17)

if(WITH_FMT)
    target_compile_definitions(easy_states INTERFACE WITH_FMT)
    find_package(fmt REQUIRED)
endif()


# Install rules (optional)
install(DIRECTORY include/ DESTINATION include)
install(TARGETS easy_states EXPORT easy_statesTargets)
install(EXPORT easy_statesTargets FILE easy_statesTargets.cmake NAMESPACE easy_states:: DESTINATION lib/cmake/easy_states)

if(BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()