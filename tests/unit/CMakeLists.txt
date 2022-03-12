find_package(GTest)

if (${GTest_FOUND})
include(GoogleTest)

add_executable(csv_test csv.cc)
target_link_libraries(csv_test PRIVATE common_options)

target_link_libraries(csv_test PRIVATE
  csv
  GTest::gtest
  GTest::gtest_main
)

gtest_discover_tests(csv_test)

add_executable(parse_test parse.cc)
target_link_libraries(parse_test PRIVATE common_options)

target_link_libraries(parse_test PRIVATE
  parse
  GTest::gtest
  GTest::gtest_main
)

gtest_discover_tests(parse_test)

add_executable(engine_test engine.cc)
target_link_libraries(engine_test PRIVATE common_options)

target_link_libraries(engine_test PRIVATE
  engine
  parse
  GTest::gtest
  GTest::gtest_main
)

gtest_discover_tests(engine_test)

endif (${GTest_FOUND})
