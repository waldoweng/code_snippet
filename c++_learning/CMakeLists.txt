cmake_minimum_required (VERSION 3.1)
project (C++_LEARNING)
find_package (Threads)
add_executable (promise_future promise_future.cpp)
target_link_libraries (promise_future ${CMAKE_THREAD_LIBS_INIT})
target_compile_features (promise_future PRIVATE cxx_range_for)

