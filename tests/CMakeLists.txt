
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

    setup_target_for_coverage_gcovr_coveralls(
      NAME coveralls
      EXECUTABLE iggy_cpp_test
      EXCLUDE "build/vcpkg_installed/*" "tests/*"
      DEPENDENCIES iggy)

    setup_target_for_coverage_gcovr_html(
      NAME coverage-html
      EXECUTABLE iggy_cpp_test
      EXCLUDE "build/vcpkg_installed/*" "tests/*"
      DEPENDENCIES iggy)
  endif()
endif()
