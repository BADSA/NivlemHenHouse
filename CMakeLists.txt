cmake_minimum_required(VERSION 3.3)
project(NivlemHenHouse)

INCLUDE_DIRECTORIES(/usr/lib/x86_64-linux-gnu)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -lm -lallegro")
set(SOURCE_FILES main.cpp utils/utils.cpp utils/graphics.cpp)
add_executable(NivlemHenHouse  ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(NivlemHenHouse
        allegro_acodec
        allegro_audio
        allegro_color
        allegro_dialog
        allegro_image
        allegro_main
        allegro_memfile
        allegro_physfs
        allegro_primitives
        allegro_ttf
        allegro_font
        allegro)