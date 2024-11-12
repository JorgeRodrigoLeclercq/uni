add_executable(imtool-aos main.cpp
        info.cpp
        info.hpp
        ../common/binaryio.hpp
        ../common/progargs.hpp
        ../common/priority_queue.hpp
        cutfreq.cpp
        cutfreq.hpp
        resize.cpp
        resize.hpp
        maxlevel.cpp
        maxlevel.hpp
        compress.cpp
        compress.hpp
        )
target_link_libraries (imtool-aos imgaos common Microsoft.GSL::GSL)