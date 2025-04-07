# Executable for all unit tests with list of sources
# For example, you may have one ∗_test.cpp for each ∗.cpp in common
add_executable(utest-common
        utest_header.cpp
        utest_progargs.cpp
        utest_pixel_structures.cpp
        utest_priority_queue.cpp
)
# Library dependencies
target_link_libraries (utest-common
        PRIVATE
        common
        GTest::gtest_main
        Microsoft.GSL::GSL)

include(GoogleTest)
gtest_discover_tests(utest-common)