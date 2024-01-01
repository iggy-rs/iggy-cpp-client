cmake_minimum_required(VERSION 3.14)
project(iggy-cpp-client)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# avoid warning about DOWNLOAD_EXTRACT_TIMESTAMP in CMake 3.24
if (CMAKE_VERSION VERSION_GREATER_EQUAL "3.24.0")
  cmake_policy(SET CMP0135 NEW)
endif()

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/dddb219c3eb96d7f9200f09b0a381f016e6b4562.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_library(
  iggy

  sdk/models.cc
)

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

# set up Doxygen for API doc generation
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