cmake_minimum_required(VERSION 3.23)
project(Prueba_CA)

set(CMAKE_CXX_STANDARD 20)



set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS_OFF)
add_compile_options(-Wall -Wextra -Werror -pedantic -pedantic-errors)
set(CMAKE_CXX_FLAGS_RELEASE "-march=native")
add_library(common progargs.cpp common.cpp common_hst.cpp common_gauss.cpp)
add_library(soa soa.cpp)
add_library(aos aos.cpp)
add_executable(image-soa imgsoa.cpp)
target_link_libraries(image-soa soa)
add_executable(image-aos imgaos.cpp)
target_link_libraries(image-aos aos)



#add_executable(Prueba_CA main.cpp soa.hpp soa.cpp common.hpp common.cpp imgsoa.cpp)
