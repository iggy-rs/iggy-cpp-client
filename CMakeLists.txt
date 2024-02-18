cmake_minimum_required(VERSION 3.14)
project(iggy-cpp-client)

option(BUILD_TESTS "Build and run unit tests" OFF)
option(BUILD_DOCS "Build documentation" OFF)

# avoid warning about DOWNLOAD_EXTRACT_TIMESTAMP in CMake 3.24
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
  cmake_policy(SET CMP0135 NEW)
endif()

# set up library dependencies
find_package(argh CONFIG REQUIRED)
find_package(benchmark CONFIG REQUIRED)
find_package(cpptoml CONFIG REQUIRED)
find_package(libuv CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)
find_package(unofficial-sodium CONFIG REQUIRED)

add_library(
  iggy

  sdk/client.cc
  sdk/model.cc
  sdk/serialization.cc
)
target_compile_features(iggy PRIVATE cxx_std_17)
target_include_directories(iggy PRIVATE ${unofficial-sodium_DIR}/../../include)

add_executable(
  iggy_cmd

  examples/cli/cli.cc
  examples/cli/iggy_cmd.cc
)
target_include_directories(iggy_cmd PRIVATE ${argh_INCLUDE_DIR})
target_link_libraries(
  iggy_cmd

  iggy
  unofficial-sodium::sodium
)

if (BUILD_TESTS)
  add_subdirectory(tests)
endif()

if (BUILD_DOCS)
  add_subdirectory(docs)
endif()
