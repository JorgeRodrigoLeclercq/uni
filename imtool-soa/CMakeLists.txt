add_executable(imtool-soa main.cpp
        functions.cpp)
target_link_libraries (imtool-soa imgsoa common)