# List of examples
find_package(fmt REQUIRED)
find_package(magic_enum REQUIRED)
add_executable(example main.cpp)
target_link_libraries(example PRIVATE easy_states PRIVATE fmt::fmt-header-only)
