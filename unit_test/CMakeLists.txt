add_executable(unit_test_soa ut_soa.cpp)
add_executable(unit_test_aos ut_aos.cpp)

target_link_libraries(unit_test_soa common GTest::gtest_main)
target_link_libraries(unit_test_aos common GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(unit_test_soa)
gtest_discover_tests(unit_test_aos)
