# Executable for all unit tests with list of sources
# For example, you may have one ∗_test.cpp for each ∗.cpp in img-soa
add_executable(utest-imgsoa
        utest_maxlevel_soa.cpp
        utest_resize_soa.cpp
        utest_info_soa.cpp
        utest_compress_soa.cpp
        utest_cutfreq_soa.cpp
)
# Library dependencies
target_link_libraries (utest-imgsoa
        PRIVATE
        imgsoa
        GTest::gtest_main
        Microsoft.GSL::GSL)
include(GoogleTest)
gtest_discover_tests(utest-imgsoa)
