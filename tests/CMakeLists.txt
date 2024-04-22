
if(BUILD_TESTS)
  find_package(Catch2 CONFIG REQUIRED)
  find_package(inja CONFIG REQUIRED)
  find_package(reproc++ CONFIG REQUIRED)

  add_executable(
    iggy_cpp_test

    client_test.cc
    crypto_test.cc
    iggy_protocol_provider_test.cc
    model_test.cc
    serialization_test.cc
    ssl_test.cc
    unit_testutils.cc
  )
  target_compile_features(iggy_cpp_test PRIVATE cxx_std_17)
  target_include_directories(iggy_cpp_test PRIVATE
    ${SODIUM_INCLUDE_DIR}
    ${ADA_INCLUDE_DIR}
    ${WOLFSSL_INCLUDE_DIR}
    ${NGHTTP3_INCLUDE_DIR}
    ${NGTCP2_INCLUDE_DIR}
    ${CURL_INCLUDE_DIR}
  )
  target_link_libraries(
    iggy_cpp_test

    iggy
    Catch2::Catch2
    Catch2::Catch2WithMain
    pantor::inja
    reproc++
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
      NAME coverage
      EXECUTABLE iggy_cpp_test
      EXCLUDE "build/vcpkg_installed/*" "tests/*"
      DEPENDENCIES iggy)
  endif()
endif()
