macro(fuzzer name)
  add_executable(${name} ${name}.cc)
  target_compile_options(${name} PRIVATE "-Wno-missing-prototypes")
  if(LIBFUZZER_FROM_DEPS)
    set_target_properties(${name} PROPERTIES LINK_FLAGS "-fsanitize=fuzzer-no-link")
    target_link_libraries(${name} Fuzzer)
  else()
    set_target_properties(${name} PROPERTIES LINK_FLAGS "-fsanitize=fuzzer")
  endif()
  set_target_properties(
    ${name}
    PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES)
endmacro()

fuzzer(pub_client)
