cmake_minimum_required(VERSION 3.23)
project(image LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS_OFF)
add_compile_options(-Wall -Wextra -Werror -pedantic -pedantic-errors)

set(CMAKE_CXX_FLAGS_RELEASE "-march=native")

add_library(common progargs.cpp common_rw.cpp common_hst.cpp common.cpp common_gauss.cpp common_mono.cpp)
add_library(soa soa.cpp)
add_library(aos aos.cpp)

add_executable(image-soa imgsoa.cpp)
target_link_libraries(image-soa common soa)
add_executable(image-aos imgaos.cpp)
target_link_libraries(image-aos common aos)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG release-1.12.1
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_subdirectory(unit_test)
