
if(BUILD_TESTS)
  find_package(Catch2 CONFIG REQUIRED)
  find_package(reproc++ CONFIG REQUIRED)

  add_executable(
    iggy_cpp_test

    model_test.cc
    iggy_protocol_provider_test.cc
  )
  target_link_libraries(
    iggy_cpp_test

    iggy
    Catch2::Catch2
    Catch2::Catch2WithMain
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
    reproc++
  )

  if(ENABLE_CODE_COVERAGE)
    include(../cmake/modules/CodeCoverage.cmake)
    append_coverage_compiler_flags()

    setup_target_for_coverage_lcov(
      NAME coverage
      EXECUTABLE iggy_cpp_test
      DEPENDENCIES iggy
      EXCLUDE /usr/include/c++/*  ${CMAKE_CURRENT_SOURCE_DIR}/../tests/*
      ${CMAKE_CURRENT_SOURCE_DIR}/../build/*)
  endif()
endif()
