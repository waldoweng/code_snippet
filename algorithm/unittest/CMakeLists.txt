cmake_minimum_required (VERSION 3.1)
project (Algorithm)
include_directories(../../external/googletest/googletest/include/ ../)
link_directories(../../external/googletest/googletest/ ../)
add_executable (unittest unittest.cpp)
target_link_libraries(unittest gtest suffix_tree)
target_compile_features (unittest PRIVATE cxx_range_for)

