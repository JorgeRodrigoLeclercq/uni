# Add to this list all files related to common library
add_library(common
        progargs.cpp
        binaryio.cpp
        ../imtool-aos/functions.cpp
        ../imtool-soa/functions.cpp
        binaryio.hpp
        progargs.hpp
        priority_queue.hpp
        ../pruebas.cpp
)
# Use this line only if you have dependencies from this library to GSL
target_link_libraries (common PRIVATE Microsoft.GSL::GSL)