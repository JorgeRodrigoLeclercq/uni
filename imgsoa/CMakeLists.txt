# Add to this list all files related to imgsoa library
add_library(imgsoa
        info.cpp
        info.hpp
        cutfreq.cpp
        cutfreq.hpp
        resize.cpp
        resize.hpp
        maxlevel.cpp
        maxlevel.hpp
        compress.cpp
        compress.hpp
        ../common/binaryio.hpp
        ../common/progargs.hpp
        ../common/priority_queue.hpp
)
# Use this line only if you have dependencies from this library to GSL
target_link_libraries (imgsoa PRIVATE common Microsoft.GSL::GSL)