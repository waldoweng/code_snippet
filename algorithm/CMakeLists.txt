cmake_minimum_required (VERSION 3.1)
project (Algorithm)
add_executable (suffix_tree.main suffix_tree.main.cpp suffix_tree.cpp)
target_compile_features (suffix_tree.main PRIVATE cxx_range_for)

add_library(suffix_tree STATIC suffix_tree.cpp)
target_compile_features(suffix_tree PRIVATE cxx_range_for)
