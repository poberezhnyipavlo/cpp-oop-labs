cmake_minimum_required(VERSION 3.20)
project(oop_15_templates LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

option(SANITIZE "AddressSanitizer + UBSan" OFF)

add_compile_options(-Wall -Wextra -Werror)

# g++ 13+ попереджає про висячі посилання (-Wdangling-reference) — у цій лабораторній вони навмисні:
# лишаємо попередження видимим, але не фатальним. clang такого попередження не має.
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    add_compile_options(-Wno-error=dangling-reference)
endif()

if(SANITIZE)
    add_compile_options(-fsanitize=address,undefined -fno-omit-frame-pointer)
    add_link_options(-fsanitize=address,undefined)
endif()

add_executable(oop-15
        main.cpp
        Sensor.cpp
        Sensor.h
        Sampling.h
)

#
#add_executable(oop-15-homework
#        homework.cpp
#)
