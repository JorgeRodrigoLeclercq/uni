add_executable(ftest-aos main.cpp
        ../imgaos/info.cpp
        ../imgaos/info.hpp
        )
target_link_libraries (ftest-aos imgaos common Microsoft.GSL::GSL)