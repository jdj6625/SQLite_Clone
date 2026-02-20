add_executable(Google_Tests_run
        test_table.cpp
        test_row.cpp
        test_input_processing.cpp
        test_input_buffer.cpp
)

target_link_libraries(Google_Tests_run
    PRIVATE
        sqlite_core
        GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(Google_Tests_run)