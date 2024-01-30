
# set up GoogleTest
if(BUILD_TESTS)
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

    models_test.cc
  )
  target_link_libraries(
    iggy_cpp_test

    iggy
    GTest::gtest_main
  )

  include(GoogleTest)
  gtest_discover_tests(iggy_cpp_test)
endif()
