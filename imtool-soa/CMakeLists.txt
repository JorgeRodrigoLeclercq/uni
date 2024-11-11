add_executable(imtool-soa main.cpp
        functions.cpp
        functions.hpp
        ../common/binaryio.hpp
        ../common/progargs.hpp
        ../common/priority_queue.hpp
        cutfreq.hpp
        cutfreq.cpp
        ReSize.cpp
        ReSize.hpp
        maxlevel.cpp
        maxlevel.cpp
        maxlevel.hpp)
target_link_libraries (imtool-soa PRIVATE imgsoa common  Microsoft.GSL::GSL)