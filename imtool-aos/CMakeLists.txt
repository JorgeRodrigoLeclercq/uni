add_executable(imtool-aos main.cpp
        functions.cpp
        ../imtool-soa/functions.cpp
        ../common/binaryio.hpp
        ../common/progargs.hpp
        ../common/priority_queue.hpp
        cutfreq.cpp
        cutfreq.hpp
        Resize.cpp
        Resize.hpp)
target_link_libraries (imtool-aos imgaos common Microsoft.GSL::GSL)