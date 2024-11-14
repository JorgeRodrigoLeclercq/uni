# Executable for all unit tests with list of sources
# For example, you may have one ∗_test.cpp for each ∗.cpp in img−aos
add_executable(utest-imgaos
        utest_maxlevel_aos.cpp
        utest_resize_aos.cpp
        utest_info_aos.cpp)
# Library dependencies
target_link_libraries (utest-imgaos
        PRIVATE
        imgaos
        GTest::gtest_main
        Microsoft.GSL::GSL)
include(GoogleTest)
gtest_discover_tests(utest-imgaos)