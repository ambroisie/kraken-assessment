find_program(BASH_PROGRAM bash)

if (BASH_PROGRAM)

add_test(test_output
  ${BASH_PROGRAM}
  ${CMAKE_CURRENT_SOURCE_DIR}/test-output.sh
  ${CMAKE_SOURCE_DIR}/data/
  # FIXME: can't get it working $<TARGET_FILE:kraken>
  ${CMAKE_BINARY_DIR}/src/kraken
)

endif (BASH_PROGRAM)
