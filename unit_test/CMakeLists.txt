add_executable(copy_test copy_test.cpp)

target_link_libraries(copy_test common GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(copy_test)