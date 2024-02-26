
if(BUILD_TESTS)
  find_package(catch2 CONFIG REQUIRED)
  find_package(reproc++ CONFIG REQUIRED)

  add_executable(
    iggy_cpp_test

    model_test.cc
  )
  target_link_libraries(
    iggy_cpp_test

    iggy
    Catch2::Catch2
    Catch2::Catch2WithMain
    libuv::uv_a
    reproc++
    unofficial-sodium::sodium
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
    Catch2::Catch2
    Catch2::Catch2WithMain
    libuv::uv_a
    reproc++
    unofficial-sodium::sodium
  )
endif()
