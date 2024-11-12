add_executable(imtool-soa main.cpp
        info.cpp
        info.hpp
        ../common/binaryio.hpp
        ../common/progargs.hpp
        ../common/priority_queue.hpp
        cutfreq.hpp
        cutfreq.cpp
        resize.cpp
        resize.hpp
        maxlevel.cpp
        maxlevel.hpp
        compress.hpp
        compress.cpp)
target_link_libraries (imtool-soa PRIVATE imgsoa common  Microsoft.GSL::GSL)