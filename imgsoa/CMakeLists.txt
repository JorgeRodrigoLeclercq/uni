# Add to this list all files related to imgsoa library
add_library(imgsoa
        imagesoa.cpp
        ../imtool-aos/functions.cpp
        ../imtool-soa/functions.cpp
        ../common/binaryio.hpp
        ../common/progargs.hpp
        ../common/priority_queue.hpp
)
# Use this line only if you have dependencies from this library to GSL
target_link_libraries (imgsoa PRIVATE common Microsoft.GSL::GSL)