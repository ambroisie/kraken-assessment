find_package(GTest)

if (${GTest_FOUND})
include(GoogleTest)

add_executable(base_test base.cc)
target_link_libraries(base_test PRIVATE common_options)

target_link_libraries(base_test PRIVATE
  GTest::gtest
  GTest::gtest_main
)

gtest_discover_tests(base_test)
endif (${GTest_FOUND})
