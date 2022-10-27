add_executable(unit_test unit_test.cpp)

target_link_libraries(unit_test common GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(unit_test)