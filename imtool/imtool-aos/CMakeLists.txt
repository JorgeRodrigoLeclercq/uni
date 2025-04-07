add_executable(imtool-aos main.cpp
        ../imgaos/info.cpp
        ../imgaos/info.hpp
        ../common/binaryio.hpp
        ../common/progargs.hpp
        ../common/priority_queue.hpp
        ../imgaos/cutfreq.cpp
        ../imgaos/cutfreq.hpp
        ../imgaos/resize.cpp
        ../imgaos/resize.hpp
        ../imgaos/maxlevel.cpp
        ../imgaos/maxlevel.hpp
        ../imgaos/compress.cpp
        ../imgaos/compress.hpp
        )
target_link_libraries (imtool-aos imgaos common Microsoft.GSL::GSL)