add_executable(imtool-soa main.cpp
        functions.cpp
        ../common/binaryio.hpp
        ../common/progargs.hpp)
target_link_libraries (imtool-soa PRIVATE imgsoa common  Microsoft.GSL::GSL)