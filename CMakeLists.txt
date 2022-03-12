cmake_minimum_required(VERSION 3.13)
project(kraken VERSION 0.0.0 LANGUAGES CXX)
enable_testing()

# Add a dummy target to share all our compile options
add_library(common_options INTERFACE)
target_compile_features(common_options INTERFACE
  cxx_std_20
)
target_compile_options(common_options INTERFACE
  -Wall
  -Wextra
)
target_include_directories(common_options INTERFACE
  src
  ${CMAKE_BINARY_DIR}/src
)

add_subdirectory(src)
add_subdirectory(tests)
