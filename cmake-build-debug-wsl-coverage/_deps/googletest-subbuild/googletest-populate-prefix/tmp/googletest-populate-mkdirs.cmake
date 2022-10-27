# Distributed under the OSI-approved BSD 3-Clause License.  See accompanying
# file Copyright.txt or https://cmake.org/licensing for details.

cmake_minimum_required(VERSION 3.5)

file(MAKE_DIRECTORY
  "/mnt/c/Users/Usuario/CLionProjects/ca_p1/cmake-build-debug-wsl-coverage/_deps/googletest-src"
  "/mnt/c/Users/Usuario/CLionProjects/ca_p1/cmake-build-debug-wsl-coverage/_deps/googletest-build"
  "/mnt/c/Users/Usuario/CLionProjects/ca_p1/cmake-build-debug-wsl-coverage/_deps/googletest-subbuild/googletest-populate-prefix"
  "/mnt/c/Users/Usuario/CLionProjects/ca_p1/cmake-build-debug-wsl-coverage/_deps/googletest-subbuild/googletest-populate-prefix/tmp"
  "/mnt/c/Users/Usuario/CLionProjects/ca_p1/cmake-build-debug-wsl-coverage/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp"
  "/mnt/c/Users/Usuario/CLionProjects/ca_p1/cmake-build-debug-wsl-coverage/_deps/googletest-subbuild/googletest-populate-prefix/src"
  "/mnt/c/Users/Usuario/CLionProjects/ca_p1/cmake-build-debug-wsl-coverage/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp"
)

set(configSubDirs )
foreach(subDir IN LISTS configSubDirs)
    file(MAKE_DIRECTORY "/mnt/c/Users/Usuario/CLionProjects/ca_p1/cmake-build-debug-wsl-coverage/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp/${subDir}")
endforeach()
if(cfgdir)
  file(MAKE_DIRECTORY "/mnt/c/Users/Usuario/CLionProjects/ca_p1/cmake-build-debug-wsl-coverage/_deps/googletest-subbuild/googletest-populate-prefix/src/googletest-populate-stamp${cfgdir}") # cfgdir has leading slash
endif()
