add_executable(imtool-soa main.cpp
        ../imgsoa/info.cpp
        ../imgsoa/info.hpp
        ../common/binaryio.hpp
        ../common/progargs.hpp
        ../common/priority_queue.hpp
        ../imgsoa/cutfreq.hpp
        ../imgsoa/cutfreq.cpp
        ../imgsoa/resize.cpp
        ../imgsoa/resize.hpp
        ../imgsoa/maxlevel.cpp
        ../imgsoa/maxlevel.hpp
        ../imgsoa/compress.hpp
        ../imgsoa/compress.cpp)
target_link_libraries (imtool-soa PRIVATE imgsoa common  Microsoft.GSL::GSL)