cmake_minimum_required(VERSION 3.14)
project(iggy-cpp-client)

option(ENABLE_GTEST "Enable GoogleTest" OFF)
option(ENABLE_DOXYGEN "Enable Doxygen" OFF)

# avoid warning about DOWNLOAD_EXTRACT_TIMESTAMP in CMake 3.24
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
  cmake_policy(SET CMP0135 NEW)
endif()

# set up library dependencies
find_package(argh CONFIG REQUIRED)
find_package(benchmark CONFIG REQUIRED)
find_package(libuv CONFIG REQUIRED)
find_package(spdlog CONFIG REQUIRED)

add_library(
  iggy

  sdk/models.cc
  sdk/utils.cc
)
target_compile_features(iggy PRIVATE cxx_std_17)

add_executable(
  iggy_cmd

  sdk/iggy_cmd.cc
)
target_include_directories(iggy_cmd PRIVATE ${argh_INCLUDE_DIR})
target_link_libraries(
  iggy_cmd

  iggy
)

# set up GoogleTest
if(ENABLE_GTEST)
  include(FetchContent)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/dddb219c3eb96d7f9200f09b0a381f016e6b4562.zip
  )
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)

  # set up GoogleTest
  enable_testing()

  add_executable(
    iggy_cpp_test

    tests/models_test.cc
  )
  target_link_libraries(
    iggy_cpp_test

    iggy
    GTest::gtest_main
  )

  include(GoogleTest)
  gtest_discover_tests(iggy_cpp_test)
endif()

# set up Doxygen for API doc generation
if(ENABLE_DOXYGEN)
  find_package(Doxygen)

  if (DOXYGEN_FOUND)
      # Note the option ALL which allows to build the docs together with the application
      set(DOXYGEN_CFG ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile)
      add_custom_target( doc_doxygen ALL
          COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_CFG}
          WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
          COMMENT "Generating API documentation with Doxygen"
          VERBATIM )
  else (DOXYGEN_FOUND)
    message("Doxygen need to be installed to generate the doxygen documentation")
  endif (DOXYGEN_FOUND)
endif()
