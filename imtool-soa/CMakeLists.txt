add_executable(imtool-soa main.cpp
        functions.cpp
        ../common/binaryio.hpp
        ../common/progargs.hpp
        ../common/priority_queue.hpp)
target_link_libraries (imtool-soa PRIVATE imgsoa common  Microsoft.GSL::GSL)