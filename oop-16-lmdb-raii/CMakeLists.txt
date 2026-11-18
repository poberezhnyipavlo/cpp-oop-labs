cmake_minimum_required(VERSION 3.25)
project(oop_16_lmdb_raii LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(SANITIZE "AddressSanitizer + UBSan" OFF)

add_compile_options(-Wall -Wextra -Werror)

if(SANITIZE)
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
endif()

# LMDB from Homebrew (Mac) or /usr/local; the same search as in the monitor (SM-3).
find_path(LMDB_INCLUDE_DIR lmdb.h PATHS /opt/homebrew/include /usr/local/include REQUIRED)
find_library(LMDB_LIBRARY lmdb PATHS /opt/homebrew/lib /usr/local/lib REQUIRED)
find_package(Threads REQUIRED)

# The wrappers as a library: the experiments, the tests and the homework link the same code.
add_library(
        lmdb_raii STATIC
        Lmdb.cpp
        Lmdb.h
)
target_include_directories(lmdb_raii PUBLIC ${LMDB_INCLUDE_DIR})
target_link_libraries(lmdb_raii PUBLIC ${LMDB_LIBRARY} Threads::Threads)

add_library(
        lab_lmdb STATIC
        LabLmdb.cpp
        LabLmdb.h
)
target_link_libraries(lab_lmdb PUBLIC lmdb_raii)

add_executable(oop-16 main.cpp)
target_link_libraries(oop-16 PRIVATE lab_lmdb)

include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.17.0
        GIT_SHALLOW TRUE
        SYSTEM
)
set(INSTALL_GTEST OFF CACHE BOOL "" FORCE)
set(BUILD_GMOCK OFF CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
# -Werror is for our code: a newer compiler may warn inside googletest itself.
target_compile_options(gtest PRIVATE -Wno-error)
target_compile_options(gtest_main PRIVATE -Wno-error)

enable_testing()
add_executable(oop-16-tests LmdbTest.cpp)
target_link_libraries(oop-16-tests PRIVATE lab_lmdb GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(oop-16-tests)

add_executable(oop-16-homework homework.cpp)
target_link_libraries(oop-16-homework PRIVATE lab_lmdb)