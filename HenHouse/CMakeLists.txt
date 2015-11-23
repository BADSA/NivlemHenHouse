cmake_minimum_required(VERSION 3.0)
project(NivlemHenHouse)

INCLUDE_DIRECTORIES(/usr/lib/x86_64-linux-gnu)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -pthread -lm -lallegro")
set(SOURCE_FILES main.cpp ../src/client.cpp ../src/utils.cpp ../src/mathfunc.cpp
        ../src/server.cpp ../src/graphics.cpp ../src/bot.cpp ../src/chicken.cpp
        ../src/nivlem.cpp ../src/workflow.cpp)
add_executable(HenHouse  ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(HenHouse
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