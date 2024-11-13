# Add to this list all files related to common library
add_library(common
        binaryio.cpp
        ../imtool-aos/info.cpp
        binaryio.hpp
        progargs.hpp
        header.hpp
        priority_queue.hpp
        pixel_structures.hpp
        ../utest-imgaos/utest_maxlevel.cpp
        ../utest-imgaos/utest_maxlevel.cpp

)
# Use this line only if you have dependencies from this library to GSL
target_link_libraries (common PRIVATE Microsoft.GSL::GSL)