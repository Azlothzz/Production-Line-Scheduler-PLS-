cmake_minimum_required(VERSION 3.22)
project(pls C)

set(CMAKE_C_STANDARD 11)
LINK_LIBRARIES(m)

add_executable(pls
        scheduler.c
        util.h
        order.h
        main.c
)
