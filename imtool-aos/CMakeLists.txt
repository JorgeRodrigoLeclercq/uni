add_executable(imtool-aos main.cpp
        functions.cpp
        ../imtool-soa/functions.cpp
        ../common/binaryio.hpp
        ../common/progargs.hpp)
target_link_libraries (imtool-aos imgaos common Microsoft.GSL::GSL)