
# set up GoogleTest
if(BUILD_TESTS)
  include(FetchContent)
  FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/dddb219c3eb96d7f9200f09b0a381f016e6b4562.zip
  )
  set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
  FetchContent_MakeAvailable(googletest)

  # for E2E tests, controlling Docker containers
  find_package(reproc++ CONFIG REQUIRED)

  # set up GoogleTest
  enable_testing()

  add_executable(
    iggy_cpp_test

    model_test.cc
  )
  target_link_libraries(
    iggy_cpp_test

    iggy
    GTest::gtest_main
    libuv::uv_a
    reproc++
    unofficial-sodium::sodium
    ${USOCKETS_LIB}
    OpenSSL::SSL
    OpenSSL::Crypto
  )

  add_executable(
    iggy_e2e_test

    e2e_testutils.cc
    ping_cmd_test.cc
  )
  target_link_libraries(
    iggy_e2e_test

    PRIVATE

    iggy
    GTest::gtest_main
    libuv::uv_a
    reproc++
    unofficial-sodium::sodium
    ${USOCKETS_LIB}
    OpenSSL::SSL
    OpenSSL::Crypto
  )

  include(GoogleTest)
  gtest_discover_tests(iggy_cpp_test)
endif()
