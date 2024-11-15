add_executable(ftest-soa main.cpp
        ../imgsoa/info.cpp
        ../imgsoa/info.hpp
        )
target_link_libraries (ftest-soa imgsoa common Microsoft.GSL::GSL)