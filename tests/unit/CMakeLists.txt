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
endif (${GTest_FOUND})
