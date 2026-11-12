cmake_minimum_required(VERSION 3.20)
project(oop_14_lambdas LANGUAGES CXX)

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

# std::jthread потребує бібліотеки потоків (на Маку — нічого не робить, на Linux — -pthread).
find_package(Threads REQUIRED)

add_executable(oop-14
        main.cpp
        Tracer.cpp
        Tracer.h
)

target_link_libraries(oop-14 PRIVATE Threads::Threads)

#
#add_executable(oop-14-homework
#        homework.cpp
#        SensorList.cpp
#        SensorList.h
#        Sensor.cpp
#        Sensor.h
#        StableSensorList.cpp
#        StableSensorList.h
#)
