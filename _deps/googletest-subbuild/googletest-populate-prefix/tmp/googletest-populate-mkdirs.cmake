# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/home/jorge/CLionProjects/imtool/_deps/googletest-src"
  "/home/jorge/CLionProjects/imtool/_deps/googletest-build"
  "/home/jorge/CLionProjects/imtool/_deps/googletest-subbuild/googletest-populate-prefix"
  "/home/jorge/CLionProjects/imtool/_deps/googletest-subbuild/googletest-populate-prefix/tmp"
  "/home/jorge/CLionProjects/imtool/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp"
  "/home/jorge/CLionProjects/imtool/_deps/googletest-subbuild/googletest-populate-prefix/src"
  "/home/jorge/CLionProjects/imtool/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/home/jorge/CLionProjects/imtool/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/home/jorge/CLionProjects/imtool/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
