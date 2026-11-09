cmake_minimum_required(VERSION 3.20)
project(oop_08_raii LANGUAGES CXX)

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

add_executable(oop-08
        main.cpp
        File.cpp
        File.h
        Logger.cpp
        Logger.h
)

add_executable(oop-08-homework
        homework.cpp
        Socket.cpp
        Socket.h
        FrameBuffer.h
        FrameBuffer.cpp
        Transaction.cpp
        Transaction.h
)
